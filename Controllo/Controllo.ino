const int dxForward = 2, dxBackward = 3, dxForwardEn = 26, dxBackwardEn = 27;   // Motore DX, pin da modificare
const int sxForward = 4, sxBackward = 5, sxForwardEn = 23, sxBackwardEn = 22;   // Motore SX, pin da modificare
const int startFromApp;     // pin collegato all'app per l'accensione
const int key = 20;         // pin della chiave
const int emergencyPin1; emergencyPin2; emergencyPin3;      // pin per inviare messaggi di emergenza
const int communicationPin1; communicationPin2; communicationPin3;      // pin per la rispota ai messaggi
String serialString = "";
int movementInt = 0;
int speed = 0;  // Valore del PWM tra 0 (spento) e 255 (massima velocità)
const int speedGain = 10;
const int maxSpeed = 150;
const int minSpeed = -100;
bool isRotating = false;
bool emergency = true;

void setup() {
    Serial1.begin(9600);    // collegamento all'arduino di comunicazione
    Serial2.begin(9600);    // collegamento all'arduino dei sensori di movimento
    Serial3.begin(9600);    // collegamento all'arduino degli altri sensori

    pinMode(dxForward, OUTPUT);
    pinMode(dxBackward, OUTPUT);
    pinMode(dxForwardEn, OUTPUT);
    pinMode(dxBackwardEn, OUTPUT);

    pinMode(sxForward, OUTPUT);
    pinMode(sxBackward, OUTPUT);
    pinMode(sxForwardEn, OUTPUT);
    pinMode(sxBackwardEn, OUTPUT);

    pinMode(emergencyPin1, OUTPUT);
    pinMode(emergencyPin2, OUTPUT);
    pinMode(emergencyPin3, OUTPUT);

    pinMode(communicationPin1, OUTPUT);
    pinMode(communicationPin2, OUTPUT);
    pinMode(communicationPin3, OUTPUT);

    pinMode(key, INPUT_PULLUP); // necessario per far funzionare la chiave

    pinMode(2, INPUT_PULLUP); // necessario per fare funzionare i bottoni

    attachInterrupt(digitalPinToInterrupt(2), emergencyState, FALLING);     // Pin 2 per emergenza pulsanti
    attachInterrupt(1, emergencyState, RISING);                             // Pin 3 per emergenza bumper
    attachInterrupt(2, emergencyState, RISING);                             // Pin 21 per emergenze arduino (hardware deve utilizzare un diodo)
}

void loop() {
    // controllo della comunicazione seriale (anche gli altri arduino devono fare il controllo del seriale)
    if (!Serial1 || !Serial2 || !Serial3 || emergency) {
        emergencyState();
        return;
    }

    readSerial();

    movement(); // switch del movimento
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
        digitalWrite(emergencyPin1, HIGH);     // manda segnale di emergenza agli altri arduino
        digitalWrite(emergencyPin2, HIGH);
        digitalWrite(emergencyPin3, HIGH);
        return;
    }

    digitalWrite(emergencyPin1, LOW);     // manda segnale di fine emergenza agli altri arduino
    digitalWrite(emergencyPin2, LOW);
    digitalWrite(emergencyPin3, LOW);
}   

// segnale di arresto del motore (potrebbe essere non necessaria)
void emergencyStop() {
    stopMotor();
    resetVariables();

    delay(200);
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
        digitalWrite(communicationPin1, HIGH);
        delay(50); // da testare
        digitalWrite(communicationPin1, LOW);
        mapping(serialString);
    }
    // if(Serial2.available()) {
    //     serialString = Serial2.readStringUntil('\r\n');
    //     digitalWrite(communicationPin2, HIGH);
    //     delay(100);
    //     digitalWrite(communicationPin2, LOW);
    //     mapping(serialString);
    // }
    // if(Serial3.available()) {
    //     serialString = Serial3.readStringUntil('\r\n');
    //     digitalWrite(communicationPin3, HIGH);
    //     delay(100);
    //     digitalWrite(communicationPin3, LOW);
    //     mapping(serialString);
    // }
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
        
        delay(50); // da testare
    }
}

// funzione per fermare i motori
void stopMotor(){
    analogWrite(dxForward, 0);
    analogWrite(dxBackward, 0);

    analogWrite(sxForward, 0);
    analogWrite(sxBackward, 0);

    delay(50); // da testare
}

// funzione per muoversi avanti o indietro
void driveMotor(int motor1, int motor2, int spd) {
    if (spd < 0) spd = -spd;

    analogWrite(motor1, spd);
    analogWrite(motor2, spd);

    delay(50); // da testare
}

// funzione per accelerare
void accelerate(){
    if (speed > 0) {
        if (speed >= maxSpeed) return;      // se la velocità è al massimo, non fare niente

        speed += speedGain;
        driveMotor(dxForward, sxForward, speed);
        return;
    }

    if (speed < 0) {
        if (speed <= minSpeed) return;      // se la velocità è al minimo, non fare niente

        speed -= speedGain;
        driveMotor(dxBackward, sxBackward, speed);
        return;
    }
}

// funzione per decelerare e lentamente fermarsi
void decelerate(){
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