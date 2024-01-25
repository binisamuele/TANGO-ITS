int emergencyControl;
String serialString = "";
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
    emergencyControl = 0;

    pinMode(dxForward, OUTPUT);
    pinMode(dxBackward, OUTPUT);
    pinMode(dxForwardEn, OUTPUT);
    pinMode(dxBackwardEn, OUTPUT);

    pinMode(sxForward, OUTPUT);
    pinMode(sxBackward, OUTPUT);
    pinMode(sxForwardEn, OUTPUT);
    pinMode(sxBackwardEn, OUTPUT);
}

void loop() {
    emergencyControl = digitalRead(7);
    if (Serial1.available() > 0) {           // Se il seriale legge qualcosa
        if (emergencyControl == 1) {         // Se c'è un'emergenza
            // codice per fermare il motore
            Serial1.write("");
        }
        else {
            mapping();
            switch (movementInt) {
                case 1:
                    if (speed < 0){
                        emergencyStop();
                        break;
                    }
                    driveMotor(dxForward);
                    driveMotor(sxForward);
                    speed = speed + speedGain;
                    break;

                case 2:
                    if (speed > 0){
                        emergencyStop();
                        break;
                    }
                    reverseMotor(dxBackward);
                    reverseMotor(sxBackward);
                    break;

                case 3:
                    driveMotor("dx")
                    halfMotor("sx")
                    break;

                case 4:
                    halfMotor("dx")
                    driveMotor("sx");
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
                    emergencyStop();
                    break;

            }
        }
    } else if (Serial2.available() > 0) {
        // codice nel caso in cui il lidar legge qualcosa
    } else {
        decelerate();
    }
}

// mapping delle 
void mapping() {
    serialString = Serial1.read();
    int index = serial1String.lastIndexOf(':');
    int length = serial1String.length();
    String topic = serial1String.substring(0, index);
    String serialVal = serial1String.substring(index+1, length);
    if(topic == "movimento"){
        if (serialVal == "forward")movementInt = 1;
        else if (serialVal == "backward")movementInt = 2;
        else if (serialVal == "left")movementInt = 3;
        else if (serialVal == "right")movementInt = 4;
        else if (serialVal == "rotateSX")movementInt = 5;
        else if (serialVal == "rotateDX")movementInt = 6;
        else if (serialVal == "stop")movementInt = 7;
    }else if(topic == "emergenza");
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
    if (speed == maxSpeed){
        return;
    }
    if (speed == 0){
        digitalWrite(motorForward, HIGH);
    }
    int newSpeed = speed + speedGain;
    if (motorForward == dxForward){
        analogWrite(dxForwardEn, newSpeed);
        return;
    }
    if (motorForward == sxForward){
        analogWrite(sxForwardEn, newSpeed);
        return;
    }
}

// Funzione per andare indietro
void reverseMotor(int motorBackward) {
    if (speed == minSpeed){
        return;
    }
    if (speed == 0){
        digitalWrite(motorBackward, HIGH);
    }
    int newSpeed = speed - speedGain;
    if (motorForward == dxBackward){
        analogWrite(dxBackwardEn, newSpeed);
        return;
    }
    if (motorForward == sxBackward){
        analogWrite(sxBackwardEn, newSpeed);
        return;
    }
}

// Funzione per girare
void leftMotor(int enA, int a1, int a2, int enB, int b1, int b2) {
    digitalWrite(a1, HIGH);
    digitalWrite(a2, HIGH);
    analogWrite(enA, 100);

    digitalWrite(b1, HIGH);
    digitalWrite(b2, HIGH);
    analogWrite(enB, 50);
}

// Funzione superflua per girare
void rightMotor(int enA, int a1, int a2, int enB, int b1, int b2) {
    digitalWrite(a1, HIGH);
    digitalWrite(a2, HIGH);
    analogWrite(enA, 50);

    digitalWrite(b1, HIGH);
    digitalWrite(b2, HIGH);
    analogWrite(enB, 100);
}

void decelerate(){  // aggiungere spegnimento motori
    if (speed > 0){
        speed = speed - 10;
    } else (speed < 0){
        speed = speed + 10;
    }
}