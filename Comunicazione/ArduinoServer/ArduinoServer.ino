#include <SPI.h>
#include <Ethernet.h>
#include <string.h>
#include <ArduinoJson.h>
#include <EthernetUdp.h>

//PINS
int emergency = 13;
int emergencyStatus = 12;

// Connection Variables
int UDP_PORT_OUT = 8989;
int UDP_PORT_IN = 7979;
EthernetUDP udp;
IPAddress defaultIP(23, 23, 23, 23);
IPAddress androidIP;
IPAddress broadcastIP(192, 168, 1, 255);

// Connection checks variables
unsigned long startTime;
unsigned long currentTime;
unsigned long duration = 3000;
String messageCheck;
bool rcvFailed = true;
int retries = 0;

//Arduino Server
int arduinoPort = 80;

// Network Settings
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };  // MAC address
IPAddress ip(192, 168, 1, 6); // IP address

EthernetClient client;
EthernetServer server(80);

void setup() {
  // emergency PIN
  pinMode(emergency, OUTPUT);
  pinMode(emergencyStatus, INPUT);
  
  // Initialize the start time
  startTime = millis();

  // Start the serial communications
  Serial.begin(9600);
  Serial1.begin(9600);
  
  // Setup the Ethernet connection
  Ethernet.begin(mac);
  Ethernet.setLocalIP(ip);
  
  // Make sure that Ethernet is linked
  if (Ethernet.linkStatus() == LinkOFF){                              // TEST THIS ONE!
    Serial.println("Ethernet cable is not connected!");
    while (Ethernet.linkStatus() == LinkOFF);
    Serial.println("Ethernet cable is now connected!");
  }

  // Start the UDP connection with the Android app
  udp.begin(UDP_PORT_IN);
  connectToAndroidApp();

  // ----  START ARDUINO SERVER  -----
  // ---------------------------------
  // -- Arduino As A Server AAAS ;) -- 
  server.begin();

  if (server) {
    /*DEBUG: */Serial.print(">> Server is listening at ");
    /*DEBUG: */Serial.println(Ethernet.localIP());
  } else {
    /*DEBUG: */Serial.println(">> Server failed to start. Restart Arduino!");
  }
}

void loop() {
  currentTime = millis();

  EthernetClient client = server.available();
  // -- POST ENDPOINT ON ARDUINO SERVER -- 
  if (client) {
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()){
      while(client.available()){
        char c = client.read();
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank){

          // Here is where the POST data is.  
          while(client.available())
          {
              String body = client.readString();
              String key = body.substring(body.indexOf("{\"")+2,body.indexOf("\":")); //get key from json
              String value = body.substring(body.indexOf(":\"")+2,body.length()-2); // get value from json
              if (key == "direction"){
                if (value != "emergencyStop"){
                  forwardToControlArduino(value);
                  if(value == "commStop");
                }
                else {
                  Serial.println("EMERGENCY!");
                  // emergency STOP
                  startEmergencyStop();
                }
              }
              //Serial.print(key);
          }
          Serial.println();

          client.println("HTTP/1.0 200 OK");
          client.println("Content-Type: text/html");
          client.println();
          client.println("200");
          client.stop();
        }
        else if (c == '\n'){
          // you're starting a new line
          currentLineIsBlank = true;
        } 
        else if (c != '\r'){
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        } 
      }
    }
    udp.beginPacket(broadcastIP, UDP_PORT_OUT);
    udp.write("ok");
    udp.endPacket();
  }

  if (currentTime - startTime > duration){
    udp.beginPacket(broadcastIP, UDP_PORT_OUT);
    messageCheck = "ok";
    udp.write(messageCheck.c_str());
    udp.endPacket();
  
    int pkt = udp.parsePacket();
    if (pkt) {
      char packetBuffer[UDP_TX_PACKET_MAX_SIZE];
      udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
      String received = String(packetBuffer);
      if (received.indexOf("ok") != -1){
        rcvFailed = false;
        retries = 0;
      }
    }
    for (int i=0; i<10; i++){
      udp.parsePacket();
    }

    if (rcvFailed)
      retries++;
    if (retries >= 3){
      retries = 0;
      // stop motors
      forwardToControlArduino("brake");
      Serial.println("DISCONNECTED!");
      connectToAndroidApp();
    }

    rcvFailed = true;
    currentTime = millis();
    startTime = currentTime;
  }
  
  // read another arduino's serial to get sensors values 
  //readSensorValues();
}

void connectToAndroidApp(){
  Serial.println("Connecting to Android...");
  androidIP = defaultIP;
  bool ipHasBeenReceived = false;
  
  while (androidIP == defaultIP || !ipHasBeenReceived){
    int packetSize = udp.parsePacket();
    if (packetSize) {
      // Read the packet
      char packetBuffer[UDP_TX_PACKET_MAX_SIZE];
      udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
      
      // DEBUG: print the received packet
      //Serial.print("Received packet: ");
      //Serial.println(packetBuffer);

      // Parse the packet and check if contains "ok"
      String received = String(packetBuffer);
      if (received.indexOf("ok") != -1){
        ipHasBeenReceived = true;
      }

      // Storing androidIP address and using it to send Arduino's IP
      androidIP = udp.remoteIP();
    } 

    udp.beginPacket(broadcastIP, UDP_PORT_OUT);
    String message;
    if (ipHasBeenReceived){
      message = "ok";
    }
    else {
      message = "Arduino IP";
    }
    udp.write(message.c_str());
    udp.endPacket();
    delay(1000);
  }

  Serial.println("CONNECTED");
}

// send interrupt to control arduino to perform an emergency stop
void startEmergencyStop(){
  digitalWrite(emergency, HIGH);
  delay(50);
  digitalWrite(emergency, LOW);
}

// function to send direction string to the other arduino
void forwardToControlArduino(String value){
  Serial.println(value);
  Serial1.print(value);
}

void readSensorValues(){
  if (Serial.available()) {
    String received = Serial.readStringUntil('\n');
  }
}