
#include <DHT11.h>
#include <LiquidCrystal.h>
#include <NewPing.h>


//costanti globali 
<<<<<<< HEAD
#define SENSORS_NOMBER 4
#define MAX_DISTANCE 200
=======
#define SENSORS_NOMBER 6
#define MAX_DISTANCE 700
>>>>>>> 2ee1231d531b79d9681f3855c3fb84daf5ca6c7c
#define SPEED_OF_SOUND 0.0343
#define EMERGENCY_PIN 1
#define VOLTMETER_COSTANT 40.92
#define MAX_VOLTAGE 25

//constanti gestione millis
const int fiveMinutes = 300000;
const int tenMinutes = 600000;

//
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


//funzioni per la gestione della distanza
double measureDistance(int sonarNum) {
  return (sonar[sonarNum].ping() / 2) * SPEED_OF_SOUND;
}
String printDistance(double distance) { 
<<<<<<< HEAD
  Serial.print("\n");
  
  return  String("Dist: " + (String)distance +"cm ");

=======
  return String("\nDistanza: " + (String)distance +"cm ");
>>>>>>> 2ee1231d531b79d9681f3855c3fb84daf5ca6c7c
}

void distanceManagement() {
  char setLcd;

  if(
     < SENSORS_NOMBER) {

    if(alarm == true) {

      //stato di emergenza
      distance = measureDistance(sensorIndex);

      Serial.print(printDistance(distance));
<<<<<<< HEAD
      Serial.print(" E");
      setLcd ='E';
      lcdManagement(distance,setLcd);  //lcd.print(printDistance(distance));
      
=======
      Serial.print("E");
      //lcd.print(printDistance(distance));
>>>>>>> 2ee1231d531b79d9681f3855c3fb84daf5ca6c7c

      if (distance > 30) {
        alarm = false;
        Serial.print("FINE EMERGENZA \n");
<<<<<<< HEAD

=======
        //lcd.print("FINE EMERGENZA \n");
>>>>>>> 2ee1231d531b79d9681f3855c3fb84daf5ca6c7c
      }

    } else {

      //stato normale
      distance = measureDistance(sensorIndex);

      Serial.print(printDistance(distance));
<<<<<<< HEAD
      setLcd ='N';
      lcdManagement(distance,setLcd);
      
=======
      //lcd.print(printDistance(distance));
>>>>>>> 2ee1231d531b79d9681f3855c3fb84daf5ca6c7c


      if(distance < 20) {

        digitalWrite(EMERGENCY_PIN, HIGH);
        alarm = true;
<<<<<<< HEAD
        Serial.print("/n EMERGENZA");
=======
        Serial.print("EMERGENZA \n");
        //lcd.print("EMERGENZA \n");
>>>>>>> 2ee1231d531b79d9681f3855c3fb84daf5ca6c7c
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
  sprintf(buffer, "\nTemperatura: %d °C", measureTemperature());
  return buffer;
}

//funzioni gestione Umidità
int measureHumidity() {
  return dht11.readHumidity();
}
String printHumidity() {
  char buffer[40];
  sprintf(buffer, "\nUmidità: %d %%", measureHumidity());
  return buffer;
}


//funzioni monitoraggio stato della batteria
float measureVoltage() {
  // Misura tensione da voltmeter1Pin
  return analogRead(voltmeter1Pin) / VOLTMETER_COSTANT;

}
String printVoltage(float voltage) { 

  return String("\nCarica batteria: " + (String)((voltage*100)/MAX_VOLTAGE) +"% ");

<<<<<<< HEAD
/* WIP da sistemare
void updateLCD() {
  lcd.setCursor(0, 1);
  lcd.print("distanzaUltraSuoni:");
  lcd.print("EMERGENZA \n");
  lcd.print(printTemperature());
  lcd.print(printHumidity());
=======
>>>>>>> 2ee1231d531b79d9681f3855c3fb84daf5ca6c7c
}

void setup() {

  Serial.begin(9600);       // Inizializza la comunicazione seriale a 9600 bps
  //lcd.begin(16, 2);         // Inizializza il display LCD
  //lcd.setCursor(0,0);
}

void lcdManagement(int distance,char set) {
  lcd.clear();

  switch (set)
  {
  case 'N':
    lcd.setCursor(0, 1);
    lcd.print(printDistance(distance));
    
    break;

    case 'E':
    lcd.setCursor(0, 0);
    lcd.print("EMMERGENZA");
    lcd.setCursor(0, 1);
    lcd.print(printDistance(distance));

    
    break;
  case 'T':
    lcd.setCursor(0, 0);
    lcd.print(printTemperature());
    break;
  case 'H':
  lcd.setCursor(0, 1);
    lcd.print(printHumidity());
    break;   

    default:
    lcd.print("FATAL ERROR");

  }


}

void loop() {

<<<<<<< HEAD

  distanceManagement();
  delay(30);
=======
  distanceManagement();
  
>>>>>>> 2ee1231d531b79d9681f3855c3fb84daf5ca6c7c
  // funzioni da eseguire ogni 5 minuti

  if (millis() % fiveMinutes == 0) {
<<<<<<< HEAD
 
    measureVoltmeters();
=======
    //updateLCD();
    Serial.print(printVoltage(measureVoltage()));
>>>>>>> 2ee1231d531b79d9681f3855c3fb84daf5ca6c7c
  }
  if(millis() % tenMinutes == 0) {
  Serial.print(printTemperature());
  Serial.print(printHumidity());
  char setLcd='T';
  lcdManagement(0,setLcd);

  setLcd='H';
  lcdManagement(0,setLcd);
  }
}