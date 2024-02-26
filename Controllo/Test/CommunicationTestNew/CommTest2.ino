// COSTANTI PINS
const int DX_FORWARD = 5, DX_BACKWARD = 4, DX_FORWARD_EN = 27, DX_BACKWARD_EN = 26;   // Motore DX
const int SX_FORWARD = 7, SX_BACKWARD = 6, SX_FORWARD_EN = 22, SX_BACKWARD_EN = 23;   // Motore SX

const int BUMPERS = 3;                  // pin dei bumpers
const int BUTTONS = 2;                  // pin dei bottoni
const int EMERGENCY_PIN;                // pin di emergenza
const int ARDUINO_EMERGENCIES = 8;      // pin per le emergenze degli altri Arduino

const int KEY = 9;                      // pin della chiave
const int START_FROM_APP;               // pin collegato all'app per l'accensione
const int COMMUNICATION_PIN;            // pin per la rispota ai messaggi

// COSTANTI
const int SENSORS_NUMBER = 6;
const int MAX_DISTANCE = 200;
const int INTERVAL = 1000;
const int SONAR_INTERVAL = 50;

const int MAX_SPEED = 150;
const int MIN_SPEED = -100;
const int SPEED_GAIN = 2;
const int LOW_SPEED = 35;   //da testare

const int TANGO_SIZE = 20;
const int EMERGENCY_DISTANCE = 40;
const int SECURITY_DISTANCE = 60;
const float SPEED_OF_SOUND = 0.0343;

// VARIABILI
unsigned long startTime;
unsigned long currentTime;
unsigned long commTime;

int speed = 0;
int movementInt = 0;
int brakingTime = 0;
int sensorIndex = 0;

bool emergency = true;
bool forwardDir = true;
bool isRotating = false;

String serialString = "";

void setup() {
    Serial.begin(9600);
    Serial1.begin(9600);    // collegamento all'arduino di comunicazione

    startTime = millis();

    pinMode(DX_FORWARD, OUTPUT);
    pinMode(DX_BACKWARD, OUTPUT);
    pinMode(DX_FORWARD_EN, OUTPUT);
    pinMode(DX_BACKWARD_EN, OUTPUT);

    pinMode(SX_FORWARD, OUTPUT);
    pinMode(SX_BACKWARD, OUTPUT);
    pinMode(SX_FORWARD_EN, OUTPUT);
    pinMode(SX_BACKWARD_EN, OUTPUT);

	digitalWrite(DX_FORWARD_EN, HIGH);
    digitalWrite(SX_FORWARD_EN, HIGH);
    digitalWrite(DX_BACKWARD_EN, HIGH);
    digitalWrite(SX_BACKWARD_EN, HIGH);

    pinMode(EMERGENCY_PIN, OUTPUT);
    pinMode(COMMUNICATION_PIN, OUTPUT);

    pinMode(KEY, INPUT_PULLUP);

    pinMode(BUTTONS, INPUT_PULLUP);         // emergenza bottoni
    pinMode(BUMPERS, INPUT_PULLUP);         // emergenza bumper
    pinMode(ARDUINO_EMERGENCIES, INPUT);    // emergenza arduino
}

void loop() {
    currentTime = millis();
    /*
    if (!Serial1 || digitalRead(BUTTONS)) {
        emergencyState();
        return;
    }
	*/
	readSerial();

    if (currentTime - startTime >= INTERVAL){
		Serial.println(movementInt);
        startTime = currentTime;
        if (movementInt == 5) emergencyState();
    }

	//movement();   // questa parte di codice è da testare
}

// GESTIONE EMERGENZA           
///////////////////////////////////////////////////////////////////////////////
// funzione stato emergenza
void emergencyState() {
    if (!emergency){            // ferma la macchina e manda un messaggio di emergenza agli altri arduino
        emergency = true;
        emergencyStop();
    }

    int start = 0;
    while (emergency)      // rimane nel loop finché non viene girata la chiave o viene mandato un messaggio dall'app
    {
        start = Serial.parseInt(); // DEBUG per far ripartire il codice scrivendo 9 nel monitor serial
        if (start == 9) emergency = false;
    }
}

// funzione di comunicazioni di emergenza
void emergencyComm(){
    if (emergency){
        digitalWrite(EMERGENCY_PIN, HIGH);
        return;
    }

    digitalWrite(EMERGENCY_PIN, LOW);
}   

// arresto di emergenza del motore
void emergencyStop() {
    // stopMotor();

    // speed = 0;
    // movementInt = 0;
}

// GESTIONE COMUNICAZIONE          
///////////////////////////////////////////////////////////////////////////////
// lettura dell'arduino di comunicazione
void readSerial(){
    if (Serial1.available()) {
        serialString = Serial1.readStringUntil('\r\n');
        digitalWrite(COMMUNICATION_PIN, HIGH);
        commTime = millis();
        mapping(serialString);   
        Serial1.flush();	// da testare
    }

    if (currentTime - commTime >= INTERVAL){
        digitalWrite(COMMUNICATION_PIN, LOW);
        commTime = currentTime;
    }
}

// mapping dei messaggi
void mapping(String serialString) {

    if (serialString == "up"){
        movementInt = 1;
        return;
    }
    if (serialString == "down"){
        movementInt = 2;
        return;
    }
    if (serialString == "right"){ // ruotare a destra
        movementInt = 3;
        return;
    }
    if (serialString == "left"){ // ruotare a sinistra
        movementInt = 4;
        return;
    }
    if (serialString == "emergencyStop"){
        movementInt = 5;
        return;
    }
    movementInt = 0;
}