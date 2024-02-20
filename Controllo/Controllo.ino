#include <NewPing.h>

//costanti globali 
#define SENSORS_NUMBER 6
#define MAX_DISTANCE 200
#define SPEED_OF_SOUND 0.0343

// COSTANTI PINS
const int DX_FORWARD = 5, DX_BACKWARD = 4, DX_FORWARD_EN = 27, DX_BACKWARD_EN = 26;   // Motore DX
const int SX_FORWARD = 7, SX_BACKWARD = 6, SX_FORWARD_EN = 22, SX_BACKWARD_EN = 23;   // Motore SX

const int BUMPERS = 3;                  // pin dei bumpers
const int BUTTONS = 2;                  // pin dei bottoni
const int EMERGENCY_PIN;                // pin di emergenza
const int ARDUINO_EMERGENCIES = 8;      // pin per le emergenze degli altri Arduino

const int KEY = 9;                      // pin della chiave
const int START_FROM_APP;               // pin collegato all'app per l'accensione
const int COMMUNICATION_PIN;            // pin per la rispota ai messaggi

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
const long INTERVAL = 1000;

const int SPEED_GAIN = 2;       // da testare
const int MAX_SPEED = 100;      // da testare
const int MIN_SPEED = -50;      // da testare
int speed = 0;                  // Valore del PWM tra 0 (spento) e 255 (massima velocità)
int movementInt = 0;

bool isRotating = false;
bool emergency = true;

String serialString = "";

//variabili di supporto sensori di prossimità
double distance = 0;
int sensorIndex = 0;
const int sensoriMancanti = 2;
const int SONAR_INTERVAL = 50;


//Array inizializzazione sensori di prossimità
NewPing sonar[SENSORS_NUMBER - sensoriMancanti] = {   
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

    pinMode(DX_FORWARD, OUTPUT);
    pinMode(DX_BACKWARD, OUTPUT);
    pinMode(DX_FORWARD_EN, OUTPUT);
    pinMode(DX_BACKWARD_EN, OUTPUT);

    pinMode(SX_FORWARD, OUTPUT);
    pinMode(SX_BACKWARD, OUTPUT);
    pinMode(SX_FORWARD_EN, OUTPUT);
    pinMode(SX_BACKWARD_EN, OUTPUT);

    pinMode(EMERGENCY_PIN, OUTPUT);
    pinMode(COMMUNICATION_PIN, OUTPUT);

    pinMode(KEY, INPUT_PULLUP);

    pinMode(BUTTONS, INPUT);     // emergenza bottoni
    pinMode(BUMPERS, INPUT_PULLUP);     // emergenza bumper
    pinMode(ARDUINO_EMERGENCIES, INPUT);  //emergenza arduino

    pinMode(TRIG_PIN_U, OUTPUT);
    pinMode(ECHO_PIN_U, INPUT);

    pinMode(TRIG_PIN_UR, OUTPUT);
    pinMode(ECHO_PIN_UR, INPUT);

    pinMode(TRIG_PIN_UL, OUTPUT);
    pinMode(ECHO_PIN_UL, INPUT);

    pinMode(TRIG_PIN_D, OUTPUT);
    pinMode(ECHO_PIN_D, INPUT);
}


void loop() {
    currentTime = millis();

    if (!Serial1 || emergency || digitalRead(BUTTONS) || digitalRead(BUMPERS) || digitalRead(ARDUINO_EMERGENCIES) || digitalRead(KEY)) {
        emergencyState();
        return;
    }

    readSerial();
    distanceManagement();
    movement();
}

// funzione stato emergenza
void emergencyState() {
    if (!emergency){            // ferma la macchina e manda un messaggio di emergenza agli altri arduino
        emergency = true;
        emergencyStop();
        emergencyComm();        // segnale di emergenza agli altri arduino
    }

    while (emergency || !digitalRead(KEY))      // rimane nel loop finché non viene girata la chiave o viene mandato un messaggio dall'app
    {
        if (!digitalRead(KEY) || digitalRead(START_FROM_APP)) emergency = false;
    }

    emergencyComm();            // segnale di fine emergenza agli altri arduino
}

// funzione di comunicazioni di emergenza
void emergencyComm(){
    if (emergency){
        digitalWrite(EMERGENCY_PIN, HIGH);
        return;
    }

    digitalWrite(EMERGENCY_PIN, LOW);
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
        digitalWrite(COMMUNICATION_PIN, HIGH);
        delay(50); // da testare
        digitalWrite(COMMUNICATION_PIN, LOW);
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

            driveMotor(SX_FORWARD, DX_BACKWARD, 20);

            isRotating = true;
            break;
        // rotazione in senso antiorario
        case 4:
            if (speed != 0){
                decelerate();
                break;
            }

            driveMotor(DX_FORWARD, SX_BACKWARD, 20);

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
    if (speed < SPEED_GAIN && speed > -SPEED_GAIN) speed = 0;
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
    analogWrite(DX_FORWARD, 0);
    analogWrite(DX_BACKWARD, 0);

    analogWrite(SX_FORWARD, 0);
    analogWrite(SX_BACKWARD, 0);

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
    if (currentTime - startTime >= INTERVAL){

        startTime = currentTime;

        if (speed >= 0) {
            if (speed >= MAX_SPEED) return;      // se la velocità è al massimo, non fare niente

            speed += SPEED_GAIN;
            driveMotor(DX_FORWARD, SX_FORWARD, speed);
            return;
        }

        if (speed <= 0) {
            if (speed <= MIN_SPEED) return;      // se la velocità è al minimo, non fare niente

            speed -= SPEED_GAIN;
            driveMotor(DX_BACKWARD, SX_BACKWARD, speed);
            return;
        }
    }
}

// funzione per decelerare e lentamente fermarsi
void decelerate(){
    if (currentTime - startTime >= INTERVAL){

        startTime = currentTime;

        if (speed < 0) {
            if(speed < -60) speed += SPEED_GAIN*5;
            else speed += SPEED_GAIN;
            speedControl();
            driveMotor(DX_BACKWARD, SX_BACKWARD, speed);
            return;
        }

        if (speed > 0) {
            if(speed > 60) speed -= SPEED_GAIN*5;
            else speed -= SPEED_GAIN;
            speedControl();
            driveMotor(DX_FORWARD, SX_FORWARD, speed);
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

        if(sensorIndex < SENSORS_NUMBER) {

            if(emergency == true) {

                //stato di emergenza
                distance = measureDistance(sensorIndex);
                Serial.print(printDistance(distance));

                if (distance > 60) {
                    emergency = false;
                    Serial.print("FINE EMERGENZA \n");
                }

            } else {

                //stato normale
                distance = measureDistance(sensorIndex);
                Serial.print(printDistance(distance));

                if(distance < 40) {
                    //digitalWrite(EMERGENCY_PIN, HIGH);
                    emergency = true;
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