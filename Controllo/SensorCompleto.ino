#include <Wire.h>
#include <DHT11.h>
#include <LiquidCrystal.h>
#include <NewPing.h>


//costanti globali 
#define SENSORS_NOMBER 6
#define MAX_DISTANCE 700
#define SPEED_OF_SOUND 0.0343
#define EMERGENCY_PIN 1
#define VOLTMETER_COSTANT 40.92

//constanti gestione millis
const int fiveMinutes = 300000;
const int tenMinutes = 600000;

//!!da definire il funzionamento!!
DHT11 dht11(2);


//frontale
const int trigPinU = 22;
const int echoPinU = 23; 
//frontale destro
const int trigPinUR = 24;
const int echoPinUR = 25; 
//frontale sinistro
const int trigPinUL = 26;
const int echoPinUL = 27; 

//posteriore
const int trigPinD = 28;
const int echoPinD = 29; 
//posteriore destro
const int trigPinDR = 30;
const int echoPinDR = 31; 
//posteriore sinistro
const int trigPinDL = 32;
const int echoPinDL = 33; 

//Inizializzazione sensori
NewPing sonar[SENSORS_NOMBER] = {   // Sensor object array.
  NewPing(trigPinU, echoPinU, MAX_DISTANCE),
  NewPing(trigPinUR, echoPinUR, MAX_DISTANCE),
  NewPing(trigPinUL, echoPinUL, MAX_DISTANCE),
  NewPing(trigPinD, echoPinD, MAX_DISTANCE),
  NewPing(trigPinDR, echoPinDR, MAX_DISTANCE),
  NewPing(trigPinDL, echoPinDL, MAX_DISTANCE)
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

  String buffer = String("Distan: " + (String)distance +"cm ");
  return buffer;

}

void distanceManagement() {

  if(sensorIndex < SENSORS_NOMBER) {

    if(alarm == true) {

      //stato di emergenza
      distance = measureDistance(sensorIndex);

      Serial.print(printDistance(distance));
      Serial.print("E");
      //lcd.print(printDistance(distance));

      if (distance > 30) {
        alarm = false;
        Serial.print("FINE EMERGENZA \n");
        //lcd.print("FINE EMERGENZA \n");
      }

    } else {

      //stato normale
      distance = measureDistance(sensorIndex);

      Serial.print(printDistance(distance));
      //lcd.print(printDistance(distance));


      if(distance < 20) {

        digitalWrite(EMERGENCY_PIN, HIGH);
        alarm = true;
        Serial.print("EMERGENZA \n");
        //lcd.print("EMERGENZA \n");
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
float measureVoltmeters() {
  // Misura tensione da voltmeter1Pin
  float voltage1 = analogRead(voltmeter1Pin) / VOLTMETER_COSTANT;
  sprintf(buffer, "voltaggio: %d.%d", (int)voltage1, ((int)(voltage1*10) % 10));
  Serial.println(buffer);
}

void setup() {

  Serial.begin(9600);       // Inizializza la comunicazione seriale a 9600 bps
  lcd.begin(16, 2);         // Inizializza il display LCD
  lcd.setCursor(0,0);
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