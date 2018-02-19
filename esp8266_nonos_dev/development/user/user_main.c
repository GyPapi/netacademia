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

#include "../../development/include/driver/uart.h"

#include "osapi.h"
#include "user_interface.h"

#include "dht.h"
#include "hcsr04.h"

#define MAIN_TASK_PRIO 		0
#define MAIN_TASK_Q_SIZE    3
static os_event_t mainTaskQ[MAIN_TASK_Q_SIZE];
static os_timer_t mainTaskTimer;
static os_timer_t dummyTimer;
static uint8_t printbuf[64];
extern ICACHE_FLASH_ATTR void initSockets();

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

void ICACHE_FLASH_ATTR mainTask(os_event_t *ev)
{
	os_printf("mainTask signal: %d, \n\r", ev->sig );
	if(ev->sig == DHT_DRIVER_SOURCE)
	{
		DhtHandler* Dht = (DhtHandler*)ev->par;
		if (ev->par == 0)
			return;
		if(Dht->state == COMPL)
		{
			ets_sprintf(printbuf, "%d.%d", (int)(Dht->DhtTemp),(int)((Dht->DhtTemp - (int)Dht->DhtTemp)*100));
			os_printf("DhtTemp: %s\n\r", printbuf);
			ets_sprintf(printbuf, "%d.%d", (int)(Dht->DhtHum),(int)((Dht->DhtHum - (int)Dht->DhtHum)*100));
			os_printf("DhtHum: %s\n\r", printbuf);
			Dht->state = IDLE;
		}
		else if(Dht->state == ERR)
		{
			os_sprintf(printbuf,"ERR in DHT reading, recall\n\r" );
			os_printf("%s", printbuf);
			DhtTestSeq();
		}
	}
	else if(ev->sig == HCSR04_DRIVER_SOURCE)
	{
		Hcsr04Handler_t* Hcsr04 = (Hcsr04Handler_t*)ev->par;
		if (ev->par == 0)
			return;
		if(Hcsr04->state == COMPL)
		{
			ets_sprintf(printbuf, "%d.%d", (int)(Hcsr04->result),(int)((Hcsr04->result - (int)Hcsr04->result)*100));
			os_printf("Hcsr04: %s\n\r", printbuf);
		}
		else
		{
			os_printf("unhandled state in Hcsr04 sensortask\n\r");
		}
	}
	else
	{
		os_printf("MainTask no handled command\n\r");
	}


}

void TestLaterSeq()
{
	//DhtTestSeq();
	Hcsr04Start();
}
/*
void wifi_handle_event_cb(System_Event_t *evt)
{
	os_printf("event	%x\n",	evt->event);
	switch	(evt->event)
	{
		case	EVENT_STAMODE_CONNECTED:
			os_printf("connect	to	ssid	%s,	channel	%d\n",
			evt->event_info.connected.ssid,
			evt->event_info.connected.channel);
		break;
		case	EVENT_STAMODE_DISCONNECTED:
			os_printf("disconnect	from	ssid	%s,	reason	%d\n",
			evt->event_info.disconnected.ssid,
			evt->event_info.disconnected.reason);
		break;
		case	EVENT_STAMODE_AUTHMODE_CHANGE:
			os_printf("mode:	%d	->	%d\n",
			evt->event_info.auth_change.old_mode,
			evt->event_info.auth_change.new_mode);
		break;
		case	EVENT_STAMODE_GOT_IP:
			os_printf("ip:"	IPSTR	",mask:"	IPSTR	",gw:"	IPSTR,
			IP2STR(&evt->event_info.got_ip.ip),
			IP2STR(&evt->event_info.got_ip.mask),
			IP2STR(&evt->event_info.got_ip.gw));
			os_printf("\n");
			initUdpHeartbeat();
			daemonConInit();
		break;
		case	EVENT_SOFTAPMODE_STACONNECTED:
			os_printf("station:	"	MACSTR	"join,	AID	=	%d\n",
			MAC2STR(evt->event_info.sta_connected.mac),

			evt->event_info.sta_connected.aid);
		break;
		case	EVENT_SOFTAPMODE_STADISCONNECTED:
			os_printf("station:	"	MACSTR	"leave,	AID	=	%d\n",
			MAC2STR(evt->event_info.sta_disconnected.mac),
			evt->event_info.sta_disconnected.aid);
		break;
		default:
		break;
	}
}
*/
 ICACHE_FLASH_ATTR
 user_init(void)
{
	 GPIO_OUTPUT_SET(DHT_PIN, 1);
	initHeartBeat();
	connSetup();
	uart_div_modify( 0, UART_CLK_FREQ / ( 115200 ) );
	os_printf("\n\n\n\n\n\n\r**************************************************************\r\n");
	os_printf( "Helloworld\n\r");
	os_printf("SDK version:%s\n\r", system_get_sdk_version());
	//os_printf("Wdew: %x\n\r", WDEW_NOW());
	// create a task
	//system_os_task(my_task,MY_TASK_PRIORITY, g_my_queue, MY_QUEUE_SIZE);

	// create a timer

	// start the task for the first time
	//run_task();
	//task_demonstrate();
	//hw_timer_test_init();
	system_os_task(mainTask,MAIN_TASK_PRIO, mainTaskQ, MAIN_TASK_Q_SIZE);
	//DhtTestSeq();
	//Hcsr04Start();
	os_timer_setfn(&dummyTimer, (os_timer_func_t *)initSockets, (void *)0);
	os_timer_arm(&dummyTimer, 5000, 0);

	//wifi_set_event_hander_cb(wifi_handle_event_cb);


}




