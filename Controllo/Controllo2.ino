#define DX_FRONT_SPEED 9
#define DX_FRONT_ENABLE 3
#define DX_BACK_SPEED 8
#define DX_BACK_ENABLE 2

void setup()
{
    Serial.begin(9600);
    pinMode(DX_FRONT_SPEED, OUTPUT);
    pinMode(DX_FRONT_ENABLE, OUTPUT);
    pinMode(DX_BACK_SPEED, OUTPUT);
    pinMode(DX_BACK_ENABLE, OUTPUT);
    digitalWrite(DX_FRONT_ENABLE, HIGH);
    digitalWrite(DX_BACK_ENABLE, HIGH);
}

void loop() {
    
}

void checkMove(int speed) {
    if (analogRead(11) == HIGH) {
        moveForward(speed);
    } else {
        brake();
    }
    
}

void moveForward(int speed) {

}

void brake() {

}