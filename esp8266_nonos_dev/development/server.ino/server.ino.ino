//INCLUDES--------------------------------
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <IPAddress.h>
#include "osapi.h"
#include "os_type.h"
#include "ets_sys.h"
#include "c_types.h"

#include "DHT.h"
//defines
//#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//Static variables------------------------

static os_timer_t heartBeatTimer;
const int triggerPin = 13;
const int echoPin = 4;
static uint8_t ledState;
static uint8_t ClientState;

const char* ssid     = "navtah10";
const char* password = "qwas1234";
const char* heartBeatwords = "HEARTBEAT";
const char* host = "192.168.1.235";
const int DHTPin = 14;
DHT dht(DHTPin, DHTTYPE);
WiFiClient client;
static WiFiUDP Udp;
//constants------------------------------
const int ledPin = 12;

void setupHC()
{
  pinMode(triggerPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  digitalWrite(triggerPin, LOW);
}

void heartBeat()
{
  Serial.println("Esp beating...");
  if(ledState++ % 2)
  {
    digitalWrite(ledPin, HIGH);
  }
  else
  {
    digitalWrite(ledPin, LOW);
  }
  Udp.beginPacket("192.168.1.235", 13000);
  Udp.write(heartBeatwords);
  Udp.endPacket();
  ClientState = 0;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("ESP8266 client started");
  ledState = 0;
  pinMode(ledPin, OUTPUT);
  os_timer_setfn(&heartBeatTimer, (os_timer_func_t*)heartBeat, (void*)0);
  os_timer_arm(&heartBeatTimer, 2000, 1);
  DHT dht(DHTPin, DHTTYPE);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  //unsigned int localUdpPort = 13000;
  //char incomingPacket[255];
  //Udp.begin(localUdpPort);
  
}



void loop() {
  dht.begin();
  if (client.connect(host, 13001))
  {
    Serial.println("Connected!");
   
  }
  else
  {
    Serial.println("error in connection");
  }
  Serial.println(ClientState);
  if( ClientState == 0)
  {
    Serial.println("Sending ASK!");
    client.print("DAEMON_ASK");
     while (client.connected())
     {
      if(client.available()> 0)
      {
        String data = client.readStringUntil('\n');
        Serial.println("DATA AVAILABLE!");
        Serial.println(data);
        if(data.equals("DHT_GET"))
        {
          Serial.println("DHT Measure Requested!");
          ClientState = 2;
          client.stop();
        }
      }
     }
  }
  else if (ClientState  == 2)
  {
    Serial.println("DHT Measure Starts!");
    String hum = String(dht.readHumidity());
    String temp = String(dht.readTemperature());
    Serial.println(String("TEMP:" + temp + String(" ") + String("HUM:") + hum));
    client.print(String("TEMP:" + temp + String(" ") + String("HUM:") + hum));
     while (client.connected())
     {
      if(client.available()> 0)
      {
        Serial.println("SENT!");
        String data = client.readStringUntil('\n');
        if(data.equals("OK"))
        {
          Serial.println("DHT Measure Sent successfully!");
          ClientState = 5;
        }
      }
     }
  }
  else if (ClientState  == 3)
  {
    
    //client.print(String("TEMP:" + temp + String(" ") + String("HUM:") + hum));
     while (client.connected())
     {
      if(client.available()> 0)
      {
        String data = client.readStringUntil('\n');
        if(data.equals("OK"))
        {
          Serial.println("DHT Measure Sent successfully!");
          ClientState = 5;
          
        }
      }
     }
  }
   else if (ClientState  == 5)
  {
    client.stop();
    ClientState = 0;
  }

  //delay(100);
}
