// COSTANTI PINS
const int DX_FORWARD = 5, DX_BACKWARD = 4, DX_FORWARD_EN = 27, DX_BACKWARD_EN = 26;   // Motore DX
const int SX_FORWARD = 7, SX_BACKWARD = 6, SX_FORWARD_EN = 22, SX_BACKWARD_EN = 23;   // Motore SX

const int BUMPER1 = 52;                  // pin dei bumpers
const int BUMPER2 = 53;                  // pin dei bumpers
const int BUTTONS = 2;                  // pin dei bottoni
const int EMERGENCY_PIN = 12;           // pin di emergenza
const int ARDUINO_EMERGENCIES = 13;     // pin per le emergenze degli altri Arduino

const int KEY = 9;                      // pin della chiave

// COSTANTI
const int SENSORS_NUMBER = 6;
const int MAX_DISTANCE = 200;
const int INTERVAL = 250;
const int SONAR_INTERVAL = 50;

const int MAX_SPEED = 150;
const int MIN_SPEED = -100;
const int SPEED_GAIN = 10;
const int LOW_SPEED = 50;   //da testare

const int TANGO_SIZE = 20;
const int EMERGENCY_DISTANCE = 40;
const int SECURITY_DISTANCE = 60;
const float SPEED_OF_SOUND = 0.0343;

// VARIABILI
unsigned long startTime;
unsigned long currentTime;
unsigned long commTime;

int speed = 0;
int movementInt = 0;
int brakingTime = 0;
int sensorIndex = 0;

bool emergency = true;
bool forwardDir = true;
bool isRotating = false;

String serialString = "";

void setup() {
    Serial.begin(9600);
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

	  digitalWrite(DX_FORWARD_EN, HIGH);
    digitalWrite(SX_FORWARD_EN, HIGH);
    digitalWrite(DX_BACKWARD_EN, HIGH);
    digitalWrite(SX_BACKWARD_EN, HIGH);

    pinMode(EMERGENCY_PIN, OUTPUT);

    pinMode(KEY, INPUT_PULLUP);

    pinMode(BUTTONS, INPUT_PULLUP);         // emergenza bottoni
    pinMode(BUMPER1, INPUT_PULLUP);         // emergenza bumper
    pinMode(BUMPER2, INPUT_PULLUP);         // emergenza bumper
    pinMode(ARDUINO_EMERGENCIES, INPUT);    // emergenza arduino
}

void loop() {
    currentTime = millis();

    if (!Serial1 || digitalRead(ARDUINO_EMERGENCIES)) {
        Serial.println("Emergenza");
        emergencyState();
        return;
    }

	  readSerial();

    /*
    if (currentTime - startTime >= INTERVAL){
        Serial.println(digitalRead(ARDUINO_EMERGENCIES));
        startTime = currentTime;
    }
    */

	  movement();
}

// GESTIONE EMERGENZA           
///////////////////////////////////////////////////////////////////////////////
// funzione stato emergenza
void emergencyState() {
    if (!emergency){            // ferma la macchina e manda un messaggio di emergenza agli altri arduino
        emergency = true;
        emergencyStop();
    }

    int start = 0;
    while (emergency)      // rimane nel loop finché non viene girata la chiave o viene mandato un messaggio dall'app
    {
        start = Serial.parseInt(); // DEBUG per far ripartire il codice scrivendo 9 nel monitor serial
        if (start == 9) emergency = false;
    }
    Serial.println("Fine Emergenza");
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

    speed = 0;
    movementInt = 0;
}

// GESTIONE COMUNICAZIONE          
///////////////////////////////////////////////////////////////////////////////
// lettura dell'arduino di comunicazione
void readSerial(){
    if (Serial1.available()) {
        serialString = Serial1.readStringUntil('\r\n');
        mapping(serialString);
    }
}

// mapping dei messaggi
void mapping(String serialString) {
    if (serialString == "up"){
        movementInt = 1;
        return;
    }
    if (serialString == "down"){
        movementInt = 2;
        return;
    }
    if (serialString == "right"){ // ruotare a destra
        movementInt = 3;
        return;
    }
    if (serialString == "left"){ // ruotare a sinistra
        movementInt = 4;
        return;
    }
    if (serialString == "brake"){
        movementInt = 5;
        return;
    }
    movementInt = 0;
}

// FUNZIONI DI MOVIMENTO          
///////////////////////////////////////////////////////////////////////////////
// funzione con le opzioni di movimento
void movement(){
    switch (movementInt) {
        // andare avanti
        case 1:
            rotationCheck();
            forwardDir = true;

            if (speed < 0) {                                   // se la velocità era negativa, rallentiamo i motori
                brakingTime = 5;
                brake();
                break;
            }

            accelerate();
            break;
        // andare indietro
        case 2:
            rotationCheck();
            forwardDir = false;

            if (speed > 0) {                                   // se la velocità era positiva, rallentiamo i motori
                brakingTime = 5;
                brake();
                break;
            }

            accelerate();
            break;
        // rotazione in senso orario
        case 3:
            if (speed != 0){
                brakingTime = 5;
                brake();
                break;
            }

            driveMotor(SX_FORWARD, DX_BACKWARD, LOW_SPEED);

            isRotating = true;
            break;
        // rotazione in senso antiorario
        case 4:
            if (speed != 0){
                brakingTime = 5;
                brake();
                break;
            }

            driveMotor(DX_FORWARD, SX_BACKWARD, LOW_SPEED);

            isRotating = true;
            break;
        // frenata
        case 5:
            rotationCheck();
            brakingTime = 5;

            brake();
            break;
        // nessun comando di movimento
        default:
            rotationCheck();
            brakingTime = 1;

            brake();
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

        if (forwardDir) {
            if (speed >= MAX_SPEED) return;      // se la velocità è al massimo, non fare niente

            speed += SPEED_GAIN;
            if (speed > 30) speed += SPEED_GAIN;
            if (speed > 80) speed += SPEED_GAIN;
            if (speed > MAX_SPEED) speed -= SPEED_GAIN;

            driveMotor(DX_FORWARD, SX_FORWARD, speed);
            return;
        }

        if (!forwardDir) {
            if (speed <= MIN_SPEED) return;      // se la velocità è al minimo, non fare niente

            speed -= SPEED_GAIN;
            if(speed < -30) speed -= SPEED_GAIN;
            if(speed < MIN_SPEED) speed += SPEED_GAIN;

            driveMotor(DX_BACKWARD, SX_BACKWARD, speed);
            return;
        }
    }
}

// funzione per decelerare e lentamente fermarsi
void brake(){
    if (currentTime - startTime >= INTERVAL / brakingTime){

        startTime = currentTime;

        if (speed < 0) {
            speed += SPEED_GAIN;
            speedControl();
            driveMotor(DX_BACKWARD, SX_BACKWARD, speed);
            return;
        }

        if (speed > 0) {
            speed -= SPEED_GAIN;
            speedControl();
            driveMotor(DX_FORWARD, SX_FORWARD, speed);
            return;
        }
    }
}