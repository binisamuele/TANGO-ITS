void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial1.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial1.available()) {
    Serial1.print("up");
    Serial.print("up");
    delay(5000);
    Serial1.print("down");
    Serial.print("down");
    delay(5000);
    Serial1.print("release");
    Serial.print("release");
    delay(5000);
    Serial1.print("emergencyStop");
    Serial.print("emergencyStop");
    delay(5000);

  }
}
