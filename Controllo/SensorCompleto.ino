#include <Wire.h>
#include <DHT11.h>
#include <LiquidCrystal.h>
#include <math.h>
#define DHTTYPE DHT11

//configurazione pin
const int trigPin = 9;
const int echoPin = 10;
//pin sensori voltimetri
const int voltmeter1Pin = A0;
const int voltmeter2Pin = A1;

//costante per utilizzare sensore a ultrasuoni
const float speedOfSound = 0.034;  // cm/microsecondo

//constanti gestione millis
const int fiveMinutes = 300000;
const int tenMinutes = 600000;

// pin per sensore DHT
const int DHTPIN = 3;
DHT dht(DHTPIN, DHTTYPE);

LiquidCrystal lcd(12, 11, 6, 5, 8, 7);

//funzioni per sensore a ultrasuoni
void triggerUltrasonicSensor() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(1000);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(2000);
  digitalWrite(trigPin, LOW);
}

long measurePulseDuration() {
  return pulseIn(echoPin, HIGH);
}

int  measureDistance() {

  int distanceCm;

  triggerUltrasonicSensor();
  if (measurePulseDuration() >= 0) {
    distanceCm = measurePulseDuration() * speedOfSound / 2;
    if (distanceCm != 0 && distanceCm < 20) {
      Serial.print("emergenza");
      Serial.print(distanceCm);
    } else {
      sprintf(buffer, "distanzaUltraSuoni: %d cm", distanceCm);
      Serial.println(buffer);
    }

  } else {
    Serial.println("Errore nella misurazione della distanza");
    //lcd.setCursor(0, 1);
    //lcd.print("Error");
  }
  
}

/*
void measureTemperatureAndHumidity() {
  float Temperature = dht.readTemperature();
  sprintf(buffer, "Temperature: %d C", temp);
  Serial.println(buffer);
}
*/

//funzione gestione Temperatura
float measureTemperature() {
  return round(dht.readTemperature());
}
String printTemperature() {
  char buffer[40];
  sprintf(buffer, "Temperatura: %d °C", measureTemperature());
  return buffer;
}

//funzione gestione Umidità
float measureHumidity() {
  return round(dht.readHumidity());
}
String printHumidity() {
  char buffer[40];
  sprintf(buffer, "Umidità: %d %", measureHumidity());
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

void updateLCD() {
  lcd.setCursor(0, 1);
  lcd.print("distanzaUltraSuoni:");
  lcd.print(distanceCm);
  lcd.print("cm Temperatura:");
  lcd.print(measureTemperature(), 1);
  lcd.print(" C Umidità:");
  lcd.print(hum, 1);
  lcd.print("% ");
}

void setup() {
  Serial.begin(9600);       // Inizializza la comunicazione seriale a 9600 bps
  dht.begin();              // Inizializza il sensore DHT
  lcd.begin(16, 2);         // Inizializza il display LCD
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {

  //measureDistance();
  printHumidity();
  printTemperature();

  // funzioni da eseguire ogni 5 minuti
  if (millis() % fiveMinutes == 0) {
    measureTemperatureAndHumidity();
    updateLCD();
    measureVoltmeters();
  }
  if(millis() % tenMinutes == 0) {

  }
}