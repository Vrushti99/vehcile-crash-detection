#include <PubSubClient.h>
#include "UbidotsESPMQTT.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>


#define TOKEN "BBFF-G2fCfFmDo0FyEJOMpVe9iYs4KwHUUM" // Your Ubidots TOKEN
#define WIFINAME "Kaps" // SSID
#define WIFIPASS "987654321" //  Wifi Pass
const int trigP = 2;  //D4 Or GPIO-2 of nodemcu
const int echoP = 0;  //D3 Or GPIO-0 of nodemcu
int touch = 5; // d1
Ubidots client(TOKEN);



void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}


void setup() {
  // put your setup code here, to run once:
  //client.ubidotsSetBroker("industrial.api.ubidots.com"); // Sets the broker properly for the industrial account
  pinMode(trigP, OUTPUT);  // Sets the trigPin as an Output
pinMode(echoP, INPUT);   // Sets the echoPin as an Input
  client.setDebug(true); // Pass a true or false bool value to activate debug messages
  Serial.begin(115200);
  client.wifiConnection(WIFINAME, WIFIPASS);
  client.begin(callback);
  }

void loop() {
  // put your main code here, to run repeatedly:
  if(!client.connected()){
      client.reconnect();
      }
      digitalWrite(trigP, LOW);   // Makes trigPin low
delayMicroseconds(2);       // 2 micro second delay 

digitalWrite(trigP, HIGH);  // tigPin high
delayMicroseconds(10);      // trigPin high for 10 micro seconds
digitalWrite(trigP, LOW);   // trigPin low

long duration = pulseIn(echoP, HIGH);   //Read echo pin, time in microseconds
int distance= duration*0.034/2;        //Calculating actual/real distance

Serial.print("Distance = ");        //Output distance on arduino serial monitor
  
 
  client.add("distance", distance);
  client.ubidotsPublish("ultra");

  if (touch==HIGH)
  {Serial.print("The sensor is touched");
    }
client.add("touch", touch);
  client.ubidotsPublish("ultra");
  
  client.loop();
}
