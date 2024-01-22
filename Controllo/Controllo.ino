int emergencyControl;
String stringaSeriale = "";
int movementInt;
int a1 = 2, a2 = 3, enA = 9;  // Motore A SX
int b1 = 4, b2 = 5, enB = 10; // Motore B DX
const int maxSpeed = 150;

void setup()
{
  Serial.begin(9600);
  emergencyControl = 0;

  pinMode(a1, OUTPUT); // Pin motore A messi a caso, da aggiornare
  pinMode(a2, OUTPUT);
  pinMode(enA, OUTPUT);

  pinMode(b1, OUTPUT); // Pin motore B messi a caso, da aggiornare
  pinMode(b2, OUTPUT);
  pinMode(enB, OUTPUT);
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
        driveMotor(enA, a1, a2);
        driveMotor(enB, b1, b2);
        delay(2000);
        break;

      case 2:
        reverseMotor(enA, a1, a2);
        reverseMotor(enB, b1, b2);
        delay(2000);
        break;

      case 3:
        leftMotor(enA, a1, a2, enB, b1, b2);
        break;

      case 4:
        rightMotor(enA, a1, a2, enB, b1, b2);
        break;

      case 5:
        driveMotor(enA, a1, a2);
        reverseMotor(enB, b1, b2);
        break;

      case 6:
        driveMotor(enB, b1, b2);
        reverseMotor(enA, a1, a2);
        break;
      case 7:
        emergencyStop();
      }
    }
  }
}

void mapping()
{
  stringaSeriale = Serial.read();
  if (stringaSeriale == "forward")movementInt = 1;
  else if (stringaSeriale == "backward")movementInt = 2;
  else if (stringaSeriale == "left")movementInt = 3;
  else if (stringaSeriale == "right")movementInt = 4;
  else if (stringaSeriale == "rotateSX")movementInt = 5;
  else if (stringaSeriale == "rotateDX")movementInt = 6;
  else if (stringaSeriale == "stop")movementInt = 7;
}

void emergencyStop()
{
  digitalWrite(a1, LOW);
  digitalWrite(a2, LOW);
  analogWrite(enA, 0);

  digitalWrite(b1, LOW);
  digitalWrite(b2, LOW);
  analogWrite(enB, 0); // segnale di arresto del motore
}

void driveMotor(int en, int m1, int m2, int speed)
{
  speed = min(speed * 1.2, maxSpeed);
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