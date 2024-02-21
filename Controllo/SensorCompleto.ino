#include <DHT11.h>
#include <LiquidCrystal.h>
#include <NewPing.h>


//costanti globali 
#define MAX_DISTANCE 200
#define SENSORS_NOMBER 8
#define MAX_DISTANCE 200
#define SPEED_OF_SOUND 0.0343
#define EMERGENCY_PIN 1


//setup pin lcd
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);



//constanti gestione millis

const long fiveSeconds = 3000 + millis();
const long fiveMinutes = 3000 + millis();
const long tenMinutes = 6000 + millis();
long t0 = millis();




//funzione per sensore Temperatura e Umidità
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
//const int trigPinDR = 30;
//const int echoPinDR = 31; 
//posteriore sinistro
//const int trigPinDL = 32;
//const int echoPinDL = 33; 


//variabili di supporto per misura distanza
double distance = 0;
bool alarm = false;
int sensorIndex = 0;
const int sensoriMancanti = 4;

// costante di controllo per lcd
  char setLcd;

//Inizializzazione sensori
NewPing sonar[SENSORS_NOMBER-sensoriMancanti] = {   // Sensor object array.
  NewPing(trigPinU, echoPinU, MAX_DISTANCE),
  NewPing(trigPinUR, echoPinUR, MAX_DISTANCE),
  NewPing(trigPinUL, echoPinUL, MAX_DISTANCE),
  NewPing(trigPinD, echoPinD, MAX_DISTANCE),
  //NewPing(trigPinDR, echoPinDR, MAX_DISTANCE)
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
  if(alarm == true) Serial.print("E/");
  if(sensorIndex == 0) Serial.print("U");
    else if(sensorIndex == 1) Serial.print("UR");
    else if(sensorIndex == 2) Serial.print("UL");
    else if(sensorIndex == 3) Serial.print("D");
    else if(sensorIndex == 4) Serial.print("DR");
    else if(sensorIndex == 4) Serial.print("DL");
  return String("Dis: " + (String)distance +"cm ");

}

void distanceManagement() {


  if( sensorIndex < SENSORS_NOMBER -sensoriMancanti) {

    if(alarm == true) {

      //stato di emergenza
      distance = measureDistance(sensorIndex);

      Serial.print(printDistance(distance));
      setLcd ='E';
      lcdManagement(distance,setLcd); 
      

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
        Serial.print("/n EMERGENZA");
      } else {
        sensorIndex++;
      }
    }
  }

  if(alarm == false) {
    sensorIndex = 0;
    //distance = 0;
  }

  delay(50);
}

//funzioni gestione Temperatura
int measureTemperature() {
  return dht11.readTemperature();
}
String printTemperature() {
  char buffer[40];
  int tempValCalc =(int) 1.0/(log(1023.0/(float)measureTemperature()-1.0)/4275+1/298.15)-273.15;
  sprintf(buffer, "         T: %dC   \n", tempValCalc);
  return buffer;
}

//funzioni gestione Umidità
int measureHumidity() {
  return dht11.readHumidity();
}
String printHumidity() {
  char buffer[40];
  sprintf(buffer, "U: %d%%", measureHumidity());
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
  //lcd.clear();

  switch (set)
  {
  case 'N':
    lcd.setCursor(0, 1);
    lcd.print(printDistance(distance));
    
    break;

    case 'E':
    lcd.setCursor(0, 0);
    lcd.print("EMMERGENZA      ");
    lcd.setCursor(0, 1);
    lcd.print(printDistance(distance));

    
    break;
  case 'T':
    lcd.setCursor(0, 0);
    lcd.print(printTemperature());
    break;
  case 'H':
    lcd.setCursor(0, 0);
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
  

  distanceManagement();
  delay(30);

  if( alarm == false){
    if (millis() - t0 > fiveSeconds ) {
      //aggiorna e stampa LCD
      setLcd ='N';
      lcdManagement(distance,setLcd);
    }   
  }

  
  if (millis() - t0 > fiveMinutes ) {
    Serial.print(printVoltmeters());

    //aggiorna e stampa LCD
    setLcd='V';
    lcdManagement(0,setLcd);
    t0 = millis();
  }

  if(millis() - t0 >  tenMinutes) {
    Serial.print(printTemperature());
    Serial.print(printHumidity());

    //aggiorna e stampa LCD
    setLcd='T';
    lcdManagement(0,setLcd);

    setLcd='H';
    lcdManagement(0,setLcd);
    t0 = millis();
    } 
}