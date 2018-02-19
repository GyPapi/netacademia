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
static uint8_t daemonSts;
ICACHE_FLASH_ATTR void daemonRequest();
ICACHE_FLASH_ATTR void daemonHandleAnswer(char *pdata, unsigned short len);

void daemonConnCbk(void *arg)
{
	DEBUG("daemon Connect cbk\n\r");
	daemonRequest();

}

void daemonConRecvCbk(void *arg, char *pdata, unsigned short len)
{
	DEBUG("daemon receive cbk\n\r");
	daemonHandleAnswer(pdata,len);
}

void daemonReconnCbk(void *arg, sint8 err)
{
	DEBUG("daemon ReConnect cbk\n\r");
	uint8_t status = espconn_send(&daemonHandler, "TCPHELLO", 8);
	if(status != 0)
		DEBUG("EspConn send TCP Fail, code %d\n\r");
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
	status = espconn_regist_connectcb(&daemonHandler, daemonConnCbk);
	if(status != 0)
		DEBUG("EspConn Create ConnCbk Fail, code %d\n\r");
	status = espconn_regist_recvcb(&daemonHandler, daemonConRecvCbk);
	if(status != 0)
		DEBUG("EspConn Create recvCbk Fail, code %d\n\r");
	status = espconn_regist_reconcb(&daemonHandler, daemonReconnCbk);
	if(status != 0)
		DEBUG("EspConn Create ReconnCbk Fail, code %d\n\r");

	status = espconn_regist_disconcb(&daemonHandler, daemonDiscCbk);
	if(status != 0)
			DEBUG("EspConn Create DiscCbk Fail, code %d\n\r");

	status = espconn_regist_write_finish(&daemonHandler, daemonWriteFinishCbk);
	if(status != 0)
			DEBUG("EspConn Create FinishCbk Fail, code %d\n\r");

	status = espconn_connect(&daemonHandler);
	if(status != 0)
		DEBUG("EspConn Create TCP Fail, code %d\n\r");
	daemonSts = 0;
	DEBUG("TcpDaemonCallbackInitDone\n\r");
}

ICACHE_FLASH_ATTR void daemonHandleAnswer(char *pdata, unsigned short len)
{
	uint8_t buf[64];
	os_memcpy(buf, pdata, len);
	if(os_strstr(buf, "DHT_GET") != NULL)
	{
		DEBUG("Got DhT\n\r");
	}
}

ICACHE_FLASH_ATTR void daemonRequest()
{
	if(daemonSts == 0)
	{
		uint8_t status = espconn_send(&daemonHandler, "DAEMON_ASK", 10);
		if(status != 0)
			DEBUG("EspConn send TCP Fail, code %d\n\r");
	}
	else if(daemonSts == 1)
	{

	}
}
