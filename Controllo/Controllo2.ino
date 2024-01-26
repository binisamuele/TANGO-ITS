#define DX_FRONT_SPEED 9
#define DX_FRONT_ENABLE 3
#define DX_BACK_SPEED 8
#define DX_BACK_ENABLE 2

int maxSpeed = 150;
int speed = 0;

void setup()
{
    pinMode(DX_FRONT_SPEED, OUTPUT);
    pinMode(DX_FRONT_ENABLE, OUTPUT);
    pinMode(DX_BACK_SPEED, OUTPUT);
    pinMode(DX_BACK_ENABLE, OUTPUT);
    digitalWrite(DX_FRONT_ENABLE, HIGH);
    digitalWrite(DX_BACK_ENABLE, HIGH);
}

void loop() {
    checkMove();
}

void checkVelocity() {
    if (speed < maxSpeed) {
        speed += 10;
        delay(20);
    }
}

void checkMove() {
    if (digitalRead(11) == HIGH) {
        moveForward();
    } else if (digitalRead(12) == HIGH) {
        brake();
    }
}

void moveForward() {
    checkVelocity();
    analogWrite(DX_FRONT_SPEED, speed);
}

void brake() {
    speed = 0;
}