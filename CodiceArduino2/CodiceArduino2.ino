#define LedPin 8


char stato_led;
char stato_allarme = '0';
char condizione_fumo = '1';

void setup()
{
  Serial.begin(115200);
  Serial1.begin(115200);
  pinMode(7, OUTPUT); // definisce la porta 7 come una porta di output
  pinMode(A0, INPUT); // definisce la porta 7 come una porta di output
}



void loop()
{
  char res;
  //Allarme fumo
   delay(1000);
   int fumo_allarme = analogRead(A0);
   Serial.println(fumo_allarme);
   if (fumo_allarme < 200){
    condizione_fumo = '1';
   }
   if ((fumo_allarme > 200 && condizione_fumo == '1') || stato_allarme == '1'){
    if (stato_allarme == '0'){
      Serial1.println('f');
    }
    stato_allarme = '1';
    tone(7, 440, 500); // attiva il suono
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
  condizione_fumo = '0';
  noTone(7);
}


if (res == 'd'){
  Serial1.println(stato_led);
}

//Acquisisci qualità dell'aria
if (res == 'e'){
int gas = analogRead(A0);
if (gas <= 190)
  {
    Serial1.println('0');
  }
  else if (gas <= 300)
  {
    Serial1.println('1');
  }
  else
  {
    Serial1.println('2');
  }

 
}

  
}
  
  
}
