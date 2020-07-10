

#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <stdio.h>



#define WIFISSID "Kaps" 
#define PASSWORD "987654321"  
#define TOKEN "BBFF-G2fCfFmDo0FyEJOMpVe9iYs4KwHUUM" 
#define VARIABLE_LABEL "distance"
#define DEVICE_LABEL "ultra" 
#define MQTT_CLIENT_NAME "vruskash99" 

char mqttBroker[] = "industrial.api.ubidots.com";
char payload[700];
char topic[150];


char str_val[6];
char str_lat[6];
char str_lng[6];



ESP8266WiFiMulti WiFiMulti;
WiFiClient ubidots;
PubSubClient client(ubidots);


 
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0;i<length;i++) {
  Serial.print((char)payload[i]);
  }
  Serial.println();
} 

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
  Serial.println("Attempting MQTT connection...");

  // Attempt to connect
  if (client.connect(MQTT_CLIENT_NAME, TOKEN,"")) {
    Serial.println("connected");
  } else {
    Serial.print("failed, rc=");
    Serial.print(client.state());
    Serial.println(" try again in 2 seconds");
    // Wait 2 seconds before retrying
    delay(2000);
  }
  }
}



void setup() {
  Serial.begin(115200);
  pinMode(A0, INPUT);
  WiFiMulti.addAP(WIFISSID, PASSWORD);
  Serial.println();
  Serial.println();
  Serial.print("Wait for WiFi... ");

  while(WiFiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  client.setServer(mqttBroker, 1883);
  client.setCallback(callback);
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }

  // Values to send
  float truck_position = random(0, 9);
  float lat = 21.2339;
  float lng = 72.8569;

  dtostrf(truck_position, 4, 2, str_val);
  dtostrf(lat, 4, 2, str_lat);
  dtostrf(lng, 4, 2, str_lng);

  sprintf(topic, "%s", ""); // Cleans the topic content
  sprintf(topic, "%s%s", "/v1.6/devices/", DEVICE_LABEL);

  sprintf(payload, "%s", ""); // Cleans the payload content
  sprintf(payload, "{\"%s\":", VARIABLE_LABEL); //    
  sprintf(payload, "%s {\"value\": %s", payload, str_val); // Adds the value
  sprintf(payload, "%s, \"context\":{\"lat\": %s, \"lng\": %s}", payload, str_lat, str_lng); // Adds coordinates
  sprintf(payload, "%s } }", payload); // Closes the dictionary brackets
 Serial.println("publishing with payload:");
    Serial.println(payload);
    client.publish(topic, payload);
    Serial.println("finished");
    client.loop();
    delay(1000);
}
