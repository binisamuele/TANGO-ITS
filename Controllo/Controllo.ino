#include <NewPing.h>

//costanti globali 
#define SENSORS_NOMBER 6
#define MAX_DISTANCE 200
#define SPEED_OF_SOUND 0.0343
#define EMERGENCY_PIN 1

// COSTANTI PINS
const int dxForward = 5, dxBackward = 4, dxForwardEn = 27, dxBackwardEn = 26;   // Motore DX
const int sxForward = 7, sxBackward = 6, sxForwardEn = 22, sxBackwardEn = 23;   // Motore SX

const int bumpers = 3;              // pin dei bumpers
const int buttons = 2;              // pin dei bottoni
const int arduinoEmergencies = 8;   // pin per le emergenze degli altri Arduino

const int key = 40;                 // pin della chiave
const int startFromApp;             // pin collegato all'app per l'accensione
const int emergencyPin;             // pin per inviare messaggi di emergenza
const int communicationPin;         // pin per la rispota ai messaggi

//Pin sensori prossimità
//frontale
const int TRIG_PIN_U = 32;     
const int ECHO_PIN_U = 33;     
//frontale destro
const int TRIG_PIN_UR = 34;
const int ECHO_PIN_UR = 35; 
//frontale sinistro
const int TRIG_PIN_UL = 36;  
const int ECHO_PIN_UL = 37;  

//posteriore
const int TRIG_PIN_D = 38;
const int ECHO_PIN_D = 39; 
//posteriore destro
const int TRIG_PIN_DR = 40;
const int ECHO_PIN_DR = 41; 
//posteriore sinistro
const int TRIG_PIN_DL = 42;
const int ECHO_PIN_DL = 43; 

// VARIABILI
unsigned long startTime;
unsigned long currentTime;
const long interval = 1000;

const int speedGain = 2;    // da testare
const int maxSpeed = 100;    // da testare
const int minSpeed = -50;   // da testare
int speed = 0;              // Valore del PWM tra 0 (spento) e 255 (massima velocità)
int movementInt = 0;

bool isRotating = false;
bool emergency = true;

String serialString = "";

//variabili di supporto
double distance = 0;
bool alarm = false;
int sensorIndex = 0;
const int sensoriMancanti = 2;
const int SONAR_INTERVAL = 50;


//Array inizializzazione sensori
NewPing sonar[SENSORS_NOMBER - sensoriMancanti] = {   
  NewPing(TRIG_PIN_U, ECHO_PIN_U, MAX_DISTANCE),      //sensore frontale
  NewPing(TRIG_PIN_UR, ECHO_PIN_UR, MAX_DISTANCE),    //sensore frontale destro
  NewPing(TRIG_PIN_UL, ECHO_PIN_UL, MAX_DISTANCE),    //sensore frontale sinistro
  NewPing(TRIG_PIN_D, ECHO_PIN_D, MAX_DISTANCE)       //sensore posteriore 
  //NewPing(TRIG_PIN_DR, ECHO_PIN_DR, MAX_DISTANCE)   //sensore posteriore destro
  //NewPing(TRIG_PIN_DL, ECHO_PIN_DL, MAX_DISTANCE)   //sensore posteriore sinistro
};


void setup() {
    Serial1.begin(9600);    // collegamento all'arduino di comunicazione

    startTime = millis();

    pinMode(dxForward, OUTPUT);
    pinMode(dxBackward, OUTPUT);
    pinMode(dxForwardEn, OUTPUT);
    pinMode(dxBackwardEn, OUTPUT);

    pinMode(sxForward, OUTPUT);
    pinMode(sxBackward, OUTPUT);
    pinMode(sxForwardEn, OUTPUT);
    pinMode(sxBackwardEn, OUTPUT);

    pinMode(emergencyPin, OUTPUT);
    pinMode(communicationPin, OUTPUT);

    pinMode(key, INPUT_PULLUP);

    pinMode(buttons, INPUT_PULLUP);     // emergenza bottoni
    pinMode(bumpers, INPUT_PULLUP);     // emergenza bumper
    pinMode(arduinoEmergencies, INPUT);  //emergenza arduino


    // attachInterrupt(0, emergencyState, FALLING);                            // Pin 2 per emergenza pulsanti
    // attachInterrupt(1, emergencyState, RISING);                             // Pin 3 per emergenza bumper
    // attachInterrupt(2, emergencyState, RISING);                             // Pin 21 per emergenze arduino (hardware deve utilizzare un diodo)
}


void loop() {
    currentTime = millis();

    if (!Serial1 || emergency || digitalRead(buttons) || digitalRead(bumpers) || digitalRead(arduinoEmergencies)) {
        emergencyState();
        return;
    }

    readSerial();

    movement();
}

// funzione stato emergenza
void emergencyState() {
    if (!emergency){            // ferma la macchina e manda un messaggio di emergenza agli altri arduino
        emergency = true;
        emergencyStop();
        emergencyComm();        // segnale di emergenza agli altri arduino
    }

    while (emergency || !digitalRead(key))      // rimane nel loop finché non viene girata la chiave o viene mandato un messaggio dall'app
    {
        if (!digitalRead(key) || digitalRead(startFromApp)) emergency = false;
    }

    emergencyComm();            // segnale di fine emergenza agli altri arduino
}

// funzione di comunicazioni di emergenza
void emergencyComm(){
    if (emergency){
        digitalWrite(emergencyPin, HIGH);
        return;
    }

    digitalWrite(emergencyPin, LOW);
}   

// arresto di emergenza del motore
void emergencyStop() {
    stopMotor();
    resetVariables();
}

// reset delle variabili
void resetVariables() {
    speed = 0;
    movementInt = 0;
}

// lettura dell'arduino di comunicazione
void readSerial(){
    if(Serial1.available()) {
        serialString = Serial1.readStringUntil('\r\n');
        digitalWrite(communicationPin, HIGH);
        delay(50); // da testare
        digitalWrite(communicationPin, LOW);
        mapping(serialString);
    }
}

// mapping dei messaggi
void mapping(String serialString) {
    int index = serialString.lastIndexOf(':');
    int length = serialString.length();
    String topic = serialString.substring(0, index);
    String serialVal = serialString.substring(index+1, length);

    if (topic == "movimento") {

        if (serialVal == "up"){
            movementInt = 1;
            return;
        }
        if (serialVal == "down"){
            movementInt = 2;
            return;
        }
        if (serialVal == "right"){ // ruotare a destra
            movementInt = 3;
            return;
        }
        if (serialVal == "left"){ // ruotare a sinistra
            movementInt = 4;
            return;
        }
        if (serialVal == "emergencyStop"){
            movementInt = 5;
            return;
        }
        movementInt = 0;
        return;
    }
}

// funzione con le opzioni di movimento
void movement(){
    switch (movementInt) {
        // andare avanti
        case 1:
            rotationCheck();

            if (speed < 0) {                                   // se la velocità era negativa, rallentiamo i motori
                decelerate();
                break;
            }

            accelerate();
            break;
        // andare indietro
        case 2:
            rotationCheck();

            if (speed > 0) {                                   // se la velocità era positiva, rallentiamo i motori
                decelerate();
                break;
            }

            accelerate();
            break;
        // rotazione in senso orario
        case 3:
            if (speed != 0){
                decelerate();
                break;
            }

            driveMotor(sxForward, dxBackward, 20);

            isRotating = true;
            break;
        // rotazione in senso antiorario
        case 4:
            if (speed != 0){
                decelerate();
                break;
            }

            driveMotor(dxForward, sxBackward, 20);

            isRotating = true;
            break;
        // frenata
        case 5:
            rotationCheck();

            decelerate();
            break;
        // nessun comando di movimento
        default:
            rotationCheck();

            decelerate();
            break;
    }
}

// controllo della velocità vicino a zero
void speedControl(){
    if (speed < speedGain && speed > -speedGain) speed = 0;
}

// controllo della rotazione
void rotationCheck(){
    if (isRotating){
        stopMotor();
        isRotating = false;
    }
}

// funzione per fermare i motori
void stopMotor(){
    analogWrite(dxForward, 0);
    analogWrite(dxBackward, 0);

    analogWrite(sxForward, 0);
    analogWrite(sxBackward, 0);

    currentTime = startTime;
}

// funzione per muoversi avanti o indietro
void driveMotor(int motor1, int motor2, int spd) {
    if (spd < 0) spd = -spd;

    analogWrite(motor1, spd);
    analogWrite(motor2, spd);
}

// funzione per accelerare
void accelerate(){
    if (currentTime - startTime >= interval){

        startTime = currentTime;

        if (speed >= 0) {
            if (speed >= maxSpeed) return;      // se la velocità è al massimo, non fare niente

            speed += speedGain;
            driveMotor(dxForward, sxForward, speed);
            return;
        }

        if (speed <= 0) {
            if (speed <= minSpeed) return;      // se la velocità è al minimo, non fare niente

            speed -= speedGain;
            driveMotor(dxBackward, sxBackward, speed);
            return;
        }
    }
}

// funzione per decelerare e lentamente fermarsi
void decelerate(){
    if (currentTime - startTime >= interval){

        startTime = currentTime;

        if (speed < 0) {
            speed += speedGain;
            speedControl();
            driveMotor(dxBackward, sxBackward, speed);
            return;
        }

        if (speed > 0) {
            speed -= speedGain;
            speedControl();
            driveMotor(dxForward, sxForward, speed);
            return;
        }
    }
}


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

    //ciclo non bloccante ogni 50 ms
    if (currentTime - startTime >= SONAR_INTERVAL){

        if(sensorIndex < SENSORS_NOMBER) {

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
                //digitalWrite(EMERGENCY_PIN, HIGH);
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
    }

}