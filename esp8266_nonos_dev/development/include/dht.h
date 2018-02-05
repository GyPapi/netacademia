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


#define DHT11		1
#define DHT22		2
#define DHT_TYPE	DHT11
#define DHT_PIN		14

#define DHT_PIN_ON()		GPIO_OUTPUT_SET(DHT_PIN,1)
#define DHT_PIN_OFF()		GPIO_OUTPUT_SET(DHT_PIN,0)

#define DHT_READ_PIN()		GPIO_INPUT_GET(DHT_PIN)

#endif /* DEVELOPMENT_INCLUDE_DHT_H_ */
