/*
 * utils.c
 *
 *  Created on: Feb 5, 2018
 *      Author: nolex
 *      Implementing basic HeartBeat functionality.
 */

#include "utils.h"

static uint8_t led_state;
static os_timer_t blinkTimer;


void heartBeat()
{
	GPIO_OUTPUT_SET(12, led_state);
	led_state ^= 1;
	os_printf("heartBeat!\n\r");
}

void initBlinky()
{
	 uart_div_modify(UART0, UART_CLK_FREQ/115200);
	 led_state = 0;
	 PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTDI_U, FUNC_GPIO12);
	 os_timer_setfn(&blinkTimer, (os_timer_func_t*)heartBeat, 0);
	 os_timer_arm(&blinkTimer, LED_BLINKY_TIME, 1);
}

void initConnection()
{
	wifi_set_opmode(0x01);
	wifi_softap_dhcps_stop();
	char ssid[]  = "navtah10";
	char password[] = "qwas1234";
	struct station_config stationConf;
	stationConf.bssid_set = 0;
	os_memcpy(&stationConf.ssid, ssid, 32);
	os_memcpy(&stationConf.password, password, 64);
	wifi_station_set_config(&stationConf);


}

void initUtils()
{
	initBlinky();
	initConnection();
}
