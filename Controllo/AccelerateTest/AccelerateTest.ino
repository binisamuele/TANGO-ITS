const int dxForward = 5, dxBackward = 4, dxForwardEn = 27, dxBackwardEn = 26;   // Motore DX, pin da modificare
const int sxForward = 7, sxBackward = 6, sxForwardEn = 22, sxBackwardEn = 23;   // Motore SX, pin da modificare
int movementInt = 0;
int speed = 0;  // Valore del PWM tra 0 (spento) e 255 (massima velocità)
int key = 40; // pin della chiave
const int speedGain = 3;
const int maxSpeed = 150;
const int minSpeed = -50;
bool emergency = false;
bool isRotating = false;

void setup() {
    Serial.begin(9600);

    pinMode(dxForward, OUTPUT);
    pinMode(dxBackward, OUTPUT);
    pinMode(dxForwardEn, OUTPUT);
    pinMode(dxBackwardEn, OUTPUT);

    pinMode(sxForward, OUTPUT);
    pinMode(sxBackward, OUTPUT);
    pinMode(sxForwardEn, OUTPUT);
    pinMode(sxBackwardEn, OUTPUT);

    digitalWrite(dxForwardEn, HIGH);
    digitalWrite(sxForwardEn, HIGH);
    digitalWrite(dxBackwardEn, HIGH);
    digitalWrite(sxBackwardEn, HIGH);

    pinMode(key, INPUT_PULLUP); // necessario per far funzionare la chiave -- fare attenzione alle interferenze nel caso in cui il motore non venga messo a 0

    pinMode(2, INPUT_PULLUP); // necessario per fare funzionare i bottoni

    // attachInterrupt(0, toDelete, RISING);    // Pin 2 per emergenza pulsanti    //DEBUG
}

void toDelete(){                        //DEBUG 
  Serial.println("Button Pressed");
}

void loop() {
	// controllo della comunicazione seriale (anche gli altri arduino devono fare il controllo del seriale)
	if (emergency) {
        emergencyState();
        return;
    }

	if (speed == 0){
        movementInt = 1;
    }
	// if (speed < maxSpeed){
    //     Serial.println("velocità: ");
    //     Serial.println(speed);
    // }

    movement(); // switch del movimento

}

// funzione stato emergenza
void emergencyState() {
    if (!emergency){            // ferma la macchina e manda un messaggio di emergenza agli altri arduino
        emergency = true;
        emergencyStop();
    }

    while (emergency || !digitalRead(key))      		// rimane nel loop finché non viene girata la chiave o viene mandato un messaggio dall'app
    {
        if (!digitalRead(key)){
            emergency = false;
        }
    }
}

// segnale di arresto del motore (potrebbe essere non necessaria)
void emergencyStop() {
    stopMotor();
    resetVariables();

    delay(1000);
}

// reset delle variabili
void resetVariables() {
    speed = 0;
    movementInt = 0;
}

// funzione con le opzioni di movimento
void movement(){
    switch (movementInt) {
        // andare avanti
        case 1:
            rotationCheck();

            if (speed < 0) {                                   // se la velocità era negativa, rallentiamo i motori
                decelerate();
                break;
            }

            accelerate();
            break;
        // andare indietro
        case 2:
            rotationCheck();

            if (speed > 0) {                                   // se la velocità era positiva, rallentiamo i motori
                decelerate();
                break;
            }

            accelerate();
            break;
		// frenata
        case 5:
            rotationCheck();

            decelerate();
            decelerate();
            break;
        // nessun comando di movimento
        default:
            rotationCheck();

            decelerate();
            break;
    }
}

// controllo della velocità vicino a zero
void speedControl(){
    if (speed < speedGain && speed > -speedGain) speed = 0;
}

// controllo della rotazione
void rotationCheck(){
    if (isRotating){
        stopMotor();
        isRotating = false;
    }
}

// funzione per fermare i motori
void stopMotor(){
    analogWrite(dxForward, 0);
    analogWrite(dxBackward, 0);

    analogWrite(sxForward, 0);
    analogWrite(sxBackward, 0);

    delay(1000); // da testare
}

// funzione per muoversi avanti o indietro
void driveMotor(int motor1, int motor2, int spd) {
    if (spd < 0) spd = -spd;

    analogWrite(motor1, spd);
    analogWrite(motor2, spd);

    delay(1000); // da testare
}

// funzione per accelerare
void accelerate(){
    if (speed >= 0) {
        if (speed >= maxSpeed) return;      // se la velocità è al massimo, non fare niente

        speed += speedGain;
        driveMotor(dxForward, sxForward, speed);
        return;
    }

    if (speed <= 0) {
        if (speed <= minSpeed) return;      // se la velocità è al minimo, non fare niente

        speed -= speedGain;
        driveMotor(dxBackward, sxBackward, speed);
        return;
    }
}

// funzione per decelerare e lentamente fermarsi
void decelerate(){
    if (speed < 0) {
        speed += speedGain;
        speedControl();
        driveMotor(dxBackward, sxBackward, speed);
        return;
    }

    if (speed > 0) {
        speed -= speedGain;
        speedControl();
        driveMotor(dxForward, sxForward, speed);
        return;
    }
}