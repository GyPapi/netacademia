/*
 * ESPRSSIF MIT License
 *
 * Copyright (c) 2016 <ESPRESSIF SYSTEMS (SHANGHAI) PTE LTD>
 *
 * Permission is hereby granted for use on ESPRESSIF SYSTEMS ESP8266 only, in which case,
 * it is free of charge, to any person obtaining a copy of this software and associated
 * documentation files (the "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished
 * to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Netacademia Development Folder
 * This lectures was held at netacademia.hu
 * Copyright Matyas Kiss, nolex90@gmail.com
 * Features:
 *  PWM, Knight rider effect
 *  Soft PWM if possible
 *  HC-SR04 ultrasound sensor driver
 *  DHT11 driver
 *  Button
 * Pins used:
 *  HCSR04: out IO13 in: IO4
 *  DHT11: IO14
 *  LED: IO12
 *  BTN: IO5
 *
 */

#include "osapi.h"
#include "user_interface.h"
#include "../include/driver/uart.h"

#include "utils.h"
#include "dht.h"

#include "heartbeat.h"
#include "daemon.h"

//Main task definition zone
#define MAIN_TASK_PRIO		0
#define MAIN_TASK_Q_SIZE	2
static os_event_t mainTaskQ[MAIN_TASK_Q_SIZE];
static os_timer_t mainTaskTimer;
static os_timer_t udpHeartBeatStartTimer;
static os_timer_t daemonStartTimer;
static uint8_t printbuf[32];

int32 ICACHE_FLASH_ATTR
user_rf_cal_sector_set(void)
{
    enum flash_size_map size_map = system_get_flash_size_map();
    uint32 rf_cal_sec = 0;

    switch (size_map) {
        case FLASH_SIZE_4M_MAP_256_256:
            rf_cal_sec = 128 - 5;
            break;

        case FLASH_SIZE_8M_MAP_512_512:
            rf_cal_sec = 256 - 5;
            break;

        case FLASH_SIZE_16M_MAP_512_512:
        case FLASH_SIZE_16M_MAP_1024_1024:
            rf_cal_sec = 512 - 5;
            break;

        case FLASH_SIZE_32M_MAP_512_512:
        case FLASH_SIZE_32M_MAP_1024_1024:
            rf_cal_sec = 1024 - 5;
            break;

        default:
            rf_cal_sec = 0;
            break;
    }

    return rf_cal_sec;
}


void mainTask(os_event_t *e)
{
	DhtHandler* Dht = (DhtHandler*)e->par;
	os_printf("mainTask running...\n\r");
	if(e->par == 0)
	{
		return;
	}
	switch(Dht->state)
	{
		case ERR:
			os_printf("Error in dht reading\n\r");
			dhtStart();
			break;
		case COMPL:
			//os_printf("DhtReading complete: %d, %d\n\r", Dht->DhtHum, Dht->DhtTemp);
			os_sprintf(printbuf,"%d,%d", (int)(Dht->DhtHum), (int)((Dht->DhtHum - (int)Dht->DhtHum)*100));
			os_printf("DhtHum: %s\n\r", printbuf);
			os_sprintf(printbuf,"%d,%d", (int)(Dht->DhtTemp), (int)((Dht->DhtTemp - (int)Dht->DhtTemp)*100));
			os_printf("DhtTemp: %s\n\r", printbuf);
			break;
		default:
			os_printf("Default state\n\r");
	}

	//testUltraS();
/*
	dhtTest();

	os_timer_disarm(&mainTaskTimer);
	os_timer_setfn(&mainTaskTimer, (os_timer_func_t*) mainTask, (void*) 0);
	os_timer_arm(&mainTaskTimer, 1200, 1);
*/
}


 user_init(void)
{
	 initUtils();
	 system_os_task(mainTask, MAIN_TASK_PRIO, mainTaskQ, MAIN_TASK_Q_SIZE);
	 //system_os_post(MAIN_TASK_PRIO,0,0);
	 dhtStart();
	os_timer_setfn(&udpHeartBeatStartTimer, (os_timer_func_t*)initUdpHeartBeat, (void*)0);
	os_timer_arm(&udpHeartBeatStartTimer,6000,0);
	os_timer_setfn(&daemonStartTimer,  (os_timer_func_t*)daemonConInit, (void*)0);
	os_timer_arm(&daemonStartTimer,6000,0);
}




