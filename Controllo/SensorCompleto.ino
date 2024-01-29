#include <Wire.h>
#include <DHT11.h>
#include <LiquidCrystal.h>


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
//const int DHTPIN = 3;
//DHT dht(DHTPIN, DHTTYPE);
DHT11 dht11(3);

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
  lcd.begin(16, 2);         // Inizializza il display LCD
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {

  //measureDistance();

  delay(2000);
  // funzioni da eseguire ogni 5 minuti
  if (millis() % fiveMinutes == 0) {
    measureTemperatureAndHumidity();
    updateLCD();
    measureVoltmeters();
  }
  if(millis() % tenMinutes == 0) {
  Serial.print(printTemperature);
  Serial.print(printHumidity());
  }
}