/*
 * heartbeat.c
 *
 *  Created on: Feb 14, 2018
 *      Author: nolex
 */

#include "heartbeat.h"

#define DEBUG_EN 0
#if DEBUG_EN == 1
#define DEBUG(format, ...) os_printf(format, ## __VA_ARGS__)
#else
#define DEBUG(format,...)
#endif

static struct espconn heartBeatUdpHandler;
static os_timer_t heartBeatTimer;


void heartBeatConCbk (void *arg, char *pdata, unsigned short len)
{
	uint8_t buf[64];
	remot_info* pcon_info = NULL;
	espconn_get_connection_info(&heartBeatUdpHandler, &pcon_info, 0);
	os_printf("Remote IP: %d,%d,%d,%d\r\n", pcon_info->remote_ip[0],pcon_info->remote_ip[1],
			pcon_info->remote_ip[2],pcon_info->remote_ip[3]);
	os_printf("Remote port: %d\n\rData:\n\r", pcon_info->remote_port);
	os_memcpy(buf, pdata, len);
	os_printf("%s\n\r", buf);
}

void heartBeatDcCbk(void *arg, sint8 err)
{
	os_printf("Disconnect initiated\n\r");
}

void UdpHeartBeatTimerCbk()
{
	int8_t status = 0;
	status = espconn_sendto(&heartBeatUdpHandler, "Helloworld", 10);
}

ICACHE_FLASH_ATTR void initUdpHeartBeat()
{
	int8_t status = 0;
	heartBeatUdpHandler.proto.udp = (esp_udp *)os_zalloc(sizeof(esp_udp));
	heartBeatUdpHandler.proto.udp->remote_port = DAEMON_REMOTE_PORT;
	heartBeatUdpHandler.proto.udp->local_port = 13000;
	heartBeatUdpHandler.proto.udp->remote_ip[0] = 192;
	heartBeatUdpHandler.proto.udp->remote_ip[1] = 168;
	heartBeatUdpHandler.proto.udp->remote_ip[2] = 1;
	heartBeatUdpHandler.proto.udp->remote_ip[3] = 235;
	heartBeatUdpHandler.type = ESPCONN_UDP;
	heartBeatUdpHandler.recv_callback = heartBeatConCbk;
	//status = espconn_regist_recvcb(&heartBeatUdpHandler, heartBeatConCbk);
	if(status != 0)
		DEBUG("EspConn Create Udp Heartbeat Fail, code %d\n\r");
	//status = espconn_regist_reconcb(&heartBeatUdpHandler, heartBeatDcCbk);
	if(status != 0)
		DEBUG("EspConn Create Udp Heartbeat Fail, code %d\n\r");


	status = espconn_create(&heartBeatUdpHandler);
	if(status != 0)
		DEBUG("EspConn Create Udp Heartbeat Fail, code %d\n\r");

	os_timer_setfn(&heartBeatTimer,  (os_timer_func_t*)UdpHeartBeatTimerCbk, (void*)0);
	os_timer_arm(&heartBeatTimer,HEARTB_DELAY_TIME,1);
	DEBUG("UdpCallbackInitDone\n\r");
}


