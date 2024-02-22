#include <NewPing.h>


//costanti globali 
#define SENSORS_NOMBER 6
#define MAX_DISTANCE 200
#define SPEED_OF_SOUND 0.0343
#define EMERGENCY_PIN 1


//frontale
const int TRIG_PIN_U = 22;
const int ECHO_PIN_U = 23; 
//frontale destro
const int TRIG_PIN_UR = 24;
const int ECHO_PIN_UR = 25; 
//frontale sinistro
const int TRIG_PIN_UL = 26;
const int ECHO_PIN_UL = 27; 


//posteriore
const int TRIG_PIN_D = 28;
const int ECHO_PIN_D = 29; 
//posteriore destro
const int TRIG_PIN_DR = 30;
const int ECHO_PIN_DR = 31; 
//posteriore sinistro
const int TRIG_PIN_DL = 32;
const int ECHO_PIN_DL = 33; 


//variabili di supporto per misura distanza
double distance = 0;
bool alarm = false;
int sensorIndex = 0;
const int sensoriMancanti = 2;


//Inizializzazione sensori
NewPing sonar[SENSORS_NOMBER - sensoriMancanti] = {   
  NewPing(TRIG_PIN_U, ECHO_PIN_U, MAX_DISTANCE),      //sensore frontale
  NewPing(TRIG_PIN_UR, ECHO_PIN_UR, MAX_DISTANCE),    //sensore frontale destro
  NewPing(TRIG_PIN_UL, ECHO_PIN_UL, MAX_DISTANCE),    //sensore frontale sinistro
  NewPing(TRIG_PIN_D, ECHO_PIN_D, MAX_DISTANCE)       //sensore posteriore 
  //NewPing(TRIG_PIN_DR, ECHO_PIN_DR, MAX_DISTANCE)   //sensore posteriore destro
  //NewPing(TRIG_PIN_DL, ECHO_PIN_DL, MAX_DISTANCE)   //sensore posteriore sinistro
};

                                                                                               
// MISURA DISTANZA           
///////////////////////////////////////////////////////////////////////////////
//funzioni per la gestione della distanza
double measureDistance(int sonarNum) {
  return (sonar[sonarNum].ping() / 2) * SPEED_OF_SOUND;
}
String printDistance(double distance) { 

    if(alarm == true) Serial.print("\n E ");
    if(sensorIndex == 0) Serial.print("\n S0 ");
    if(sensorIndex == 1) Serial.print("\n S1 ");
    if(sensorIndex == 2) Serial.print("\n S2 ");
    if(sensorIndex == 3) Serial.print("\n S3 ");
    return String("Distanza: " + (String)distance +"cm");
        
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
  } else {
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
  delay(500);
}