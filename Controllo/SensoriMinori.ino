#include <DHT11.h>
#include <LiquidCrystal.h>
#include <NewPing.h>


//costanti globali 



//setup pin lcd
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

//constanti gestione millis
const int fiveMinutes = 300000;
const int tenMinutes = 600000;


//funzione per sensore Temperatura e Umidità
DHT11 dht11(2);







//pin sensori voltimetri
const int voltmeter1Pin = A0;
const int voltmeter2Pin = A1;



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