/*
 * hcsr04.c
 *
 *  Created on: Feb 5, 2018
 *      Author: nolex
 */

#include "hcsr04.h"
#include "stdio.h"

static uint8_t UltraSensSts;
static uint32_t TimerOffset;
static uint32_t Hcsr04ResultRaw;

#define HC_TASK_PRIO		1
#define HC_TASK_Q_SIZE	2
static os_event_t hcTaskQ[HC_TASK_Q_SIZE];

//extern os_event_t mainTaskQ[2];

void ultraSCbkGpio();

void ICACHE_FLASH_ATTR ultraSInit()
{
	ETS_GPIO_INTR_DISABLE();
	GPIO_DIS_OUTPUT(HC_INPUT);
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO4_U, FUNC_GPIO4);
	ETS_GPIO_INTR_ATTACH(ultraSCbkGpio, NULL);
	gpio_register_set(GPIO_PIN_ADDR(HC_INPUT), GPIO_PIN_INT_TYPE_SET(GPIO_PIN_INTR_DISABLE)
			| GPIO_PIN_PAD_DRIVER_SET(GPIO_PAD_DRIVER_DISABLE)
			| GPIO_PIN_SOURCE_SET(GPIO_AS_PIN_SOURCE));
	GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, BIT(HC_INPUT));
	gpio_pin_intr_state_set(GPIO_ID_PIN(HC_INPUT), GPIO_PIN_INTR_POSEDGE);
	ETS_GPIO_INTR_ENABLE();
	UltraSensSts = 0;
}

void ultraSStart()
{
	ultraSInit();
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTCK_U, FUNC_GPIO13);
	GPIO_OUTPUT_SET(HC_OUTPUT,0);
	os_delay_us(200);
	GPIO_OUTPUT_SET(HC_OUTPUT,1);
	os_delay_us(10);
	GPIO_OUTPUT_SET(HC_OUTPUT,0);
}


void ultraSCbkGpio()
{
	uint32_t gpioSts = GPIO_REG_READ(GPIO_STATUS_ADDRESS);
	if(gpioSts & BIT(HC_INPUT)) // gpio_status & 2 << HC_INPUT->(13)
	{
		if(0 == UltraSensSts)
		{
			TimerOffset  = WDEW_NOW();
			gpio_pin_intr_state_set(GPIO_ID_PIN(HC_INPUT), GPIO_PIN_INTR_NEGEDGE);
			GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, gpioSts & BIT(HC_INPUT));
			UltraSensSts++;
		}
		else if (1 == UltraSensSts)
		{
			Hcsr04ResultRaw = WDEW_NOW() - TimerOffset;
			gpio_pin_intr_state_set(GPIO_ID_PIN(HC_INPUT), GPIO_PIN_INTR_DISABLE);
			GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, gpioSts & BIT(HC_INPUT));
			UltraSensSts++;
			system_os_post(1, 0, 0);
		}
	}
}

float ultraSCalcDist()
{
	return (float)(Hcsr04ResultRaw * 0.034)/ 2;
}

void ultraSTask(os_event_t *ev)
{
	os_printf("Result of HCSR04 Raw measrement %d\n\r", Hcsr04ResultRaw);
	float Distance = ultraSCalcDist();
	uint8_t buf[16];
	//sprintf(buf, "%.2f", Distance);
	//os_printf("Result of HCSR04: %s", buf);
}

void testUltraS()
{
	system_os_task(ultraSTask, 1, hcTaskQ, HC_TASK_Q_SIZE);
	ultraSStart();
}
