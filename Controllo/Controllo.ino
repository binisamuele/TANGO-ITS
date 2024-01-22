int emergencyControl;
String serialString = "";
int movementInt;
int sx1 = 2, sx2 = 3, sxEn = 9;  // Motore A SX
int dx1 = 4, dx2 = 5, dxEn = 10; // Motore B DX
int speed;
const int maxSpeed = 150;

void setup()
{
    Serial.begin(9600);
    emergencyControl = 0;
    speed = 0;

    pinMode(sx1, OUTPUT); // Pin motore A messi a caso, da aggiornare
    pinMode(sx2, OUTPUT);
    pinMode(sxEn, OUTPUT);

    pinMode(dx1, OUTPUT); // Pin motore B messi a caso, da aggiornare
    pinMode(dx2, OUTPUT);
    pinMode(dxEn, OUTPUT);
}
void loop()
{
    if (Serial.available() > 0)
    { // Se il seriale legge qualcosa

        emergencyControl = digitalRead(10);
        if (emergencyControl == 1)
        { // Se c'è un'emergenza
            // codice per fermare il motore
            Serial.write("");
        }
        else
        {
            mapping();
            switch (movementInt)
            {
            case 1:
                driveMotor(sxEn, sx1, sx2);
                driveMotor(dxEn, dx1, dx2);
                delay(2000);
                break;

            case 2:
                reverseMotor(sxEn, sx1, sx2);
                reverseMotor(dxEn, dx1, dx2);
                delay(2000);
                break;

            case 3:
                leftMotor(sxEn, sx1, sx2, dxEn, dx1, dx2);
                break;

            case 4:
                rightMotor(sxEn, sx1, sx2, dxEn, dx1, dx2);
                break;

            case 5:
                driveMotor(sxEn, sx1, sx2);
                reverseMotor(dxEn, dx1, dx2);
                break;

            case 6:
                driveMotor(dxEn, dx1, dx2);
                reverseMotor(sxEn, sx1, sx2);
                break;
            case 7:
                emergencyStop();
            }
        }
    }
}

void mapping(){
    serialString = Serial.read();
    int index = serialString.lastIndexOf(':');
    int length = serialString.length();
    serialString = serialString.substring(index+1, length);
    if (serialString == "forward")
        movementInt = 1;
    else if (serialString == "backward")
        movementInt = 2;
    else if (serialString == "left")
        movementInt = 3;
    else if (serialString == "right")
        movementInt = 4;
    else if (serialString == "rotateSX")
        movementInt = 5;
    else if (serialString == "rotateDX")
        movementInt = 6;
    else if (serialString == "stop")
        movementInt = 7;
}

void emergencyStop()
{
    digitalWrite(sx1, LOW);
    digitalWrite(sx2, LOW);
    analogWrite(sxEn, 0);

    digitalWrite(b1, LOW);
    digitalWrite(b2, LOW);
    analogWrite(enB, 0); // segnale di arresto del motore
}

void driveMotor(int en, int m1, int m2)
{
    if (speed < maxSpeed)   // aumenta velocità se inferiore a maxSpeed
        speed++;
    digitalWrite(m1, HIGH);
    digitalWrite(m2, HIGH);
    analogWrite(en, speed); // Valore del PWM tra 0 (spento) e 255 (massima velocità)
}

// Funzione per guidare un motore all'indietro
void reverseMotor(int en, int m1, int m2)
{
    digitalWrite(m1, HIGH);
    digitalWrite(m2, HIGH);
    analogWrite(en, -100); // Valore del PWM tra 0 (spento) e 255 (massima velocità)
}

void leftMotor(int enA, int a1, int a2, int enB, int b1, int b2)
{
    digitalWrite(a1, HIGH);
    digitalWrite(a2, HIGH);
    analogWrite(enA, 100);

    digitalWrite(b1, HIGH);
    digitalWrite(b2, HIGH);
    analogWrite(enB, 50);
}

void rightMotor(int enA, int a1, int a2, int enB, int b1, int b2)
{
    digitalWrite(a1, HIGH);
    digitalWrite(a2, HIGH);
    analogWrite(enA, 50);

    digitalWrite(b1, HIGH);
    digitalWrite(b2, HIGH);
    analogWrite(enB, 100);
}