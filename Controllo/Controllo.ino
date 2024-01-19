int emergencyControl;
void setup(){
    Serial.begin(9600);
    emergencyControl = 0;
}
void loop(){
    if(Serial.available()>0){                   //Se il seriale legge qualcosa
        emergencyControl=Serial.read();
        if(emergencyControl == 1){              //Se c'è un'emergenza
                                                //codice per fermare il motore
            Serial.write("");
        }
        
    }
}