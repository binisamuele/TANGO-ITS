#include <Wire.h>
//#include <DHT.h>
#include <LiquidCrystal.h>

//array di support per invio stringhe al seriale
char buffer[40];

//costante per utilizzare sensore a ultrasuoni
const float speedOfSound = 0.034;  // cm/microsecondo

//configurazione pin
const int trigPin = 9;
const int echoPin = 10;

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

void measureDistance() {
  int distanceCm = 0;
  long duration = 0;

  triggerUltrasonicSensor();
  duration = measurePulseDuration();
  if (duration >= 0) {
    distanceCm = duration * speedOfSound / 2;
    if (distanceCm < 20) {
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

void setup() {
  Serial.begin(9600);       // Inizializza la comunicazione seriale a 9600 bps
  //dht.begin();              // Inizializza il sensore DHT
  //lcd.begin(16, 2);         // Inizializza il display LCD
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  measureDistance();

}
