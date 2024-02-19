#include <DHT11.h>
#include <LiquidCrystal.h>

//costanti globali 
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

//funzioni gestione Temperatura
int measureTemperature() {
  return dht11.readTemperature();
}
String printTemperature() {
  char buffer[40];
  int tempValCalc =(int) 1.0/(log(1023.0/(float)measureTemperature()-1.0)/4275+1/298.15)-273.15;
  sprintf(buffer, "        T : %d C", tempValCalc);
  return buffer;
}

//funzioni gestione Umidità
int measureHumidity() {
  return dht11.readHumidity();
}
String printHumidity() {
  char buffer[40];
  sprintf(buffer, "U : %d%% ", measureHumidity());
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
  
  //      voltage1/12=x/100
  voltage1 =(2*100)/(voltage1-10)

  sprintf(buffer, "Batteria:%d.%d %%", (int)voltage1, ((int)(voltage1*10) % 10));
  Serial.println(buffer);
  return buffer;
}




void setup() {

  Serial.begin(9600);       // Inizializza la comunicazione seriale a 9600 bps
  lcd.begin(16, 2);         // Inizializza il display LCD
  lcd.setCursor(0,0);
}

void lcdManagement(char set) {
  //lcd.clear();

  switch (set) {
    case 'E':
    lcd.setCursor(0, 0);
    lcd.print("EMERGENZA       ");
    lcd.setCursor(0, 1);
    lcd.print("EMERGENZA       ");    
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
  
  if( Serial.digitalWrite() == "EMERGENZA"){
      lcdManagement(E);
  }   

  if(millis() - t0 >  tenMinutes) {
    Serial.print(printVoltmeters());
    Serial.print(printTemperature());
    Serial.print(printHumidity());

    //aggiorna e stampa LCD
    lcdManagement(V);
    lcdManagement(T);
    lcdManagement(H);
    t0 = millis();
    }

  
}