int emergencyControl;
String serialString = "";
int movementInt;
int dxForward = 2, dxBackward = 3, dxForwardEn = 9, dxBackwardEn = 10; // Motore DX
int sxForward = 4, sxBackward = 5, sxForwardEn = 11, sxBackwardEn = 12;  // Motore SX
int speed;
const int maxSpeed = 150;

void setup() {
    Serial.begin(9600);
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
    if (Serial.available() > 0) {           // Se il seriale legge qualcosa
        emergencyControl = digitalRead(7);
        if (emergencyControl == 1) {         // Se c'è un'emergenza
            // codice per fermare il motore
            Serial.write("");
        }
        else {
            mapping();
            switch (movementInt) {
                case 1:
                    driveMotor("dx");
                    driveMotor("sx");
                    break;

                case 2:
                    reverseMotor("dx");
                    reverseMotor("sx");
                    break;

                case 3:
                    driveMotor("dx")
                    halfMotor("sx")
                    break;

                case 4:
                    halfMotor("dx")
                    driveMotor("sx");
                    break;

                case 5:
                    reverseMotor("dx");
                    driveMotor("sx");
                    break;

                case 6:
                    driveMotor("dx");
                    reverseMotor("sx");
                    break;
                case 7:
                    emergencyStop();
            }
        }
    }
}

// mapping delle 
void mapping() {
    serialString = Serial.read();
    int index = serialString.lastIndexOf(':');
    int length = serialString.length();
    String topic = serialString.substring(0, index);
    String serialVal = serialString.substring(index+1, length);
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
}

// Funzione per andare avanti
void driveMotor(String direction) {
    if (speed < maxSpeed){
        speed++;
    }
    digitalWrite(m1, HIGH);
    analogWrite(en, speed); // Valore del PWM tra 0 (spento) e 255 (massima velocità)
}

// Funzione per andare indietro
void reverseMotor(int en, int m1, int m2) {
    digitalWrite(m1, HIGH);
    digitalWrite(m2, HIGH);
    analogWrite(en, -100); // Valore del PWM tra 0 (spento) e 255 (massima velocità)
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