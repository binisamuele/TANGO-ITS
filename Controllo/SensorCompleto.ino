#include <LiquidCrystal.h>


//costanti globali (più efficente con define)
#define SENSORS_NOMBER 4;
#define MAX_DISTANCE 700;
#define SPEED_OF_SOUND 0.0343;


//array di support per invio stringhe al seriale
char buffer[40];

//constanti gestione millis
const int fiveMinutes = 300000;
const int tenMinutes = 600000;

//
//!!da definire il funzionamento!!
DHT11 dht11(2);


//numero sensori di distanza
const int NUMBER_SENSORS=4;

//configurazione pin sensore distanza (vanno messi su pin in maniera crescente per esigenze del codice 
//configurazione pin sensori distanza (vanno messi su pin in maniera crescente per esigenze del codice 
//                                      altrimenti sostituire i for del programma)
  const int trigPinUp = 2;
  const int echoPinUp = 3; 
const int trigPinUp = 2;
const int echoPinUp = 3; 

  const int trigPinDown = 4;
  const int echoPinDown = 5; 
const int trigPinDown = 4;
const int echoPinDown = 5; 

  const int trigPinRh = 6;
  const int echoPinRh = 7; 
const int trigPinRh = 6;
const int echoPinRh = 7; 

  const int trigPinLh = 8;
  const int echoPinLh = 9; 
const int trigPinLh = 8;
const int echoPinLh = 9; 



@@ -39,7 +42,6 @@ const int voltmeter1Pin = A0;
const int voltmeter2Pin = A1;



//constanti gestione millis
const int fiveMinutes = 300000;
const int tenMinutes = 600000;
@@ -58,13 +60,13 @@ void emergencyManagement(){
  int emergency; 
  int distanceTolerance=35;

  while(emergency<NUMBER_SENSORS){
  while(emergency<SENSORS_NOMBER){
    int trigPin=trigPinUp;
    int echoPin=echoPinUp;
    int incrementToNextPin = 2;

    emergency=0;
    for(int i=0;i<NUMBER_SENSORS;i++){
    for(int i=0;i<SENSORS_NOMBER;i++){

      if( measureDistance( trigPin, echoPin, distanceTolerance) ){
        emergency++;
@@ -78,6 +80,7 @@ void emergencyManagement(){
  }
}

/* Verisione di Davide
int distanceManagement(){
  int trigPin=trigPinUp;
  int echoPin=echoPinUp;
@@ -86,7 +89,7 @@ int distanceManagement(){
  int incrementToNextPin = 2;
  int distanceTolerance=30;
  for(int i=0;i<NUMBER_SENSORS;i++){
  for(int i=0;i<SENSORS_NOMBER;i++){
    distance=measureDistance( trigPin, echoPin, distanceTolerance);
    if( distance ){
      sprintf(buffer, "distanzaUltraSuoni: %d cm", distance);
@@ -102,19 +105,21 @@ int distanceManagement(){
  }
} */

void distanceManagement(){

}


int  measureDistance(int trigPin, int echoPin, int distanceTolerance) {

  int distance;
  const int MAX_DISTANCE =700;
  int error =-1;

  NewPing sonar(trigPin, echoPin, MAX_DISTANCE);

  //distance = sonar.ping_cm();
  distance = (sonar.ping() / 2) * 0.0343;
  distance = (sonar.ping() / 2) * ;

  if (distance >= 0) {
    if (distance < distanceTolerance) {
@@ -123,7 +128,7 @@ int  measureDistance(int trigPin, int echoPin, int distanceTolerance) {
        return distance;
    }
  } else {
      return error;
      return -1;
  }

}
@@ -177,6 +182,7 @@ void setup() {


  lcd.begin(16, 2);         // Inizializza il display LCD

  pinMode(trigPinUp, OUTPUT);
  pinMode(echoPinUp, INPUT);
  pinMode(trigPinDown, OUTPUT);