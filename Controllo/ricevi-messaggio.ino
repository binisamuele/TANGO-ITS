int messagge;

void setup() {
    Serial.begin(9600);
    message = 0;
}

void loop() {
    message = Serial.read()
}