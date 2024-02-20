#include <NewPing.h>

//costanti globali 
#define SENSORS_NUMBER 1
#define MAX_DISTANCE 200
#define SPEED_OF_SOUND 0.0343
#define EMERGENCY_PIN 1

const int dxForward = 5, dxBackward = 4, dxForwardEn = 27, dxBackwardEn = 26;   // Motore DX, pin da modificare
const int sxForward = 7, sxBackward = 6, sxForwardEn = 22, sxBackwardEn = 23;   // Motore SX, pin da modificare
int movementInt = 0;
int speed = 0;  // Valore del PWM tra 0 (spento) e 255 (massima velocità)
int key = 40; // pin della chiave
const int speedGain = 3;
const int maxSpeed = 150;
const int minSpeed = -50;
bool emergency = false;
bool isRotating = false;
const int buttons = 2;              // pin dei bottoni

unsigned long startTime;
unsigned long currentTime;
const long interval = 1000;

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

//variabili di supporto sensori di prossimità
double distance = 0;
bool alarm = false;
int sensorIndex = 0;
const int SONAR_INTERVAL = 50;
//const int SONAR_INTERVAL = 1000;


//Array inizializzazione sensori di prossimità
NewPing sonar[SENSORS_NUMBER] = {   
  NewPing(TRIG_PIN_U, ECHO_PIN_U, MAX_DISTANCE)      //sensore frontale
  //NewPing(TRIG_PIN_UR, ECHO_PIN_UR, MAX_DISTANCE),    //sensore frontale destro
  //NewPing(TRIG_PIN_UL, ECHO_PIN_UL, MAX_DISTANCE)     //sensore frontale sinistro
  //NewPing(TRIG_PIN_D, ECHO_PIN_D, MAX_DISTANCE)       //sensore posteriore 
  //NewPing(TRIG_PIN_DR, ECHO_PIN_DR, MAX_DISTANCE)   //sensore posteriore destro
  //NewPing(TRIG_PIN_DL, ECHO_PIN_DL, MAX_DISTANCE)   //sensore posteriore sinistro
};

void setup() {
    Serial.begin(9600);
    startTime = millis();

    pinMode(dxForward, OUTPUT);
    pinMode(dxBackward, OUTPUT);
    pinMode(dxForwardEn, OUTPUT);
    pinMode(dxBackwardEn, OUTPUT);

    pinMode(sxForward, OUTPUT);
    pinMode(sxBackward, OUTPUT);
    pinMode(sxForwardEn, OUTPUT);
    pinMode(sxBackwardEn, OUTPUT);

    digitalWrite(dxForwardEn, HIGH);
    digitalWrite(sxForwardEn, HIGH);
    digitalWrite(dxBackwardEn, HIGH);
    digitalWrite(sxBackwardEn, HIGH);

    pinMode(key, INPUT_PULLUP); // necessario per far funzionare la chiave -- fare attenzione alle interferenze nel caso in cui il motore non venga messo a 0

    pinMode(buttons, INPUT);     // emergenza bottoni

    pinMode(TRIG_PIN_U, OUTPUT);
    pinMode(ECHO_PIN_U, INPUT);

    pinMode(TRIG_PIN_UR, OUTPUT);
    pinMode(ECHO_PIN_UR, INPUT);

    pinMode(TRIG_PIN_UL, OUTPUT);
    pinMode(ECHO_PIN_UL, INPUT);

    pinMode(TRIG_PIN_D, OUTPUT);
    pinMode(ECHO_PIN_D, INPUT);
}

void toDelete(){                        //DEBUG 
  Serial.println("Button Pressed");
}


void loop() {

    currentTime = millis();
    
    //if(!digitalRead(buttons))toDelete();
	// controllo della comunicazione seriale (anche gli altri arduino devono fare il controllo del seriale)
	if (emergency) {
        emergencyState();
        return;
    }

	if (speed == 0){
        movementInt = 0;
    }

    distanceManagement();
   

}

// funzione stato emergenza
void emergencyState() {
    if (!emergency){            // ferma la macchina e manda un messaggio di emergenza agli altri arduino
        emergency = true;
        emergencyStop();
    }

    while (emergency || !digitalRead(key))      		// rimane nel loop finché non viene girata la chiave o viene mandato un messaggio dall'app
    {
        if (!digitalRead(key)){
            emergency = false;
        }
    }
}

// segnale di arresto del motore (potrebbe essere non necessaria)
void emergencyStop() {
    stopMotor();
    resetVariables();
}

// reset delle variabili
void resetVariables() {
    speed = 0;
    movementInt = 0;
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
        Serial.println(speed);
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
        Serial.println(speed);
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

      delay(1000);

        if(sensorIndex < SENSORS_NUMBER) {

            if(alarm == true) {

            //stato di emergenza
            distance = measureDistance(sensorIndex);
            Serial.print(printDistance(distance));


            if (distance > 50) {
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
                Serial.print("EMERGENZA Sensore: ");
                Serial.println(sensorIndex);
            } else {
                //sensorIndex++;
            }

            }
        } else {
            sensorIndex = 0;
            distance = 0;
        }
    }

}