#include <Wire.h>
#include <DHT.h>
#include <LiquidCrystal.h>
#define DHTTYPE DHT22

//array di support per invio stringhe al seriale
char buffer[40];

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


const int DHTPIN = 3;
DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal lcd(12, 11, 6, 5, 8, 7);

int distanceCm;
long duration;

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
    lcd.setCursor(0, 1);
    lcd.print("Error");
  }
  
}

void updateLCD() {
  lcd.setCursor(0, 1);
  lcd.print("distanzaUltraSuoni:");
  lcd.print(distanceCm);
  lcd.print("cm T:");
  lcd.print(temp, 1);
  lcd.print(" C H:");
  lcd.print(hum, 1);
  lcd.print("% ");
}

void measureTemperatureAndHumidity() {
  hum = dht.readHumidity();
  sprintf(buffer, "Humidity: %d %", hum);
  Serial.println(buffer);

  temp = dht.readTemperature();
  sprintf(buffer, "Temperature: %d C", temp);
  Serial.println(buffer);
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

void setup() {
  Serial.begin(9600);       // Inizializza la comunicazione seriale a 9600 bps
  dht.begin();              // Inizializza il sensore DHT
  lcd.begin(16, 2);         // Inizializza il display LCD
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {

  measureDistance();

  // funzioni da eseguire ogni 5 minuti
  if (millis() % fiveMinutes == 0) {
    measureTemperatureAndHumidity();
    updateLCD();
    measureVoltmeters();
  }
}