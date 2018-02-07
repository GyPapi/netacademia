/*
 * dht.c
 *
 *  Created on: Feb 7, 2018
 *      Author: nolex
 */

#include "dht.h"

static uint8_t DhtReadSts;
static os_timer_t DhtTimer;
static uint8_t DhtResultIdx;
static uint8_t DhtResult[42];
static uint32_t DhtTimerOffset;

void dhtGpioCbk()
{

}

ICACHE_FLASH_ATTR void dhtGpioCbkSetup()
{
	ETS_GPIO_INTR_DISABLE();
	GPIO_DIS_OUTPUT(DHT_PIN);
	ETS_GPIO_INTR_ATTACH(&dhtGpioCbk, NULL);
	gpio_register_set(GPIO_PIN_ADDR(DHT_PIN), GPIO_PIN_INT_TYPE_SET(GPIO_PIN_INTR_DISABLE)
				| GPIO_PIN_PAD_DRIVER_SET(GPIO_PAD_DRIVER_DISABLE)
				| GPIO_PIN_SOURCE_SET(GPIO_AS_PIN_SOURCE));

	GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, BIT(DHT_PIN));
	gpio_pin_intr_state_set(GPIO_ID_PIN(DHT_PIN), GPIO_PIN_INTR_NEGEDGE);
	ETS_GPIO_INTR_ENABLE();
	DhtResultIdx = 0;
	os_memset(&DhtResult, 0, sizeof(DhtResult));
	DhtTimerOffset = WDEW_NOW();

}

void dhtTimerCbk()
{
	DHT_PIN_DIS();
	dhtGpioCbkSetup();
}

ICACHE_FLASH_ATTR void dhtProcess()
{

}

ICACHE_FLASH_ATTR void dhtInit()
{
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTMS_U, FUNC_GPIO14);
	DHT_PIN_ON();
	DhtReadSts++;
}

ICACHE_FLASH_ATTR void dhtStart()
{
	dhtInit();
	os_delay_us(500);
	DHT_PIN_OFF();
	os_timer_disarm(&DhtTimer);
	os_timer_setfn(&DhtTimer, (os_timer_t*)dhtTimerCbk, (void*)0);
	os_timer_arm(&DhtTimer, 20, 0);

}

ICACHE_FLASH_ATTR void dhtTest()
{

}
