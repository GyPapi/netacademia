/*
 * hcsr04-drv.c
 *
 *  Created on: Jan 23, 2018
 *      Author: nolex
 */

#include "hcsr04-drv.h"


#define HC_INPUT	4
#define HC_OUTPUT   13

LOCAL uint8_t dbg;

#define ULTRASENS_TASK_PRIO 		2
#define ULTRASENS_TASK_Q_SIZE    1
static os_event_t ultrasensTaskQ[ULTRASENS_TASK_Q_SIZE];

void ultrasensorInit();
void ultrasensorStartMeasure();
FRC1_TIMER_SOURCE_TYPE frctype = FRC1_SOURCE;
LOCAL void hcsr04GpioIntCbk();
uint32_t TimerOffset;
static uint32_t UltraSensorResult;
static uint8_t UltraSensSts;

/*
void TimerMeasureCbkDummy()
{

	//os_printf("!!!XXXX!\n\r");
}



void ICACHE_FLASH_ATTR TimerMeasureSetup()
{

	//hw_timer_init(frctype,0);
	//hw_timer_set_func(TimerMeasureCbkDummy);
	//TimerOffset = WDEW_NOW();
	//hw_timer_arm(10000);

	//TimerOffset = system_get_rtc_time();
}

*/

void ICACHE_FLASH_ATTR ultrasensorInit()
{
    ETS_GPIO_INTR_DISABLE();
    GPIO_OUTPUT_SET(HC_OUTPUT, HC_INPUT);
    GPIO_DIS_OUTPUT(HC_INPUT);
    PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO4_U,FUNC_GPIO4);
    ETS_GPIO_INTR_ATTACH(hcsr04GpioIntCbk, NULL);
    gpio_register_set(GPIO_PIN_ADDR(HC_INPUT), GPIO_PIN_INT_TYPE_SET(GPIO_PIN_INTR_DISABLE)
                      | GPIO_PIN_PAD_DRIVER_SET(GPIO_PAD_DRIVER_DISABLE)
                      | GPIO_PIN_SOURCE_SET(GPIO_AS_PIN_SOURCE));

    GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, BIT(HC_INPUT));
    gpio_pin_intr_state_set(GPIO_ID_PIN(HC_INPUT), GPIO_PIN_INTR_POSEDGE);
    ETS_GPIO_INTR_ENABLE();
    dbg = 0;
    UltraSensSts = 0;
}

void ICACHE_FLASH_ATTR ultrasensorStartMeasure()
{
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTCK_U,FUNC_GPIO13);
	GPIO_OUTPUT_SET(HC_OUTPUT, 0);
	os_delay_us(100);
	GPIO_OUTPUT_SET(HC_OUTPUT, 1);
	os_delay_us(10);
	GPIO_OUTPUT_SET(HC_OUTPUT, 0);
	TimerMeasureSetup();
}

void hcsr04GpioIntCbk(void *arg)
{
	//os_delay_us(30);
	//UltraSensorResult = GET_TIME_MEASURE_RES();
	uint32 gpio_status;
	gpio_status = GPIO_REG_READ(GPIO_STATUS_ADDRESS);
	//clear
	if (gpio_status & BIT(HC_INPUT))
	{
		if(0 == UltraSensSts)
		{
			//TimerOffset = system_get_rtc_time();
			TimerOffset =  WDEW_NOW() ;
			gpio_pin_intr_state_set(GPIO_ID_PIN(HC_INPUT), GPIO_PIN_INTR_NEGEDGE);
			GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, gpio_status & BIT(HC_INPUT));
			UltraSensSts++;
		}
		else if(1 == UltraSensSts)
		{
			//UltraSensorResult = system_get_rtc_time() - TimerOffset;
			UltraSensorResult = GET_TIME_MEASURE_RES();
			gpio_pin_intr_state_set(GPIO_ID_PIN(HC_INPUT), GPIO_PIN_INTR_DISABLE);
			GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, gpio_status & BIT(HC_INPUT));
			ETS_GPIO_INTR_DISABLE();
			dbg++;
			//hw_timer_init(frctype,0);
			system_os_post(ULTRASENS_TASK_PRIO,0,0);
		}
	}

}
/*
 * TestBench
void ICACHE_FLASH_ATTR TestPrint(os_event_t *q)
{
	os_printf("TestPrint\n\r");
	os_printf("UltraSens result: %d, offset: %x \n\r",UltraSensorResult, TimerOffset);
}

void ICACHE_FLASH_ATTR TestBenchUltraSound()
{
	os_printf("testBenchUltraSound\n\r");
	ultrasensorInit();
	ultrasensorStartMeasure();
	system_os_task(TestPrint,ULTRASENS_TASK_PRIO, ultrasensTaskQ, ULTRASENS_TASK_Q_SIZE);
}
*/
