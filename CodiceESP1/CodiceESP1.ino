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
    client.publish("casa/piano1/soggiorno/accendiled", "Accendi Led soggiorno");
  }
  if (res1 == 'g'){
    client.publish("casa/piano1/soggiorno/spegniled", "Spegni Led soggiorno");
  }
  if (res1 == 'h'){
    client.publish("casa/piano1/cameradaletto/alerttemperatura", "Alert Temperatura Camera Da Letto");
  }
}
  //maintain MQTT connection
  client.loop();
  //MUST delay to allow ESP8266 WIFI functions to run
  delay(20); 
}



char * readStringUntil(char * data,const unsigned int length, const unsigned long timeout_period,const char terminator, boolean * stato) {
unsigned int index = 0;
unsigned long start_time = millis ();
while (index <= length){
// check if time is up
if (millis () - start_time >= timeout_period)
return {}; // no data in timeout period

 // if data, add to buffer
if (Serial.available () > 0) {
char r = Serial.read();
if (r == terminator) {
data[index] = 0; // terminating null byte
*stato = true;
return data;
} // end if terminator reached

data [index++] = r;

} // end if available

 } // end while loop
*stato= 0;
 return {}; // filled up without terminator
} // end of readStringUntil



void callback(char* topic, byte* payload, unsigned int length) {
  //convert topic to string to make it easier to work with
String topicStr = topic; 
char statoled;
char temperatura[4] = ""; //Stiamo inserendo 4 in quanto dalla seriale ci arriverà ad esempio '19+\0'
//char temperatura_finale[3] = "";
char umidita[4] = ""; //Stiamo inserendo 4 in quanto dalla seriale ci arriverà ad esempio '35+\0'
boolean i;
if(topicStr == "casa/piano1/cameradaletto/accendiled"){
  Serial.println('a');
  return;
}


if(topicStr == "casa/piano1/cameradaletto/spegniled"){
  Serial.println('b');
  return;
}


if(topicStr == "casa/piano1/Temperatura/spegniAllarme"){
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
  client.publish("casa/piano1/statoledCameraDaLetto", "Off", true);
}
if (statoled == '1'){
   client.publish("casa/piano1/statoledCameraDaLetto", "On", true);
}
return;
}


if(topicStr == "casa/piano1/cameradaletto/richiedi/temperatura"){
Serial.println('e');
do {
readStringUntil(temperatura,4, 6000,'+', &i); //Man mano che aggiungiamo ulteriori righe di codice occorre aumentare il valore del parametro timeout (6000), altrimenti non funziona nulla.
} while(i == false);
/*
//Serial.println(temperatura_finale);
temperatura_finale[0] = temperatura[2];
temperatura_finale[1] = temperatura[3];
temperatura_finale[2] = '\0';
*/
client.publish("casa/piano1/cameradaletto/ottieni/temperatura", temperatura);
return;
}


if(topicStr == "casa/piano1/cameradaletto/richiedi/umidita"){
  Serial.println('f');
do {
readStringUntil(umidita,4, 4000,'+', &i); //Man mano che aggiungiamo ulteriori righe di codice occorre aumentare il valore del parametro timeout (4000), altrimenti non funziona nulla.
} while(i == false);
  client.publish("casa/piano1/cameradaletto/ottieni/umidita", umidita);
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
        client.subscribe("casa/piano1/cameradaletto/accendiled");
        client.subscribe("casa/piano1/cameradaletto/spegniled");
        client.subscribe("casa/piano1/Temperatura/spegniAllarme");
        client.subscribe("casa/piano1/led/isaccesi");
        client.subscribe("casa/piano1/cameradaletto/richiedi/temperatura");
        client.subscribe("casa/piano1/cameradaletto/richiedi/umidita");
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
