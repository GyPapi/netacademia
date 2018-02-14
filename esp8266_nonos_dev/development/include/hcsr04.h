/*
 * hcsr04-drv.h
 *
 *  Created on: Jan 24, 2018
 *      Author: nolex
 */

//module dependent includes


#ifndef DEVELOPMENT_INCLUDE_HCSR04_DRV_H_
#define DEVELOPMENT_INCLUDE_HCSR04_DRV_H_

#include "ets_sys.h"
#include "os_type.h"
#include "osapi.h"
#include "mem.h"
#include "gpio.h"
#include "user_interface.h"
#include "c_types.h"

#include "uni-sensor.h"

//configurations
#define HC_INPUT	4
#define HC_OUTPUT   13


typedef struct{
	SensorSts_t state;
	float		result;
}Hcsr04Handler_t;

#define REG_READ(_r) 	(*(volatile uint32_t *)(_r))
//#define WDEW_NOW()		RTC_REG_READ(FRC2_COUNT_ADDRESS)
#define WDEW_NOW()				REG_READ(0x3ff20c00)
#define GET_TIME_MEASURE_RES()	 WDEW_NOW() - TimerOffset

typedef enum {
    FRC1_SOURCE = 0,
    NMI_SOURCE = 1,
} FRC1_TIMER_SOURCE_TYPE;

#define DEBUG_EN 1
#if DEBUG_EN == 1
#define DEBUG( format, ... ) os_printf( format, ## __VA_ARGS__ )
#else
#define DEBUG( format, ... )
#endif



#endif /* DEVELOPMENT_INCLUDE_HCSR04_DRV_H_ */
