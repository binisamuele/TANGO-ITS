#include <NewPing.h>


//costanti globali 
#define SENSORS_NOMBER 6
#define MAX_DISTANCE 200
#define SPEED_OF_SOUND 0.0343
#define EMERGENCY_PIN 1


//frontale
const int trigPinU = 22;
const int echoPinU = 23; 
//frontale destro
const int trigPinUR = 24;
const int echoPinUR = 25; 
//frontale sinistro
//const int trigPinUL = 26;
//const int echoPinUL = 27; 


//posteriore
const int trigPinD = 28;
const int echoPinD = 29; 
//posteriore destro
const int trigPinDR = 30;
const int echoPinDR = 31; 
//posteriore sinistro
//const int trigPinDL = 32;
//const int echoPinDL = 33; 


//variabili di supporto per misura distanza
double distance = 0;
bool alarm = false;
int sensorIndex = 0;
const int sensoriMancanti = 2;


//Inizializzazione sensori
NewPing sonar[SENSORS_NOMBER] = {   // Sensor object array.
  NewPing(trigPinU, echoPinU, MAX_DISTANCE),
  NewPing(trigPinUR, echoPinUR, MAX_DISTANCE),
  //NewPing(trigPinUL, echoPinUL, MAX_DISTANCE),
  NewPing(trigPinD, echoPinD, MAX_DISTANCE),
  NewPing(trigPinDR, echoPinDR, MAX_DISTANCE)
  //NewPing(trigPinDL, echoPinDL, MAX_DISTANCE)
};

//funzioni per la gestione della distanza
double measureDistance(int sonarNum) {
  return (sonar[sonarNum].ping() / 2) * SPEED_OF_SOUND;
}
String printDistance(double distance) { 

    if(alarm == true) Serial.print("E");
    if(sensorIndex == 0)
    return String("\nDistanza: " + (String)distance +"cm ");
        
}

void distanceManagement() {

  if(sensorIndex < SENSORS_NOMBER - sensoriMancanti) {

    if(alarm == true) {

      //stato di emergenza
      distance = measureDistance(sensorIndex);
      Serial.print(printDistance(distance));


      if (distance > 30) {
        alarm = false;
        Serial.print("FINE EMERGENZA \n");
      }

    } else {

      //stato normale
      distance = measureDistance(sensorIndex);
      Serial.print(printDistance(distance));


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

void setup() {

  Serial.begin(9600);       // Inizializza la comunicazione seriale a 9600 bps

}

void loop() {

  distanceManagement();

}