/*
 * dht.h
 *
 *  Created on: Feb 7, 2018
 *      Author: nolex
 */

#ifndef ORAI_MUNKA_INCLUDE_DHT_H_
#define ORAI_MUNKA_INCLUDE_DHT_H_

#include "osapi.h"
#include "os_type.h"
#include "user_interface.h"
#include "ets_sys.h"
#include "gpio.h"
#include "user_interface.h"
#include "c_types.h"


#define DHT_PIN		14

#define DHT11		1
#define DHT22		2

#define DHT_TYPE	DHT11

#define DHT_PIN_ON()	GPIO_OUTPUT_SET(DHT_PIN, 1)
#define DHT_PIN_OFF()	GPIO_OUTPUT_SET(DHT_PIN, 0)

#define DHT_PIN_DIS()	GPIO_DIS_OUTPUT(DHT_PIN)

#define DHT_READ_PIN()	GPIO_INPUT_GET(DHT_PIN)

#define READ_REG(_r)	(*(volatile uint32_t *)(_r))
#define WDEW_NOW()		READ_REG(0x3ff20c00)




#endif /* ORAI_MUNKA_INCLUDE_DHT_H_ */
