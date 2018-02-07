/*
 * hcsr04.h
 *
 *  Created on: Feb 5, 2018
 *      Author: nolex
 */

#ifndef ORAI_MUNKA_INCLUDE_HCSR04_H_
#define ORAI_MUNKA_INCLUDE_HCSR04_H_

#include "osapi.h"
#include "os_type.h"
#include "user_interface.h"
#include "ets_sys.h"
#include "gpio.h"
#include "user_interface.h"
#include "c_types.h"

#define HC_INPUT		4
#define HC_OUTPUT		13

#define READ_REG(_r)	(*(volatile uint32_t *)(_r))
#define WDEW_NOW()		READ_REG(0x3ff20c00)


#endif /* ORAI_MUNKA_INCLUDE_HCSR04_H_ */
