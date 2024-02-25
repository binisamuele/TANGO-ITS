#include <DHT11.h>
#include <LiquidCrystal.h>



//setup pin lcd
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

//constanti gestione millis

const long twoMinutes = 2200;
bool inizio = true; 
long t0 = millis();

//  da passare alla comunicazione
char umidita[40];
char temperatura[40];
char batteria[40];


//funzione per sensore Temperatura e Umidità
DHT11 dht11(50);

//pin sensori voltimetri
const int voltmeter1Pin = A8;

//funzioni gestione Temperatura
int measureTemperature() {
  return dht11.readTemperature();
}
String printTemperature() {
  char buffer[40];
  int tempValCalc =(int) measureTemperature();
  sprintf(buffer, "        T : %d C", tempValCalc);
  sprintf(temperatura, "Temperatura:%d", tempValCalc);
  return buffer;
}

//funzioni gestione Umidità
int measureHumidity() {
  return dht11.readHumidity();
}
String printHumidity() {
  char buffer[40];
  int umid=measureHumidity();
  sprintf(buffer, "U : %d%%", umid);
  sprintf(umidita, "Umidita:%d", umid);
  return buffer;
}


//funzioni monitoraggio stato della batteria
float measureVoltmeters() {
  // Misura tensione da voltmeter1Pin
   
  float letturaValore= analogRead(voltmeter1Pin);
  // lettura/1023=x/5
  return letturaValore*5/1023;

}

String printVoltmeters() {
  char buffer[40];
  float voltage1=measureVoltmeters();
  
  //      voltage1/12=x/100
  float test = voltage1; 
  float voltCarico=28;
  float voltScarico=23.8;
  float delta= voltCarico-voltScarico;
  voltage1 = ((voltage1-voltScarico)*100)/delta;

  if(voltage1<0) voltage1=0;

  sprintf(buffer, "Batteria:%d.%d %%     ", (int)voltage1, ((int)(voltage1*10) % 10));
  sprintf(batteria, "Batteria:%d misura:%d.%d", (int)voltage1,(int)test, ((int)(test*1000) % 1000));
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
  
 if( Serial.read() == "EMERGENZA"){
    lcdManagement('E');
  }   

  if(millis() - t0 >  twoMinutes) {
    //aggiorna e stampa LCD
    lcdManagement('V');
    lcdManagement('T');
    lcdManagement('H');
    // manda in seriale
    Serial.println(temperatura);
    Serial.println(umidita);
    Serial.println(batteria);

    t0 = millis();
    }

  if(inizio){
    inizio= false;
    lcdManagement('V');
    lcdManagement('T');
    lcdManagement('H');

    Serial.println(temperatura);
    Serial.println(umidita);
    Serial.println(batteria);

  }

}
