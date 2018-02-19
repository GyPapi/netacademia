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

static uint8_t daemonSts;
static struct espconn daemonHandler;
ICACHE_FLASH_ATTR void daemonHandleAnswer(char *pdata, unsigned short len);
ICACHE_FLASH_ATTR void daemonRequest(uint8_t* data);

void daemonConnCbk(void *arg)
{
	DEBUG("daemon Connect cbk\n\r");
	daemonRequest(NULL);
}

void daemonConRecvCbk(void *arg, char *pdata, unsigned short len)
{
	DEBUG("daemon Connect receive cbk\n\r");
	daemonHandleAnswer(pdata,len);
}

void daemonReconnCbk(void *arg, sint8 err)
{
	DEBUG("daemon ReConnect cbk\n\r");
	daemonRequest(NULL);
}

void daemonDiscCbk(void *arg)
{
	DEBUG("daemon DisConnect cbk\n\r");
}

void daemonWriteFinishCbk(void *arg)
{
	DEBUG("daemon writefinish cbk\n\r");
}

ICACHE_FLASH_ATTR void socketStarter()
{
	DEBUG("Initializing sockets...\n\r");
	daemonConInit();
	initUdpHeartBeat();
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

	status = espconn_connect(&daemonHandler);
	if(status != 0)
		DEBUG("EspConn Create Udp Heartbeat Fail, code %d\n\r");

	DEBUG("TcpDaemonCallbackInitDone\n\r");
	daemonSts = 0;
}

ICACHE_FLASH_ATTR void daemonHandleAnswer(char *pdata, unsigned short len)
{
	uint8_t buf[64];
	os_memcpy(buf, pdata,len);
	if(os_strstr(buf, "DHT_GET") != NULL )
	{
		DEBUG("GOT DhT\n\r");
		daemonSts++;
		dhtStart();
	}
	else if(os_strstr(buf, "OK") != NULL )
	{
		DEBUG("GOT DhT ACK\n\r");
		daemonSts = 0;
	}
	else
	{
		DEBUG("Unhandled answer\n\r");
	}
}

ICACHE_FLASH_ATTR void daemonRequest(uint8* data)
{
	if(daemonSts == 0)
	{
		uint8_t status = espconn_send(&daemonHandler, "DAEMON_ASK", 10);
		if(status != 0)
			DEBUG("Espconn send tcp fail\n\r");
	}
	if(daemonSts == 1)
	{
		espconn_connect(&daemonHandler);
		uint8_t status = espconn_send(&daemonHandler, data, sizeof(data));
		if(status != 0)
			DEBUG("Espconn send tcp fail\n\r");
		DEBUG("Espconn dht tcp ok data: %s\n\r", data);
		daemonSts++;
	}
}

