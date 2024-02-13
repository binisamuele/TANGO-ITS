#include <DHT11.h>
#include <LiquidCrystal.h>
#include <NewPing.h>


//costanti globali 
#define SENSORS_NOMBER 4
#define MAX_DISTANCE 200
#define SENSORS_NOMBER 6
#define MAX_DISTANCE 200
#define SPEED_OF_SOUND 0.0343
#define EMERGENCY_PIN 1


//setup pin lcd
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

//constanti gestione millis
const int fiveMinutes = 300000;
const int tenMinutes = 600000;


//funzione per sensore Temperatura e Umidità
DHT11 dht11(2);


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


//pin sensori voltimetri
const int voltmeter1Pin = A0;
const int voltmeter2Pin = A1;


//funzioni per la gestione della distanza
double measureDistance(int sonarNum) {

  return (sonar[sonarNum].ping() / 2) * SPEED_OF_SOUND;

}
String printDistance(double distance) { 
  Serial.print("\n");
  
  return  String("Dist: " + (String)distance +"cm ");

}

void distanceManagement() {
  char setLcd;

  if( sensorIndex < SENSORS_NOMBER) {

    if(alarm == true) {

      //stato di emergenza
      distance = measureDistance(sensorIndex);

      Serial.print(printDistance(distance));
      Serial.print(" E");
      setLcd ='E';
      lcdManagement(distance,setLcd);  //lcd.print(printDistance(distance));
      

      if (distance > 30) {
        alarm = false;
        Serial.print("FINE EMERGENZA \n");

      }

    } else {

      //stato normale
      distance = measureDistance(sensorIndex);

      Serial.print(printDistance(distance));
      setLcd ='N';
      lcdManagement(distance,setLcd);
      


      if(distance < 20) {

        digitalWrite(EMERGENCY_PIN, HIGH);
        alarm = true;
        Serial.print("/n EMERGENZA");
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
   
  return analogRead(voltmeter1Pin) /40.92;
}

String printVoltmeters() {
  char buffer[40];
  float voltage1=measureVoltmeters();
  sprintf(buffer, "voltaggio: %d.%d", (int)voltage1, ((int)(voltage1*10) % 10));
  Serial.println(buffer);
  return buffer;
}




void setup() {

  Serial.begin(9600);       // Inizializza la comunicazione seriale a 9600 bps
  lcd.begin(16, 2);         // Inizializza il display LCD
  lcd.setCursor(0,0);
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
  case 'V':
    lcd.setCursor(0, 1);
    lcd.print(printVoltmeters());
    break;   

  default:
    lcd.print("FATAL ERROR");

  }
}

void loop() {
  char setLcd;

  distanceManagement();
  delay(30);
  // funzioni da eseguire ogni 5 minuti

  if (millis() % fiveMinutes == 0) {
    Serial.print(printVoltmeters());
    setLcd='V';
    lcdManagement(0,setLcd);
  }
  
  if(millis() % tenMinutes == 0) {
  Serial.print(printTemperature());
  Serial.print(printHumidity());
  setLcd='T';
  lcdManagement(0,setLcd);

  setLcd='H';
  lcdManagement(0,setLcd);
  }
}