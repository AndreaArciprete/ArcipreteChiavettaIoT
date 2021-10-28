#include <PubSubClient.h>
#include <ESP8266WiFi.h>
//EDIT THESE LINES TO MATCH YOUR SETUP
#define MQTT_SERVER "broker.hivemq.com"  ///YourMQTTBroker'sIP
const char* ssid = "TIM-23294875";
const char* password = "OU1k4oVoUhwyp59xGzXDMFmT";
WiFiClient wifiClient;



void setup() {
  //initialize the light as an output and set to LOW (off)
  //start the serial line for debugging
  Serial.begin(115200);
  delay(100);
  //start wifi subsystem
  WiFi.begin(ssid, password);
  //attempt to connect to the WIFI network and then connect to the MQTT server
  reconnect();
  //wait a bit before starting the main loop
      delay(2000);
}


PubSubClient client(MQTT_SERVER, 1883, callback, wifiClient);


void loop() {
  char res1;
  //reconnect if connection is lost
  if (!client.connected() && WiFi.status() == 3) {reconnect();}
  
if (Serial.available () > 0) {
  res1 = Serial.read();
  if (res1 == 'f'){
    client.publish("casa/piano1/soggiorno/alertfumo", "Alert Fumo Soggiorno");
  }
}
  //maintain MQTT connection
  client.loop();
  //MUST delay to allow ESP8266 WIFI functions to run
  delay(20); 
}


void callback(char* topic, byte* payload, unsigned int length) {
  //convert topic to string to make it easier to work with
String topicStr = topic;
char statoled;
char qualitaaria;
if(topicStr == "casa/piano1/soggiorno/accendiled"){
  Serial.println('a');
  return;
}


if(topicStr == "casa/piano1/soggiorno/spegniled"){
  Serial.println('b');
  return;
}


if(topicStr == "casa/piano1/Fumo/spegniAllarme"){
  Serial.println('c');
  return;
}



if(topicStr == "casa/piano1/led/isaccesi"){
  Serial.println('d');
do {
if (Serial.available()){
  statoled = Serial.read();
}
} while(statoled != '0' && statoled!= '1');
if (statoled == '0'){
  client.publish("casa/piano1/statoledSoggiorno", "Off", true);
}
if (statoled == '1'){
   client.publish("casa/piano1/statoledSoggiorno", "On", true);
}
return;
}


if(topicStr == "casa/piano1/soggiorno/richiedi/qualitaaria"){
Serial.println('e');
do {
if (Serial.available()){
  qualitaaria = Serial.read();
}
} while(qualitaaria != '0' && qualitaaria!= '1' && qualitaaria != '2');
if (qualitaaria == '0'){
  client.publish("casa/piano1/soggiorno/ottieni/qualitaaria", "Buona", true);
}
if (qualitaaria == '1'){
   client.publish("casa/piano1/soggiorno/ottieni/qualitaaria", "Mediocre", true);
}
if (qualitaaria == '2'){
   client.publish("casa/piano1/soggiorno/ottieni/qualitaaria", "Pessima", true);
}
return;
}

}

  
void reconnect() {
  //attempt to connect to the wifi if connection is lost
  if(WiFi.status() != WL_CONNECTED){
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
    }
  }
//make sure we are connected to WIFI before attemping to reconnect to MQTT
  if(WiFi.status() == WL_CONNECTED){
  // Loop until we're reconnected to the MQTT server
    while (!client.connected()) {
      //Serial.print("Attempting MQTT connection...");
      // Generate client name based on MAC address and last 8 bits of microsecond counter
      String clientName;
      clientName += "esp8266-";
      uint8_t mac[6];
      WiFi.macAddress(mac);
      clientName += macToStr(mac);
      //if connected, subscribe to the topic(s) we want to be notified about
      if (client.connect((char*) clientName.c_str())) {
        //Serial.print("\tMTQQ Connected");
        client.subscribe("casa/piano1/soggiorno/accendiled");
        client.subscribe("casa/piano1/soggiorno/spegniled");
        client.subscribe("casa/piano1/Fumo/spegniAllarme");
        client.subscribe("casa/piano1/led/isaccesi");
        client.subscribe("casa/piano1/soggiorno/richiedi/qualitaaria");
      }
      //otherwise print failed for debugging
      else{abort();}
    }
  }
}
//generate unique name from MAC addr
String macToStr(const uint8_t* mac){
  String result;
  for (int i = 0; i < 6; ++i) {
    result += String(mac[i], 16);
    if (i < 5){
      result += ':';
    }
  }
  return result;
}
