/*
 * dht.c
 *
 *  Created on: Feb 4, 2018
 *      Author: nolex
 */

#include "dht.h"

static uint32_t DhtTimeOffset;
static uint32_t DhtReadSts;
static os_timer_t DhtTimer;

void DhtInit()
{
    PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTMS_U,FUNC_GPIO14);
    DHT_PIN_OFF();
}

float DhtReadStart()
{
	DhtReadSts = 1;
	DHT_PIN_ON();
	os_delay_us(100);
	DHT_PIN_OFF();
	os_timer_disarm(&DhtTimer);
	os_timer_setfn(&DhtTimer, (os_timer_func_t *)DhtTimerCbk, (void *)0);
	os_timer_arm(&DhtTimer, 20, 1);
	DHT_PIN_OFF();
}

void DhtTimerCbk()
{
	DHT_PIN_ON();
	uint8_t loop = 2;
	while(loop > 0)
	{
		if(loop == 2)
		{
			if(0 == DHT_READ_PIN())
				loop--;
		}
		else if(loop == 1)
		{
			if(1 == DHT_READ_PIN())
				loop--;
		}
		else
		{
			os_printf("dht unhandled state!!");
		}
	}
	loop = 0;
	while(loop < 40)
	{
		os_delay_us(25);
		if(0 == DHT_READ_PIN())
		{
			os_delay()
		}
	}


}
