#include <Wire.h>
#include <DHT11.h>
#include <LiquidCrystal.h>


//array di support per invio stringhe al seriale
char buffer[40];

//constanti gestione millis
const int fiveMinutes = 300000;
const int tenMinutes = 600000;

//
DHT11 dht11(2);


//configurazione pin sensore distanza
const int triggerPinUp = 4;
const int echoPinUp = 5; 

const int triggerPinDown = 6;
const int echoPinDown = 7; 


//pin sensori voltimetri
const int voltmeter1Pin = A0;
const int voltmeter2Pin = A1;


//LiquidCrystal
LiquidCrystal lcd(12, 11, 6, 5, 8, 7);


//constanti sesore a ultrasuoni
const int SONAR_NUM = 2;
const int MAX_DISTANCE =700;
const double SPEED_OF_SOUND = 0.0343;


int measureDistance(int triggerPin, int echoPin) {

  NewPing ping = NewPing(triggerPin, echoPin, MAX_DISTANCE);
  int distance;

  NewPing sonar[SONAR_NUM] = {   // Sensor object array.
    NewPing(triggerPinUp, echoPinUp, MAX_DISTANCE),// Each sensor's trigger pin, echo pin, and max distance to ping. 
    NewPing(triggerPinDown, echoPinDown, MAX_DISTANCE) 
  };

  for(int i=0; i<SONAR_NUM; i++){

    distance = sonar[i].ping_cm();

    if (distance >= 0) {
      distance = (sonar[i].ping() / 2) * SPEED_OF_SOUND;
      if (distance < 30) {
        Serial.print("emergenza");
        Serial.print(distance);
      } else {
        sprintf(buffer, "distanzaUltraSuoni n%d: %d cm",(i+1), distance);
        Serial.println(buffer);
      }

    } else {
      Serial.println("Errore nella misurazione della distanza");
    }
  }
}

//funzione gestione Temperatura
int measureTemperature() {
  return dht11.readTemperature();
}
String printTemperature() {
  char buffer[40];
  sprintf(buffer, "Temperatura: %d °C \n", measureTemperature());
  return buffer;
}

//funzione gestione Umidità
int measureHumidity() {
  return dht11.readHumidity();
}
String printHumidity() {
  char buffer[40];
  sprintf(buffer, "Umidità: %d %% \n", measureHumidity());
  return buffer;
}

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
  
  //dht.begin();              // Inizializza il sensore DHT
  //lcd.begin(16, 2);         // Inizializza il display LCD
  pinMode(triggerPinUp, OUTPUT);
  pinMode(echoPinUp, INPUT);
  pinMode(triggerPinDown, OUTPUT);
  pinMode(echoPinDown, INPUT);



}

void loop() {

  measureDistance();
  
  
  // funzioni da eseguire ogni 5 minuti
  if (millis() % fiveMinutes == 0) {
    measureTemperatureAndHumidity();
    //updateLCD();
    measureVoltmeters();
  }
  if(millis() % tenMinutes == 0) {
  Serial.print(printTemperature);
  Serial.print(printHumidity());
  }
}