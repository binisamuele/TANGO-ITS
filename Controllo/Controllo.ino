String serial1String = "";
String serial2String = "";
String serial3String = "";
int movementInt;
int dxForward = 2, dxBackward = 3, dxForwardEn = 9, dxBackwardEn = 10; // Motore DX
int sxForward = 4, sxBackward = 5, sxForwardEn = 11, sxBackwardEn = 12;  // Motore SX
int speed = 0;  // Valore del PWM tra 0 (spento) e 255 (massima velocità)
int speedGain = 10;
const int maxSpeed = 150;
const int minSpeed = -100;
float lidarDistance;
float signalStrength;
float batteryCharge;
float ultrasoundDistance;
float temperature;
float humidity;

void setup() {
    Serial1.begin(9600);    // collegamento all'arduino di comunicazione
    Serial2.begin(9600);    // collegamento all'arduino del LIDAR
    Serial3.begin(9600);    // collegamento all'arduino dei sensori

    pinMode(dxForward, OUTPUT);
    pinMode(dxBackward, OUTPUT);
    pinMode(dxForwardEn, OUTPUT);
    pinMode(dxBackwardEn, OUTPUT);

    pinMode(sxForward, OUTPUT);
    pinMode(sxBackward, OUTPUT);
    pinMode(sxForwardEn, OUTPUT);
    pinMode(sxBackwardEn, OUTPUT);

    attachInterrupt(0, emergencyStop, FALLING); // Pin 2 per emergenza pulsanti
    attachInterrupt(1, emergencyStop, RISING); // Pin 3 per emergenza bumper
    attachInterrupt(2, emergencyStop, RISING); // Pin 20 per emergenze arduino (hardware deve utilizzare un diodo)
}

void loop() {

    // controllo della comunicazione seriale (anche gli altri arduino devono fare il controllo del seriale)
    if (!Serial1 && !Serial2 && !Serial3) emergencyStop();
    
    readSerial();

    mapping(serial1String);
    mapping(serial2String);
    mapping(serial3String);

    switch (movementInt) {
        // andare avanti
        case 1:
            if (speed == maxSpeed) break;                       // se la velocità è al massimo, non fare niente

            speed += speedGain;
            speedControl();

            if (speed < 10) {                                   // se la velocità era negativa, rallentiamo i motori
                driveMotor(dxBackward, sxBackward, speed);
                break;
            }

            driveMotor(dxForward, sxForward, speed);
            break;
        // andare indietro
        case 2:
            if (speed == minSpeed) break;                       // se la velocità è al minimo, non fare niente

            speed -= speedGain;
            speedControl();

            if (speed > -10) {                                   // se la velocità era positiva, rallentiamo i motori
                driveMotor(dxForward, sxForward, speed);
                break;
            }

            driveMotor(dxBackward, sxBackward, speed);
            break;
        // curvare destra da fare
        case 3:
            halfMotor(sxForward);
            driveMotor(dxForward);
            break;
        // curvare sinistra da fare
        case 4:   
            halfMotor(dxForward);
            driveMotor(sxForward);
            break;
        // rotazione in senso orario
        case 5:
            if (speed != 0){
                decelerate();
                break;
            }
            driveMotor(sxForward, dxBackward, 20);
            break;
        // rotazione in senso antiorario
        case 6:
            if (speed != 0){
                decelerate();
                break;
            }
            driveMotor(dxForward, sxBackward, 20);
            break;

        // frenata
        case 7:
            // funzione per frenare
            break;

        default:
            decelerate();
            break;
            // controllo se la macchina stava curvando
    }
}

void readSerial(){
    if(Serial1.available()) serial1String = Serial1.readStringUntil('\r\n');
    if(Serial2.available()) serial2String = Serial2.readStringUntil('\r\n');
    if(Serial3.available()) serial3String = Serial3.readStringUntil('\r\n');
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
        if (serialVal == ""){ // curvare a destra
            movementInt = 3;
            return;
        }
        if (serialVal == ""){ // curvare a sinistra
            movementInt = 4;
            return;
        }
        if (serialVal == "right"){
            movementInt = 5;
            return;
        }
        if (serialVal == "left"){
            movementInt = 6;
            return;
        }
        if (serialVal == "stop"){
            movementInt = 7;
            return;
        }
        movementInt = 0;
        return;
    }
    
    // trovare motivazione per queste variabili
    if (topic == "distanzaLidar"){
        lidarDistance = serialVal.toFloat();
        return;
    }
    if (topic == "potenzaSegnale"){
        signalStrength = serialVal.toFloat();
        return;
    }
    if (topic == "caricaBatteria"){
        batteryCharge = serialVal.toFloat();
        return;
    }
    if (topic == "distanzaUltraSuoni"){
        ultrasoundDistance = serialVal.toFloat();
        return;
    }
    if (topic == "temperatura"){
        temperature = serialVal.toFloat();
        return;
    }
    if (topic == "umidita"){
        humidity = serialVal.toFloat();
        return;
    }
}

// segnale di arresto del motore
void emergencyStop() {
    digitalWrite(dxForwardEn, LOW);
    digitalWrite(dxBackwardEn, LOW);
    analogWrite(dxForward, 0);
    analogWrite(dxBackward, 0);

    digitalWrite(sxForwardEn, LOW);
    digitalWrite(sxBackwardEn, LOW);
    analogWrite(sxForward, 0);
    analogWrite(sxBackward, 0);

    reset();

    delay(1000);
}

// reset delle variabili
void reset() {
    speed = 0;
    movementInt = 0;
}

// funzione di emergenza gestita
    // mandare messaggi agli altri arduino di risolta emergenza
    // controllo della comunicazione seriale 

// funzione di spegnimento

// Funzione per muoversi avanti o indietro
void driveMotor(int motor1, int motor2, int spd) {
    if (spd < 0) spd = -spd;
    analogWrite(motor1, spd);
    analogWrite(motor2, spd);

    delay(50);
}

// controllo della velocità vicino a zero
void speedControl(){
    if (speed < 10 && speed > -10) speed = 0;

    delay (1000);
}

// Funzione per girare DA FARE
void halfMotor(int motorForward) {
    int newSpeed = 0;

    if (speed > 0) newSpeed = speed - speedGain;

    if (motorForward == dxForward){
        analogWrite(dxForwardEn, newSpeed);
        return;
    }
    if (motorForward == sxForward){
        analogWrite(sxForwardEn, newSpeed);
        return;
    }
}

// Funzione per decelerare
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

// Funzione per la comunicazione in seriale
void serialCommunications(){
    Serial1.println("Distanza Lidar :" + String(lidarDistance));
    Serial1.println("Potenza Segnale :" + String(signalStrength));
    Serial1.println("Carica Batteria :" + String(batteryCharge));
    Serial1.println("Distanza Ultrasuoni :" + String(ultrasoundDistance));
    Serial1.println("Temperatura :" + String(temperature));
    Serial1.println("Umidita :" + String(humidity));
}   