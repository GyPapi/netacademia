/*
 * daemon-handle.c
 *
 *  Created on: Feb 14, 2018
 *      Author: nolex
 */

#include "daemon.h"

#define DEBUG_EN 1
#if DEBUG_EN == 1
#define DEBUG(format, ...) os_printf(format, ## __VA_ARGS__)
#else
#define DEBUG(format,...)
#endif


static struct espconn daemonHandler;

void daemonConnCbk(void *arg)
{
	DEBUG("daemon Connect cbk\n\r");
}

void daemonConRecvCbk(void *arg, char *pdata, unsigned short len)
{
	DEBUG("daemon Connect receive cbk\n\r");
}

void daemonReconnCbk(void *arg, sint8 err)
{
	DEBUG("daemon ReConnect cbk\n\r");
}

void daemonDiscCbk(void *arg)
{
	DEBUG("daemon DisConnect cbk\n\r");
}

void daemonWriteFinishCbk(void *arg)
{
	DEBUG("daemon writefinish cbk\n\r");
}

ICACHE_FLASH_ATTR void daemonConInit()
{
	int8_t status = 0;
	daemonHandler.proto.tcp = (esp_tcp *)os_zalloc(sizeof(esp_tcp));
	daemonHandler.proto.tcp->remote_port = 13001;
	daemonHandler.proto.tcp->local_port = 13001;
	daemonHandler.proto.tcp->remote_ip[0] = 192;
	daemonHandler.proto.tcp->remote_ip[1] = 168;
	daemonHandler.proto.tcp->remote_ip[2] = 1;
	daemonHandler.proto.tcp->remote_ip[3] = 235;
	daemonHandler.type = ESPCONN_TCP;
	status = espconn_regist_recvcb(&daemonHandler, daemonConRecvCbk);
	if(status != 0)
		DEBUG("EspConn Create ConnCbk Fail, code %d\n\r");
	status = espconn_regist_reconcb(&daemonHandler, daemonReconnCbk);
	if(status != 0)
		DEBUG("EspConn Create ReconnCbk Fail, code %d\n\r");

	status = espconn_regist_disconcb(&daemonHandler, daemonDiscCbk);
	if(status != 0)
			DEBUG("EspConn Create DiscCbk Fail, code %d\n\r");

	status = espconn_regist_write_finish(&daemonHandler, daemonWriteFinishCbk);
	if(status != 0)
			DEBUG("EspConn Create FinishCbk Fail, code %d\n\r");

	status = espconn_create(&daemonHandler);
	if(status != 0)
		DEBUG("EspConn Create Udp Heartbeat Fail, code %d\n\r");

	status = espconn_sendto(&daemonHandler, "Helloworld", 10);
	DEBUG("TcpDaemonCallbackInitDone\n\r");
}
