#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include "osapi.h"
#include "ets_sys.h"
#include "os_type.h"
#include "c_types.h"

#include "DHT.h"
//define-------------------------------------------------------------------------
#define DHTTYPE DHT22

//static variables---------------------------------------------------------------
static os_timer_t heartBeatTimer;
static uint8_t ClientState;
static uint8_t ledState;

const char* ssid = "navtah10";
const char* password = "qwas1234";
const char* heartBeatword = "HEARTBEAT";
const int ledPin = 12;
const int DHTPin = 14;
WiFiUDP Udp;

void heartBeat()
{
  Serial.println("Esp heartBeating...");
  if(ledState++ % 2)
  {
    digitalWrite(ledPin, HIGH);
  }
  else
  {
    digitalWrite(ledPin, LOW);
  }
  Udp.beginPacket("192.168.1.235", 13000);
  Udp.write(heartBeatword);
  Udp.endPacket();
}

void setup() {
  Serial.begin(115200);
  Serial.println("Arduino ESP8266 client starting...");
  ledState = 0;
  pinMode(ledPin, OUTPUT);
  os_timer_setfn(&heartBeatTimer, (os_timer_func_t*)heartBeat, (void*)0);
  os_timer_arm(&heartBeatTimer, 3000, 1);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  while( WiFi.status() != WL_CONNECTED)
  { 
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  //Udp.begin(13000);
 ClientState = 0;
}

void loop() {
  WiFiClient client;
  DHT dht(DHTPin, DHTTYPE);
  dht.begin();
  if(client.connect("192.168.1.235",13001))
  {
    Serial.println("Connected TCP host");
  }
  else
  {
    Serial.println("Error connecting TCP host");
  }
  if(ClientState == 0)
  {
    Serial.println("TCP host asking");
    client.print("DAEMON_ASK");
    while(client.connected());
    {
      client.print("LOL");
      if(client.available() > 0)
      {
        String data = client.readStringUntil('\n');
        Serial.println("Data available!!");
        Serial.println(data);
        if(data.equals("DHT_GET"))
        {
          Serial.println("DHT Meas requested");
          ClientState = 1;
          client.stop();
        }
      }
    }
  }
  else if(ClientState == 1)
  {
    Serial.println("DHT Data send to host");
    String hum = String(dht.readHumidity());
    String temp = String(dht.readTemperature());
    String res = String("TEMP: "+ temp + " " + " HUM: " + hum);
    Serial.println(res);
    client.print(res);
    while(client.connected())
    {
      if(client.available())
      {
        Serial.println("SENT!");
        String data = client.readStringUntil('\n');
        if(data.equals("OK"))
        {
          Serial.println("DHT measure sent successfully");
          ClientState = 0;
          client.stop();
        }
      }
    }
    
  }
  else if(ClientState == 2)
  {
    
  }
  else
  {
    Serial.println("Error connecting TCP host");
  }
}
