// PINS
const int dxForward = 5, dxBackward = 4, dxForwardEn = 27, dxBackwardEn = 26;   // Motore DX
const int sxForward = 7, sxBackward = 6, sxForwardEn = 22, sxBackwardEn = 23;   // Motore SX

const int bumpers = 3;              // pin dei bumpers
const int buttons = 2;              // pin dei bottoni
const int arduinoEmergencies = 8;   // pin per le emergenze degli altri Arduino

const int key = 40;                 // pin della chiave
const int startFromApp;             // pin collegato all'app per l'accensione
const int emergencyPin;             // pin per inviare messaggi di emergenza
const int communicationPin;         // pin per la rispota ai messaggi

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

// lettura dei 3 arduino
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