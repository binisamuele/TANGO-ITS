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

    attachInterrupt(0, emergencyStop, RISING);
    attachInterrupt(1, emergencyStop, FALLING);
}

void loop() {

    readSerial();

    mapping(serial1String);
    mapping(serial2String);
    mapping(serial3String);

    switch (movementInt) {
        // andare avanti
        case 1:
            if (speed == maxSpeed) break;                       // se la velocità è al massimo, non fare niente

            speed += speedGain;

            if (speed < 10) {                                   // se la velocità era negativa, rallentiamo i motori
                reverseMotor(dxBackward, sxBackward, speed);
                break;
            }

            driveMotor(dxForward, sxForward, speed);
            break;
        // andare indietro
        case 2:
            if (speed == minSpeed) break;                       // se la velocità è al minimo, non fare niente

            speed -= speedGain;

            if (speed > -10) {                                   // se la velocità era positiva, rallentiamo i motori
                driveMotor(dxForward, sxForward, speed);
                break;
            }

            reverseMotor(dxBackward, sxBackward, speed);
            break;
        // curvare destra
        case 3:
            halfMotor(sxForward);
            driveMotor(dxForward);
            break;
        // curvare sinistra
        case 4:   
            halfMotor(dxForward);
            driveMotor(sxForward);
            break;
        // rotazione in senso orario
        case 5:
            if (speed > 0) {
                speed -= speedGain;
                driveMotor(dxForward, sxForward, speed);
                break;
            }
            
            if (speed < 0) {
                speed += speedGain;
                driveMotor(dxBackward, sxBackward, speed);
                break;
            }

            driveMotor(sxForward, dxBackward, 25);

            break;
        // rotazione in senso antiorario
        case 6:
            // il caso sopra al contrario
            break;

        case 7:
            decelerate();
            break;

        default:
            // nessun pulsante premuto
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

        if (serialVal == "forward"){
            movementInt = 1;
            return;
        }
        if (serialVal == "backward"){
            movementInt = 2;
            return;
        }
        if (serialVal == "right"){
            movementInt = 3;
            return;
        }
        if (serialVal == "left"){
            movementInt = 4;
            return;
        }
        if (serialVal == "rotateDX"){
            movementInt = 5;
            return;
        }
        if (serialVal == "rotateSX"){
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

    if (topic == "emergenza"){
        emergencyStop();   
        return;
    }
    
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

    delay(1000);
}

// Funzione per muoversi avanti o indietro
void driveMotor(int motor1, int motor2, int spd) {
    if (spd < 0) spd = -spd;
    analogWrite(motor1, spd);
    analogWrite(motor2, spd);

    delay(50);
}

/* Funzione per andare indietro
void reverseMotor(int motor1, int motor2, int spd) {    
    int reverseSpeed = -spd;

    driveMotor(motor1, motor2, reverseSpeed);
}*/

// Funzione per girare
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
    if (speed > 0) {
        speed = speed - speedGain;
        if (speed == 0){
            emergencyStop();
            return;
        }
        driveMotor(dxForward);
        driveMotor(sxForward);
        return;
    }
    if (speed < 0) {
        speed = speed + speedGain;
        if (speed == 0){
            emergencyStop();
            return;
        }
        reverseMotor(dxBackward);
        reverseMotor(sxBackward);
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