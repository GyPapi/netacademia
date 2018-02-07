/*
 * dht.c
 *
 *  Created on: Feb 4, 2018
 *      Author: nolex
 */

#include "dht.h"
static uint32_t DhtTimeOffsetPast;
static uint32_t DhtTimeOffset;
static uint8_t DhtResultIdx;
static uint32_t DhtResult[42];
static uint8_t DhtReadSts;
static os_timer_t DhtTimer;

static os_timer_t DhtApiTimer;

#if DHT_TYPE == DHT11
static uint32_t DhtHum;
static uint32_t DhtTemp;
#else
static float DhtHum;
static float DhtTemp;
#endif

void DhtTimerCbk();
void DhtGpioCbk();


ICACHE_FLASH_ATTR void DhtInit()
{
    PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTMS_U,FUNC_GPIO14);
    //DHT_PIN_OFF();
    GPIO_OUTPUT_SET(DHT_PIN, 1);
    DhtReadSts = 0;
}

ICACHE_FLASH_ATTR void DhtReadStart()
{
	GPIO_OUTPUT_SET(DHT_PIN, 1);
	os_delay_us(300);
	GPIO_OUTPUT_SET(DHT_PIN, 0);
	os_timer_disarm(&DhtTimer);
	os_timer_setfn(&DhtTimer, (os_timer_func_t *)DhtTimerCbk, (void *)0);
	os_timer_arm(&DhtTimer, 20, 1);
}

void ICACHE_FLASH_ATTR DhtGpioCbkSetup()
{
    ETS_GPIO_INTR_DISABLE();
    GPIO_DIS_OUTPUT(DHT_PIN);
    ETS_GPIO_INTR_ATTACH(&DhtGpioCbk, NULL);
    gpio_register_set(GPIO_PIN_ADDR(DHT_PIN), GPIO_PIN_INT_TYPE_SET(GPIO_PIN_INTR_DISABLE)
                      | GPIO_PIN_PAD_DRIVER_SET(GPIO_PAD_DRIVER_DISABLE)
                      | GPIO_PIN_SOURCE_SET(GPIO_AS_PIN_SOURCE));

    GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, BIT(DHT_PIN));
    gpio_pin_intr_state_set(GPIO_ID_PIN(DHT_PIN), GPIO_PIN_INTR_NEGEDGE);
    ETS_GPIO_INTR_ENABLE();
    DhtResultIdx = 0;
    os_memset(&DhtResult, 0, sizeof(DhtResult));
    DhtTimeOffset = WDEW_NOW();
}

void ICACHE_FLASH_ATTR DhtProcess()
{
	uint8_t result[5];
	uint8_t bytes;
	uint8_t bits, temp, offset;
	bits = 2;
	for(bytes = 0; bytes < 5 ; bytes++)
	{
		offset = bits;
		for(temp = 0; temp < 8; temp++)
		{
			if(DhtResult[bits] > 100)
				result[bytes] |= 0x1 << (bits-offset);
			bits++;
		}
	}
	os_printf("Result: %d, %d, %d ,%d, %d\n\r",result[0], result[1], result[2], result[3], result[4]);
	if((result[0] + result[1] + result[2] + result[3]) == result[4])
	{
		os_printf("parity_ok!!\n\r");
	}
	else
	{
		os_printf("parity_NOK!!\n\r");
	}
#if DHT_TYPE == DHT11
	DhtHum = result[0];
	DhtTemp = result[2];
#else
	uint16_t rawhumidity = result[0]<<8 | result[1];
	uint16_t rawtemperature = result[2]<<8 | result[3];
	if(rawtemperature & 0x8000)
	{
		DhtTemp = (float)((rawtemperature & 0x7FFF) / 10.0) * -1.0;
	}
	else
	{
		DhtTemp = (float)(rawtemperature)/10.0;
	}
	DhtHum = (float)(rawhumidity)/10.0;
#endif
}

void DhtGpioCbk()
{
	uint32 gpio_status;
	gpio_status = GPIO_REG_READ(GPIO_STATUS_ADDRESS);
	//clear
	if (gpio_status & BIT(DHT_PIN))
	{
		DhtResult[DhtResultIdx++] = WDEW_NOW() - DhtTimeOffset;
		DhtTimeOffset = WDEW_NOW();
		if(DhtResultIdx > 41)
		{
			os_printf("Results in us:\n\r***\n\r");
			uint8_t t;
			for(t = 0; t < 42; t++)
				os_printf("|%d : %d \n\r",t, DhtResult[t]);
			ETS_GPIO_INTR_DISABLE();
			DhtProcess();
			DhtReadSts = 2;
		}
		GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, gpio_status & BIT(DHT_PIN));
	}

}

void DhtTimerCbk()
{
	GPIO_OUTPUT_SET(DHT_PIN, 1);
	GPIO_DIS_OUTPUT(DHT_PIN);
	DhtGpioCbkSetup();
}

#if DHT_TYPE == DHT11
ICACHE_FLASH_ATTR int32_t DhtReadTemp()
#else
ICACHE_FLASH_ATTR float DhtReadTemp()
#endif
{
	if(0 == DhtReadSts)
	{
		os_timer_setfn(&DhtApiTimer, (os_timer_func_t *)DhtReadTemp, (void *)0);
		os_timer_arm(&DhtTimer, 100, 1);
		DhtReadSts = 1;
		DhtInit();
		DhtReadStart();
	}
	else if (2 == DhtReadSts)
	{
		os_timer_disarm(&DhtApiTimer);
		return DhtTemp;
	}

}
/*
ICACHE_FLASH_ATTR void DhtTestSeq()
{
	DhtInit();
	DhtReadStart();
}
*/
