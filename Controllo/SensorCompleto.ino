#include <Wire.h>
#include <DHT.h>
#include <LiquidCrystal.h>

const int trigPin = 9;
const int echoPin = 10;
const float speedOfSound = 0.034;  // Velocità del suono in cm/microsecondo
const int DHTPIN = 3;   // Pin a cui è connesso il sensore
#define DHTTYPE DHT22   // Tipo di sensore che stiamo utilizzando (DHT22)

DHT dht(DHTPIN, DHTTYPE);  // Dichiarazione dell'oggetto DHT

LiquidCrystal lcd(12, 11, 6, 5, 8, 7);

int chk;
float hum;  // Variabile in cui verrà inserita la % di umidità
float temp; // Variabile in cui verrà inserita la temperatura
long duration;
int distanceCm;

void setup() {
  Serial.begin(9600); // Inizializza la comunicazione seriale a 9600 bps
  dht.begin();  // Inizializza il sensore DHT
  lcd.begin(16, 2); // Inizializza il display LCD
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

 
}

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

void updateLCD() {
  lcd.setCursor(0, 1);
  lcd.print("D:");
  lcd.print(distanceCm);
  lcd.print("cm T:");
  lcd.print(temp, 1);
  lcd.print(" C H:");
  lcd.print(hum, 1);
  lcd.print("% ");

}

void loop() {
  triggerUltrasonicSensor();
  duration = measurePulseDuration();
  if (duration >= 0) {
    distanceCm = duration * speedOfSound / 2;

    // Stampare la distanza sulla porta seriale
    Serial.print("D:");
    Serial.print(distanceCm);
    Serial.println(" cm");


  } else {
    // Stampare un messaggio di errore sulla porta seriale
    Serial.println("Errore nella misurazione della distanza");

     // Aggiorna il display LCD con un messaggio di errore
    lcd.setCursor(0, 1);
    lcd.print("Error");
  }

  delay(2000);  // Ritardo di 5 secondi
  hum = dht.readHumidity();
  temp = dht.readTemperature();

  // Stampa umidità e temperatura tramite monitor seriale
  Serial.print("H: ");
  Serial.print(hum, 1);
  Serial.print("%,T: ");
  Serial.print(temp, 1);
  Serial.println(" C");


  // Aggiorna il display LCD con i dati di umidità e temperatura
  lcd.setCursor(0, 0);
  lcd.print("H:");
  lcd.print(hum, 1);
  lcd.print("%");
  lcd.setCursor(0, 1);
  lcd.print("T:");
  lcd.print(temp, 1);
  lcd.print(" C");


  // Aggiorna il display LCD
    updateLCD();
}