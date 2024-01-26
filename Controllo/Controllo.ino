String serial1String = "";
String serial2String = "";
String serial3String = "";
int movementInt;
int dxForward = 2, dxBackward = 3, dxForwardEn = 9, dxBackwardEn = 10; // Motore DX
int sxForward = 4, sxBackward = 5, sxForwardEn = 11, sxBackwardEn = 12;  // Motore SX
int speed = 0;  // Valore del PWM tra 0 (spento) e 255 (massima velocità)
int speedGain = 25;
const int maxSpeed = 150;
const int minSpeed = -100;

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

    attachInterrupt(0,emergencyStop,RISING);
    attachInterrupt(1,emergencyStop,FALLING);
}

void loop() {

            mapping();

            switch (movementInt) {
                case 1:
                    if (speed < 0){
                        emergencyStop();
                        break;
                    }
                    if (speed == maxSpeed) break;

                    speed = speed + speedGain;
                    driveMotor(dxForward);
                    driveMotor(sxForward);
                    break;

                case 2:
                    if (speed > 0){
                        emergencyStop();
                        break;
                    }
                    if (speed == minSpeed) break;

                    speed = speed - speedGain;
                    reverseMotor(dxBackward);
                    reverseMotor(sxBackward);
                    break;

                case 3:                         // curvare destra
                    halfMotor(sxForward);
                    driveMotor(dxForward);
                    break;
                        
                case 4:                         // curvare sinistra                    
                    halfMotor(dxForward);
                    driveMotor(sxForward);
                    break;

                case 5:                         // rotazione in senso orario
                    reverseMotor(dxBackward);
                    driveMotor(sxForward);
                    break;

                case 6:                         // rotazione in senso antiorario
                    driveMotor(dxForward);
                    reverseMotor(sxBackward);
                    break;

                case 7:
                    decelerate();
                    break;
            }
    } 

// mapping dei messaggi
void mapping() {
    if(Serial1.available)serialString = Serial1.readStringUntil('\r\n');
    else if(Serial2.available)serialString = Serial2.readStringUntil('\r\n');
    else if(Serial3.available) serialString = Serial3.readStringUntil('\r\n');
    int index = serialString.lastIndexOf(':');
    int length = serialString.length();
    String topic = serialString.substring(0, index);
    String serialVal = serialString.substring(index+1, length);

    if (topic == "movimento") {

        if (serialVal == "forward") movementInt = 1;
        else if (serialVal == "backward") movementInt = 2;
        else if (serialVal == "left") movementInt = 3;
        else if (serialVal == "right") movementInt = 4;
        else if (serialVal == "rotateSX") movementInt = 5;
        else if (serialVal == "rotateDX") movementInt = 6;
        else if (serialVal == "stop") movementInt = 7;

    } else if (topic == "emergenza"){

        if(serialVal == 1) emergencyStop();    

    }
}

// segnale di arresto del motore
void emergencyStop() {
    digitalWrite(dxForward, LOW);
    digitalWrite(dxBackward, LOW);
    analogWrite(dxForwardEn, 0);
    analogWrite(dxBackwardEn, 0);

    digitalWrite(sxForward, LOW);
    digitalWrite(sxBackward, LOW);
    analogWrite(sxForwardEn, 0);
    analogWrite(sxBackwardEn, 0);

    delay(1000);
}

// Funzione per andare avanti
void driveMotor(int motorForward) {
    if (speed == 0) digitalWrite(motorForward, HIGH);

    if (motorForward == dxForward){
        analogWrite(dxForwardEn, speed);
        return;
    }
    if (motorForward == sxForward){
        analogWrite(sxForwardEn, speed);
        return;
    }
}

// Funzione per andare indietro
void reverseMotor(int motorBackward) {    
    if (speed == 0) digitalWrite(motorBackward, HIGH);

    int reverseSpeed = 0 - speed;

    if (motorForward == dxBackward){
        analogWrite(dxBackwardEn, reverseSpeed);
        return;
    }
    if (motorForward == sxBackward){
        analogWrite(sxBackwardEn, reverseSpeed);
        return;
    }
}

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