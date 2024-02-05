#include <Wire.h>
#include <DHT11.h>
#include <LiquidCrystal.h>
#include <NewPing.h>


//costanti globali 
#define SENSORS_NOMBER 1
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
const int trigPinUp = 2;
const int echoPinUp = 3; 

const int trigPinDown = 4;
const int echoPinDown = 5; 

const int trigPinRh = 6;
const int echoPinRh = 7; 

const int trigPinLh = 8;
const int echoPinLh = 9; 


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


LiquidCrystal lcd(12, 11, 6, 5, 8, 7);

//funzioni per la gestione della distanza
double measureDistance(int sonarNum) {

  return (sonar[sonarNum].ping() / 2) * SPEED_OF_SOUND;

}
void printDistance(double distance) { 
  Serial.print("Distanza: ");
  Serial.print(distance);
  Serial.print(" cm \n");
}

void distanceManagement() {

  if(sensorIndex < SENSORS_NOMBER) {

    if(alarm == true) {

      //stato di emergenza
      distance = measureDistance(sensorIndex);
      printDistance(distance);

      if (distance > 30) {
        alarm = false;
        Serial.print("FINE EMERGENZA \n");
      }

    } else {

      //stato normale
      distance = measureDistance(sensorIndex);
      printDistance(distance);

      if(distance < 20) {

        digitalWrite(EMERGENCY_PIN, HIGH);
        alarm = true;
        Serial.print("EMERGENZA \n");
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
    // Misura tensione da voltmeter1Pin e voltmeter2Pin
    float voltage1 = analogRead(voltmeter1Pin) * (5.0 / 1023.0);
    //float voltage2 = analogRead(voltmeter2Pin) * (5.0 / 1023.0);

    // Stampa tensioni sulla porta seriale
    Serial.print("V1: ");
    Serial.print(voltage1, 2);
    Serial.print("V");
    //Serial.print(voltage2, 2);
    //Serial.println("V");

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

}

void loop() {

  distanceManagement();
  
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