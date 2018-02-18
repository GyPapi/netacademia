/*
 * heartbeat.h
 *
 *  Created on: Feb 14, 2018
 *      Author: nolex
 */

#ifndef ORAI_MUNKA_INCLUDE_HEARTBEAT_H_
#define ORAI_MUNKA_INCLUDE_HEARTBEAT_H_


#include "ets_sys.h"
#include "os_type.h"
#include "osapi.h"
#include "mem.h"
#include "gpio.h"
#include "user_interface.h"
#include "c_types.h"
#include "espconn.h"
#include "ip_addr.h"
#define HEARTB_DELAY_TIME		1000
#define HEARTB_FIRSTRUN_DELAY	6000
#define DAEMON_REMOTE_PORT		13000


extern void initUdpHeartBeat();



#endif /* ORAI_MUNKA_INCLUDE_HEARTBEAT_H_ */
