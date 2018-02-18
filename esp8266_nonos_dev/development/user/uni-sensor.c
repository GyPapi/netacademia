/*
 * uni-sensor.c
 *
 *  Created on: Feb 13, 2018
 *      Author: nolex
 */

#include "uni-sensor.h"

static os_timer_t waitingTimer;

void registerWaitGpio(void)
{

}

void GpioUniCbk(void * arg)
{
	hcsr04GpioIntCbk(arg);
}
