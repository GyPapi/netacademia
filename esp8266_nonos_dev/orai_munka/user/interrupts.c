/*
 * interrupts.c
 *
 *  Created on: Feb 12, 2018
 *      Author: nolex
 */



#include "osapi.h"
#include "os_type.h"
#include "user_interface.h"
#include "ets_sys.h"
#include "gpio.h"
#include "user_interface.h"
#include "c_types.h"

#include "dht.h"
#include "hcsr04.h"

void GpioCbk()
{
	dhtGpioCbk();
	ultraSCbkGpio();
}
