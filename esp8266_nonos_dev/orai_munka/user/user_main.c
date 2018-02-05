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


#define LED_GPIO_MUX
#define LED_GPIO_FUNC

//IO12 LED

static uint8_t led_state;
static os_timer_t blinkTimer;

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


void heartBeat()
{
	GPIO_OUTPUT_SET(12, led_state);
	led_state ^= 1;
	os_printf("heartBeat!\n\r");
}


 user_init(void)
{
	 uart_div_modify(UART0, UART_CLK_FREQ/115200);
	 led_state = 0;
	 PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTDI_U, FUNC_GPIO12);
	 os_timer_setfn(&blinkTimer, (os_timer_func_t*)heartBeat, 0);
	 os_timer_arm(&blinkTimer, 500, 1);
}




