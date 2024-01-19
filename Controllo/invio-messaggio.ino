long rand;

void setup() {
    Serial.begin(9600);
    rand = 0;
}

void loop() {
    rand = random(2, 10);
    Serial.write("ciao");
    delay(rand * 1000);
}