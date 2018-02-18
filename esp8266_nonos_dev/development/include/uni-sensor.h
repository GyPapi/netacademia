/*
 * uni-sensor.h
 *
 *  Created on: Feb 13, 2018
 *      Author: nolex
 */

#ifndef DEVELOPMENT_INCLUDE_UNI_SENSOR_H_
#define DEVELOPMENT_INCLUDE_UNI_SENSOR_H_

#include "osapi.h"
typedef enum{
	IDLE,
	MEAS,
	PREP,
	COMPL,
	ERR,
}SensorSts_t;

#define DHT_DRIVER_SOURCE 1
#define HCSR04_DRIVER_SOURCE 2

void GpioUniCbk(void *arg);

extern void hcsr04GpioIntCbk(void *arg);
extern void DhtGpioCbk(void *arg);
#endif /* DEVELOPMENT_INCLUDE_UNI_SENSOR_H_ */




