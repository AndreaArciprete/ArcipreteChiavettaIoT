#include <SimpleDHT.h>
#include <MFRC522.h>
#define DHTPIN 2
#define LedPin 8
SimpleDHT11 dht11;
MFRC522 rfid(53, 9);


char stato_led;
char stato_allarme = '0';
char condizione_temp = '1';
void setup()
{
  Serial.begin(115200);
  Serial1.begin(115200);
  SPI.begin();
  rfid.PCD_Init();
  pinMode(7, OUTPUT); // definisce la porta 7 come una porta di output
}


String getUID(){
  String uid = "";
  for(int i = 0; i < rfid.uid.size; i++){
    uid += rfid.uid.uidByte[i] < 0x10 ? "0" : "";
    uid += String(rfid.uid.uidByte[i], HEX);
  }
  rfid.PICC_HaltA();
  return uid;
}


 int leggiTemperatura(){
  byte temp = 0;
  byte umid = 0;
  int err = SimpleDHTErrSuccess;
  if ((err = dht11.read(DHTPIN, &temp, &umid, NULL)) != SimpleDHTErrSuccess) {
    //Serial.print("Read DHT11 failed, err=");
    //Serial.println(err);
    delay(1000);
    return -5;
  }
return (int)temp;
 
 }



 int leggiUmidita(){
  byte temp = 0;
  byte umid = 0;
  int err = SimpleDHTErrSuccess;
  if ((err = dht11.read(DHTPIN, &temp, &umid, NULL)) != SimpleDHTErrSuccess) {
    //Serial.print("Read DHT11 failed, err=");
    //Serial.println(err);
    delay(1000);
    return -5;
  }
return (int)umid;
 
 }

void loop()
{
  char res;
if(rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()){
    String uid = getUID();
    if (uid == "8b44360b"){
      if (stato_led == '0'){
        digitalWrite(LedPin,HIGH);
        stato_led = '1';
        Serial1.println('f');
      }
      else {
        digitalWrite(LedPin,LOW);
        stato_led = '0';
        Serial1.println('g');
      }
    }
  }

   delay(1500);
   int temperatura_allarme = leggiTemperatura();
   if (temperatura_allarme < 26){
    condizione_temp = '1';
   }
   if ((temperatura_allarme > 26 && condizione_temp == '1') || stato_allarme == '1'){
    if (stato_allarme == '0'){
      Serial1.println('h');
    }
    stato_allarme = '1';
    digitalWrite(7, HIGH); // attiva il suono
   }
if (Serial1.available() > 0) {
  res = Serial1.read();
  Serial.println(res);
if (res == 'a'){
  digitalWrite(LedPin,HIGH);
  stato_led = '1';
}
if (res == 'b'){
  digitalWrite(LedPin, LOW);
  stato_led = '0';
}

//spegniallarme
if (res == 'c'){
  stato_allarme = '0';
  condizione_temp = '0';
  digitalWrite(7, LOW);
}


if (res == 'd'){
  Serial1.println(stato_led);
}

if (res == 'e'){
  delay(2000);
  int temperatura = leggiTemperatura();
  char array_temp[4]; //Stiamo inserendo 4 in quanto sulla seriale arriverà qualcosa del tipo '23+\0'
  itoa(temperatura, array_temp,10);
  char tmp[2] = "+";
  strcat(array_temp,tmp);
  Serial1.println(array_temp);
}
if (res == 'f'){
  delay(2000);
  int umidita = leggiUmidita();
  char array_umid[4]; //Stiamo inserendo 4 in quanto sulla seriale arriverà qualcosa del tipo '53+\0'
  itoa(umidita, array_umid,10);
  char tmp[2] = "+";
  strcat(array_umid,tmp);
  Serial1.println(array_umid);
}
  
}
  
  
}
