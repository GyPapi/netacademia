/*
 * dht.h
 *
 *  Created on: Feb 4, 2018
 *      Author: nolex
 */

#ifndef DEVELOPMENT_INCLUDE_DHT_H_
#define DEVELOPMENT_INCLUDE_DHT_H_


#include "ets_sys.h"
#include "os_type.h"
#include "osapi.h"
#include "mem.h"
#include "gpio.h"
#include "user_interface.h"
#include "c_types.h"

#include "uni-sensor.h"

#define DHT11		1
#define DHT22		2
#define DHT_TYPE	DHT22
#define DHT_PIN		14

#define DHT_TIMEOUT 10


typedef struct
{
	SensorSts_t state;
#if DHT_TYPE == DHT11
	int16_t DhtTemp;
	int16_t DhtHum;
#else
	float DhtTemp;
	float DhtHum;
#endif
} DhtHandler;

//#define DHT_PIN_ON()		GPIO_OUTPUT_SET(DHT_PIN,1)
//#define DHT_PIN_OFF()		GPIO_OUTPUT_SET(DHT_PIN,0)

#define DHT_PIN_ON()		PIN_PULLUP_EN(PERIPHS_IO_MUX_MTMS_U)
#define DHT_PIN_OFF()		PIN_PULLUP_DIS(PERIPHS_IO_MUX_MTMS_U)

#define REG_READ(_r) 	(*(volatile uint32_t *)(_r))
//#define WDEW_NOW()		RTC_REG_READ(FRC2_COUNT_ADDRESS)
#define WDEW_NOW()				REG_READ(0x3ff20c00)
#define GET_TIME_MEASURE_RES()	 WDEW_NOW() - TimerOffset

#define DHT_READ_PIN()		GPIO_INPUT_GET(DHT_PIN)

#define DEBUG_EN 1
#if DEBUG_EN == 1
#define DEBUG( format, ... ) os_printf( format, ## __VA_ARGS__ )
#else
#define DEBUG( format, ... )
#endif

#endif /* DEVELOPMENT_INCLUDE_DHT_H_ */
