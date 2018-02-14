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

static uint8_t DhtTimeout;

void DhtTimerCbk();
void DhtGpioCbk(void *arg);

static DhtHandler DhtHandle;

ICACHE_FLASH_ATTR void DhtInit()
{
    PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTMS_U,FUNC_GPIO14);
    GPIO_OUTPUT_SET(DHT_PIN, 1);
    DhtHandle.state = PREP;
    DhtTimeout = 0;
}

ICACHE_FLASH_ATTR void DhtReadStart()
{
	GPIO_OUTPUT_SET(DHT_PIN, 1);
	os_delay_us(200);
	GPIO_OUTPUT_SET(DHT_PIN, 0);
	os_timer_disarm(&DhtTimer);
	os_timer_setfn(&DhtTimer, (os_timer_func_t *)DhtTimerCbk, (void *)0);
	os_timer_arm(&DhtTimer, 20, 1);
	DEBUG("Dht Starting measure\n\r");
}

void ICACHE_FLASH_ATTR DhtGpioCbkSetup()
{
    //ETS_GPIO_INTR_DISABLE();
    GPIO_DIS_OUTPUT(DHT_PIN);
    //ETS_GPIO_INTR_ATTACH(&DhtGpioCbk, NULL);
    ETS_GPIO_INTR_ATTACH(&GpioUniCbk, NULL);
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
	os_memset(result, 0 , sizeof(result));
	uint8_t bytes;
	uint8_t bits, offset;
	int8_t temp;
	bits = 2;
	for(bytes = 0; bytes < 5 ; bytes++)
	{
		offset = bits;
		for(temp = 7; temp >= 0; temp--)
		{
			if(DhtResult[bits] > 100)
				result[bytes] |= 0x1 << (temp);
			bits++;
		}
	}
	DEBUG("Result: %d, %d, %d ,%d, %d\n\r",result[0], result[1], result[2], result[3], result[4]);
	if((result[0] + result[1] + result[2] + result[3]) == result[4])
	{
		DEBUG("parity_ok!!\n\r");
	}
	else
	{
		DEBUG("parity_NOK!!\n\r");
		DhtHandle.state = ERR;
		system_os_post(0,DHT_DRIVER_SOURCE,(os_param_t)&DhtHandle);
	}
#if DHT_TYPE == DHT11
	DhtHandle.DhtHum = result[0];
	DhtHandle.DhtTemp = result[2];
#else
	uint16_t rawhumidity = result[0]<<8 | result[1];
	uint16_t rawtemperature = result[2]<<8 | result[3];
	if(rawtemperature & 0x8000)
	{
		DhtHandle.DhtTemp = (float)((rawtemperature & 0x7FFF) / 10.0) * -1.0;
	}
	else
	{
		DhtHandle.DhtTemp = (float)(rawtemperature)/10.0;
	}
	DhtHandle.DhtHum = (float)(rawhumidity)/10.0;
#endif
}

void DhtGpioCbk(void *arg)
{
	uint32 gpio_status;
	gpio_status = GPIO_REG_READ(GPIO_STATUS_ADDRESS);
	DhtHandle.state = MEAS;
	//clear
	if (gpio_status & BIT(DHT_PIN))
	{
		DhtResult[DhtResultIdx++] = WDEW_NOW() - DhtTimeOffset;
		DhtTimeOffset = WDEW_NOW();
		if(DhtResultIdx > 41)
		{
			//ETS_GPIO_INTR_DISABLE();
			uint8_t temp5 = 0;
			/*
			for (temp5 = 0; temp5 < 42; temp5++)
			{
				DEBUG("|Dht: %d, \n\r", DhtResult[temp5]);
			}
			*/
			DhtProcess();
			gpio_pin_intr_state_set(GPIO_ID_PIN(DHT_PIN), GPIO_PIN_INTR_DISABLE);
			GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, gpio_status & BIT(DHT_PIN));
			os_timer_disarm(&DhtTimer);
			DhtHandle.state = COMPL;
			system_os_post(0,DHT_DRIVER_SOURCE,(os_param_t)&DhtHandle);
		}
		GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, gpio_status & BIT(DHT_PIN));
	}

}

void DhtTimerCbk()
{
	//DEBUG("DhtTimerCbk, %d\n\r", DhtTimeout);
	GPIO_OUTPUT_SET(DHT_PIN, 1);
	GPIO_DIS_OUTPUT(DHT_PIN);
	DhtTimeout++;
	if(DHT_TIMEOUT < DhtTimeout)
	{
		DhtHandle.state = ERR;
		DEBUG("DhTTimeout!!\n\r");
		os_timer_disarm(&DhtTimer);
		system_os_post(0,DHT_DRIVER_SOURCE,(os_param_t)&DhtHandle);
	}
	DhtGpioCbkSetup();

}


ICACHE_FLASH_ATTR void DhtTestSeq()
{
	DhtInit();
	DhtReadStart();
}

