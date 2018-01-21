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
 */

#include "../../development/include/driver/uart.h"
#include "osapi.h"
#include "user_interface.h"

// see eagle_soc.h for these definitions
#define LED_GPIO 4
#define LED_GPIO_MUX PERIPHS_IO_MUX_GPIO4_U
#define LED_GPIO_FUNC FUNC_GPIO4

#define DELAY 1000 /* milliseconds */
#define SSID "navtah"
#define PASSWORD "qwas1234"

LOCAL os_timer_t blink_timer;
LOCAL uint8_t led_state=0;
LOCAL uint32_t asd;

LOCAL void ICACHE_FLASH_ATTR blink_cb(void *arg)
{
	GPIO_OUTPUT_SET(LED_GPIO, led_state);
	led_state ^=1;
	os_printf( "lol %d\n", asd++);
}

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

ICACHE_FLASH_ATTR
ConnSetup(void)
{
	wifi_set_opmode(0x01);
	wifi_softap_dhcps_stop();
	char ssid[32] =	SSID;
	char password[64] = PASSWORD;
	struct	station_config	stationConf;
	stationConf.bssid_set = 0;						//need	not	check	MAC	address
	os_memcpy(&stationConf.ssid, ssid, 32);
	os_memcpy(&stationConf.password, password, 64);
	wifi_station_set_config(&stationConf);


}

 ICACHE_FLASH_ATTR
user_init(void)
{
	 asd = 0;
	ConnSetup();
	uart_div_modify( 0, UART_CLK_FREQ / ( 115200 ) );
	os_printf( "Helloworld\n");
	os_printf("SDK version:%s\n", system_get_sdk_version());
	// Configure pin as a GPIO
	PIN_FUNC_SELECT(LED_GPIO_MUX, LED_GPIO_FUNC);
	// Set up a timer to blink the LED
	// os_timer_disarm(ETSTimer *ptimer)
	os_timer_disarm(&blink_timer);
	// os_timer_setfn(ETSTimer *ptimer, ETSTimerFunc *pfunction, void *parg)
	os_timer_setfn(&blink_timer, (os_timer_func_t *)blink_cb, (void *)0);
	// void os_timer_arm(ETSTimer *ptimer,uint32_t milliseconds, bool repeat_flag)
	os_timer_arm(&blink_timer, DELAY, 1);
}


