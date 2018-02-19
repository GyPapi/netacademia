/*
 * utils-heartbeat.c
 *
 *  Created on: Feb 12, 2018
 *      Author: nolex
 */


#include "ets_sys.h"
#include "os_type.h"
#include "osapi.h"
#include "mem.h"
#include "gpio.h"
#include "user_interface.h"
#include "c_types.h"
#include "espconn.h"
#include "ip_addr.h"

static struct espconn HeartBeatConn;
static os_timer_t heartbeatTimer;


#define DEBUG_EN 0
#if DEBUG_EN == 1
#define DEBUG(format, ...) os_printf(format, ## __VA_ARGS__)
#else
#define DEBUG(format,...)
#endif

void HeartBeatConnCbk(void *arg, char *pusrdata, unsigned short length)
{
	remot_info* pcon_info = NULL;

	espconn_get_connection_info(&HeartBeatConn, &pcon_info, 0);
	DEBUG("remote ip: %d.%d.%d.%d \r\n",pcon_info->remote_ip[0],pcon_info->remote_ip[1],
		                                    pcon_info->remote_ip[2],pcon_info->remote_ip[3]);
	DEBUG("remote port: %d \r\n",pcon_info->remote_port);
}

void HeartBeatReconCbk(void *arg, sint8 err)
{
	DEBUG("reconnection\n\r");
}

void udpHeartBeat()
{
	int8_t status = 0;
	status = espconn_create(&HeartBeatConn);
	char *heartbeat = "Helloworld";
	if(status != 0)
		DEBUG("heartbeat sending fail, code: %d\n\r", status);
	status = espconn_sendto(&HeartBeatConn, heartbeat, 10);
	if(status != 0)
		DEBUG("heartbeat sending fail, code: %d\n\r", status);
}

ICACHE_FLASH_ATTR void initUdpHeartbeat()
{
	int8_t status = 0;
	HeartBeatConn.recv_callback = HeartBeatConnCbk;
	HeartBeatConn.proto.udp = (esp_udp *)os_zalloc(sizeof(esp_udp));
	HeartBeatConn.proto.udp->remote_port = 13000;
	HeartBeatConn.proto.udp->local_port = 13000;
	HeartBeatConn.proto.udp->remote_ip[0] = 192;
	HeartBeatConn.proto.udp->remote_ip[1] = 168;
	HeartBeatConn.proto.udp->remote_ip[2] = 1;
	HeartBeatConn.proto.udp->remote_ip[3] = 235;
	HeartBeatConn.type = ESPCONN_UDP;
    espconn_regist_recvcb(&HeartBeatConn, HeartBeatConnCbk);
    espconn_regist_reconcb(&HeartBeatConn, HeartBeatReconCbk);
    //status = espconn_create(&HeartBeatConn);

    //espconn_connect(&HeartBeatConn);
	os_timer_setfn(&heartbeatTimer, (os_timer_func_t *)udpHeartBeat, (void *)0);
	os_timer_arm(&heartbeatTimer, 1000, 1);
	DEBUG("UdpHeartBeatDone!\n\r");
}

ICACHE_FLASH_ATTR void initSockets()
{
	initUdpHeartbeat();
	daemonConInit();
}


