/*
 * time_measure.c
 *
 *  Created on: Jan 30, 2018
 *      Author: nolex
 *      Noticements:
 *       Didn't find proper information about how to implement correct timers.
 *       So I used the NONOS-SDK pdf paper's example at page
 */

#include "osapi.h"
#include "user_interface.h"
#define REG_READ(_r) 	(*(volatile uint32_t *)(_r))
//#define WDEW_NOW()		RTC_REG_READ(FRC2_COUNT_ADDRESS)
#define WDEW_NOW()		REG_READ(0x3ff20c00)

uint32_t tick_now2 = 0;
typedef enum {
    FRC1_SOURCE = 0,
    NMI_SOURCE = 1,
} FRC1_TIMER_SOURCE_TYPE;
static uint32_t offset;


FRC1_TIMER_SOURCE_TYPE frctypeOwn = FRC1_SOURCE;

void hw_test_timer_cb()
{
	/*static uint16_t j = 0;
	j++;
	if(( WDEW_NOW() - tick_now2) >= 1000000)
	{
		static uint32_t idx = 1;
		tick_now2 = WDEW_NOW();
		//os_printf("b %u:%d\n", idx++,j);
		//os_printf("Wdew: %x \n\r", WDEW_NOW());
		j = 0;
	}
	//os_printf("Wdew: post  %x\n\r", WDEW_NOW());
	 *
	 */
}



//TEST entry point
void ICACHE_FLASH_ATTR hw_timer_test_init()
{
	//setting the timer, must give a dummy function!
	hw_timer_init(frctypeOwn,0);
	hw_timer_set_func(hw_test_timer_cb);
	///os_printf("Wdew pre: %x\n\r", WDEW_NOW());

	//lets get an offset
	offset = WDEW_NOW();
	hw_timer_arm(10000);
	//proper delay
	os_delay_us(30);
	//get the time
	uint32_t temp = WDEW_NOW() - offset;
	//print out values, at 10.000uS delay provides real 10005 delay. Enough.
	os_printf("time passed: %x", temp);
}
