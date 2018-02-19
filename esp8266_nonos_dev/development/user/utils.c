/*
 * utils.c
 *
 *  Created on: Jan 22, 2018
 *      Author: nolex
 */
#include "../../development/include/driver/uart.h"
#include "../include/settings.h"
#include "osapi.h"
#include "user_interface.h"


LOCAL os_timer_t blink_timer;
LOCAL uint8_t led_state=0;
LOCAL uint32_t asd;


//GPIO15(TX) && 13(RX) will used
ICACHE_FLASH_ATTR
void switchUart1()
{
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTCK_U, FUNC_U0CTS);//CONFIG MTCK PIN FUNC TO U0CTS
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTDO_U, FUNC_U0RTS);//CONFIG MTDO PIN FUNC TO U0RTS
	SET_PERI_REG_MASK(0x3ff00028 , BIT2);//SWAP PIN : U0TXD<==>U0RTS(MTDO) , U0RXD<==>U0CTS(MTCK)
}

ICACHE_FLASH_ATTR
connSetup(void)
{
	wifi_set_opmode(0x01);	//station
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
void heartBeatCbkGpio()
{
	GPIO_OUTPUT_SET(LED_GPIO, led_state);
	led_state ^=1;
	os_printf( "lol %d\n", asd++);
}

ICACHE_FLASH_ATTR
void initHeartBeat()
{
	asd = 0;
	PIN_FUNC_SELECT(LED_GPIO_MUX, LED_GPIO_FUNC);
	os_timer_disarm(&blink_timer);
	os_timer_setfn(&blink_timer, (os_timer_func_t *)heartBeatCbkGpio, (void *)0);
	os_timer_arm(&blink_timer, DELAY, 1);
}

