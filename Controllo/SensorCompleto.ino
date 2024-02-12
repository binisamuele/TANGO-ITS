#include <Wire.h>
#include <DHT11.h>
#include <LiquidCrystal.h>
#include <NewPing.h>


//costanti globali 
#define SENSORS_NOMBER 4
#define MAX_DISTANCE 700
#define SPEED_OF_SOUND 0.0343
#define EMERGENCY_PIN 1


//array di support per invio stringhe al seriale
char buffer[40];

//constanti gestione millis
const int fiveMinutes = 300000;
const int tenMinutes = 600000;

//!!da definire il funzionamento!!
DHT11 dht11(2);


//configurazione pin sensori distanza (vanno messi su pin in maniera crescente per esigenze del codice 
//                                      altrimenti sostituire i for del programma)
const int trigPinUp = 22;
const int echoPinUp = 23; 

const int trigPinDown = 24;
const int echoPinDown = 25; 

const int trigPinRh = 26;
const int echoPinRh = 27; 

const int trigPinLh = 28;
const int echoPinLh = 29; 


NewPing sonar[SENSORS_NOMBER] = {   // Sensor object array.
  NewPing(trigPinUp, echoPinUp, MAX_DISTANCE),
  NewPing(trigPinDown, echoPinDown, MAX_DISTANCE),
  NewPing(trigPinRh, echoPinRh, MAX_DISTANCE),
  NewPing(trigPinLh, echoPinLh, MAX_DISTANCE)
};


//pin sensori voltimetri
const int voltmeter1Pin = A0;
const int voltmeter2Pin = A1;


//variabili di supporto per misura distanza
double distance = 0;
bool alarm = false;
int sensorIndex = 0;


LiquidCrystal lcd(8, 9, 4, 5, 6, 7);



//funzioni per la gestione della distanza
double measureDistance(int sonarNum) {

  return (sonar[sonarNum].ping() / 2) * SPEED_OF_SOUND;

}
String printDistance(double distance) { 
  // Serial.print("Distanza: ");
  // Serial.print(distance);
  // Serial.print(" cm \n");

  String contenitore = String("Distan: " + (String)distance +"cm ");
  return contenitore;

}

void distanceManagement() {

  if(sensorIndex < SENSORS_NOMBER) {

    if(alarm == true) {

      //stato di emergenza
      distance = measureDistance(sensorIndex);

      Serial.print(printDistance(distance));
      Serial.print("E");
      lcd.print(printDistance(distance));

      if (distance > 30) {
        alarm = false;
        Serial.print("FINE EMERGENZA \n");
        lcd.print("FINE EMERGENZA \n");
      }

    } else {

      //stato normale
      distance = measureDistance(sensorIndex);

      Serial.print(printDistance(distance));
      lcd.print(printDistance(distance));
      //lcd.print(printDistance(distance));


      if(distance < 20) {

        digitalWrite(EMERGENCY_PIN, HIGH);
        alarm = true;
        Serial.print("EMERGENZA \n");
        lcd.print("EMERGENZA \n");
      } else {
        sensorIndex++;
      }
    }
  }

  if(alarm == false) {
    sensorIndex = 0;
    distance = 0;
  }

  delay(50);
}

//funzioni gestione Temperatura
int measureTemperature() {
  return dht11.readTemperature();
}
String printTemperature() {
  char buffer[40];
  sprintf(buffer, "Temperatura: %d °C \n", measureTemperature());
  return buffer;
}

//funzioni gestione Umidità
int measureHumidity() {
  return dht11.readHumidity();
}
String printHumidity() {
  char buffer[40];
  sprintf(buffer, "Umidità: %d %% \n", measureHumidity());
  return buffer;
}


//funzioni monitoraggio stato della batteria
void measureVoltmeters() {
  // Misura tensione da voltmeter1Pin
  float voltage1 = analogRead(voltmeter1Pin) /40.92;
  sprintf(buffer, "voltaggio: %d.%d", (int)voltage1, ((int)(voltage1*10) % 10));
  Serial.println(buffer);
}

/* WIP da sistemare
void updateLCD() {
  lcd.setCursor(0, 1);
  lcd.print("distanzaUltraSuoni:");
  lcd.print(distanceCm);
  lcd.print(printTemperature());
  lcd.print(printHumidity());
}
*/

void setup() {

  Serial.begin(9600);       // Inizializza la comunicazione seriale a 9600 bps
  lcd.begin(16, 2);         // Inizializza il display LCD
  lcd.setCursor(0,0);
}

void loop() {


  delay(500);
    lcd.setCursor(0, 1);

  lcd.print(printDistance(34));
  //distanceManagement();
  
  // funzioni da eseguire ogni 5 minuti
  if (millis() % fiveMinutes == 0) {
    //updateLCD();
    measureVoltmeters();
  }
  if(millis() % tenMinutes == 0) {
  Serial.print(printTemperature());
  Serial.print(printHumidity());
  }
}