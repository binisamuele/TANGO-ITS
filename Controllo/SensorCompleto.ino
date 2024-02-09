String serial1String = "";
String serial2String = "";
String serial3String = "";
int movementInt = 0;
int dxForward = 28, dxBackward = 29, dxForwardEn = 26, dxBackwardEn = 27; // Motore DX
int sxForward = 24, sxBackward = 25, sxForwardEn = 22, sxBackwardEn = 23;  // Motore SX che stiamo testando
int startFromApp = 50 // pin collegato all'app per l'accensione
int key = 30; // pin della chiave
int speed = 0;  // Valore del PWM tra 0 (spento) e 255 (massima velocità)
const int speedGain = 10;
const int maxSpeed = 150;
const int minSpeed = -100;
bool isRotating = false;
int instruction[] = {1, 1, 5, 2, 2, 5, 3, 5, 4, 5, 9};
int i = 0;
// float lidarDistance;
// float signalStrength;
// float batteryCharge;
// float ultrasoundDistance;
// float temperature;
// float humidity;
bool emergency = true;

void setup() {
    pinMode(dxForward, OUTPUT);
    pinMode(dxBackward, OUTPUT);
    pinMode(dxForwardEn, OUTPUT);
    pinMode(dxBackwardEn, OUTPUT);

    pinMode(sxForward, OUTPUT);
    pinMode(sxBackward, OUTPUT);
    pinMode(sxForwardEn, OUTPUT);
    pinMode(sxBackwardEn, OUTPUT);

    pinMode(key, INPUT);

    // attachInterrupt(0, emergencyState, FALLING); // Pin 2 per emergenza pulsanti
    // attachInterrupt(1, emergencyState, RISING); // Pin 3 per emergenza bumper
    // <attachInterrupt(2, emergencyState, RISING); // Pin 21 per emergenze arduino (hardware deve utilizzare un diodo)
    //attachInterrupt(3, emergencyResolve, RISING); // Pin 20 per stato emergenza
}

void loop() {
    // controllo della comunicazione seriale (anche gli altri arduino devono fare il controllo del seriale)
    if (emergency) {
        emergencyState();
        return;
    }
    movementInt = instruction[i];
    i++;
    
    movement(); // switch del movimento
}

// funzione stato emergenza
void emergencyState() {
    if (!emergency){            // ferma la macchina e manda un messaggio di emergenza agli altri arduino
        emergency = true;
        emergencyStop();
    }
    while (emergency || !digitalRead(key))      // rimane nel loop finché non viene girata la chiave o viene mandato un messaggio dall'app
    {
        if (!digitalRead(key) || digitalRead(startFromApp)) emergency = false;
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
            if (speed == maxSpeed) break;                       // se la velocità è al massimo, non fare niente

            speed += speedGain;
            speedControl();

            if (speed < 10) {                                   // se la velocità era negativa, rallentiamo i motori
                driveMotor(dxBackward, sxBackward, speed);
                break;
            }

            driveMotor(dxForward, sxForward, speed);
            break;
        // andare indietro
        case 2:
            rotationCheck();
            if (speed == minSpeed) break;                       // se la velocità è al minimo, non fare niente

            speed -= speedGain;
            speedControl();

            if (speed > -10) {                                   // se la velocità era positiva, rallentiamo i motori
                driveMotor(dxForward, sxForward, speed);
                break;
            }

            driveMotor(dxBackward, sxBackward, speed);
            break;
        // rotazione in senso orario
        case 3:
            if (speed != 0){
                decelerate();
                break;
            }
            driveMotor(sxForward, dxBackward, 20);
            isRotating = true;
            break;
        // rotazione in senso antiorario
        case 4:
            if (speed != 0){
                decelerate();
                break;
            }
            driveMotor(dxForward, sxBackward, 20);
            isRotating = true;
            break;
        // frenata
        case 5:
            rotationCheck();
            decelerate();
            decelerate();
            break;
        // // curvare destra DA FARE
        // case 6:
        //     halfMotor(sxForward);
        //     driveMotor(dxForward);
        //     break;
        // // curvare sinistra DA FARE
        // case 7:   
        //     halfMotor(dxForward);
        //     driveMotor(sxForward);
        //     break;
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
        
        // delay(50); // da testare
    }
}

// funzione per fermare i motori
void stopMotor(){
    analogWrite(dxForward, 0);
    analogWrite(dxBackward, 0);

    analogWrite(sxForward, 0);
    analogWrite(sxBackward, 0);
}

// funzione per muoversi avanti o indietro
void driveMotor(int motor1, int motor2, int spd) {
    if (spd < 0) spd = -spd;
    analogWrite(motor1, spd);
    analogWrite(motor2, spd);

    // delay(50); // da testare
}

// funzione per girare DA FARE
// void halfMotor(int motorForward) {
//     int newSpeed = 0;

//     if (speed > 0) newSpeed = speed - speedGain;

//     if (motorForward == dxForward){
//         analogWrite(dxForwardEn, newSpeed);
//         return;
//     }
//     if (motorForward == sxForward){
//         analogWrite(sxForwardEn, newSpeed);
//         return;
//     }
// }

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