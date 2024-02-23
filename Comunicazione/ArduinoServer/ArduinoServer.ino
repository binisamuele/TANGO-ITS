#include <SPI.h>
#include <Ethernet.h>
#include <string.h>
#include <ArduinoJson.h>
#include <EthernetUdp.h>

//PINS
int emergency = 13;

// Connection Variables
int UDP_PORT = 8888;
EthernetUDP udp;
IPAddress defaultIP(23, 23, 23, 23);
IPAddress androidIP = defaultIP;
IPAddress broadcastIP(192, 168, 0, 255);

// Connection checks variables
unsigned long startTime;
unsigned long currentTime;
unsigned long duration = 4500;
bool checked = false;
bool failed = false;
bool comExtableshed = false;

//Arduino Server
int arduinoPort = 80;

// Network Settings
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };  // MAC address
IPAddress ip(192, 168, 0, 4); // IP address

EthernetClient client;
EthernetServer server(80);

void setup() {
  // emergency PIN
  pinMode(13, OUTPUT);
  
  startTime = millis();

  Serial.begin(9600);
  
  // -- Start the Ethernet connection --
  Ethernet.begin(mac);
  Ethernet.setLocalIP(ip);
  
  // Make sure that Ethernet is linked
  if (Ethernet.linkStatus() == LinkOFF){                              // TEST THIS ONE!
    Serial.println("Ethernet cable is not connected!");
    while (Ethernet.linkStatus() == LinkOFF);
    Serial.println("Ethernet cable is now connected!");
  }

  Serial.println("Connecting to Android...");
  udp.begin(8888);
  connectToAndroidApp();    //possible issue: i may have passed variable by value insted of reference

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
    while (client.connected()) {
      while(client.available()) {
        char c = client.read();
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {

          // Here is where the POST data is.  
          while(client.available())
          {
              String body = client.readString();
              String key = body.substring(body.indexOf("{\"")+2,body.indexOf("\":")); //get key from json
              String value = body.substring(body.indexOf(":\"")+2,body.length()-2); // get value from json
              if (key == "arduinoCheck"){
                if (value == "stop"){
                  comExtableshed = false;
                } else if (value == "ok"){
                  checked = true;
                }
              }
              if (key == "direction"){
                if (value != "emergencyStop"){
                  forwardToControlArduino(value);
                  comExtableshed = true;
                }
                else {
                  comExtableshed = false;
                  Serial.println("EMERGENCY!");
                  // emergency STOP
                  startEmergencyStop();
                }
              }
              Serial.print(key);
          }
          Serial.println();

          client.println("HTTP/1.0 200 OK");
          client.println("Content-Type: text/html");
          client.println();
          client.println("ok");
          client.stop();
        }
        else if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } 
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        } 
      }
    }
  }

  if (currentTime - startTime > duration) {
    startTime = currentTime;
    if (checked) {
      Serial.println("> Check passed!");
      checked = false; //check passed 
      failed = false;
    } else if (comExtableshed){
      if (!failed){
        failed = true;
      }
      else {
        Serial.println("> Check failed!");
        comExtableshed = false;
        checked = false;
        failed = false;
        // emergency STOP
        startEmergencyStop();
      }
    }
  }
  
  // read another arduino's serial to get sensors values 
  readSensorValues();
}

void connectToAndroidApp(){
  Serial.println(androidIP);
  while (androidIP == defaultIP){
    int packetSize = udp.parsePacket();
    if (packetSize) {
      Serial.println("Datagram Received");
      // Read the packet
      char packetBuffer[UDP_TX_PACKET_MAX_SIZE];
      udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
      
      // DEBUG: print the received packet
      Serial.print("Received packet: ");
      Serial.println(packetBuffer);

      // Storing androidIP address and using it to send Arduino's IP
      androidIP = udp.remoteIP();
      //DEBUG START
      Serial.print("Android IP: ");
      Serial.println(androidIP);
      //DEBUG END

      // Send Arduino's IP to Android until it's received
      do{
        udp.beginPacket(broadcastIP, UDP_PORT);
        udp.write("ciao");
        udp.endPacket();
        delay(3000);
      } while (true);
    } 
  }
}

// send interrupt to control arduino to perform an emergency stop
void startEmergencyStop(){
        digitalWrite(emergency, HIGH);
        delay(1000);
        digitalWrite(emergency, LOW);
}

// function to send direction string to the other arduino
void forwardToControlArduino(String value){
  Serial.println(value);
}

void readSensorValues(){
  if (Serial.available()) {
    String received = Serial.readStringUntil('\n');
  }
}