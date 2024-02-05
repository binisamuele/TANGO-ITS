#include <SPI.h>
#include <Ethernet.h>
#include <string.h>
#include <ArduinoJson.h>

//Check if connected to server
unsigned long startTime = millis();
unsigned long currentTime;
unsigned long duration = 8000;
bool checked = false;
bool firstFail = true;
bool comExtableshed = false;

//Connectivity Test IP
int currIPNode = 3;

//Arduino Server
int arduinoIP = 4;
int arduinoPort = 80;

// Network Settings
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };  // MAC address
IPAddress ip(192, 168, 1, arduinoIP); // IP address
IPAddress nodeServer(192, 168, 1, currIPNode);  // IP address of the server you want to connect to

EthernetClient client;
EthernetServer server(80);

void setup() {
  unsigned long startTime = millis();

  Serial.begin(9600);
  
  // -- Start the Ethernet connection --
  Ethernet.begin(mac);
  Ethernet.setLocalIP(ip);

  // -- Testing connectivity --
  Serial.println(">> Testing connectivity (using Node server connection on port 3000)");
  
  if (Ethernet.linkStatus() == LinkOFF){
    Serial.println("Ethernet cable is not connected!");
  }

  // -- HTTP GET request to test connectivity (using retries) --
  for (int retryCount = 0; retryCount < 3; retryCount++){
    if (client.connect(nodeServer, 3000)) {
      /*DEBUG: */Serial.println("Connected to server");

      client.println("GET /");
      String connectionString = String("Host: 192.168.0." + String(currIPNode));
      client.println(connectionString);   // Replace with the actual IP address or domain of Node server
      client.println("Connection: keep-alive");
      client.println();
      break;
    } else{
      if (retryCount == 2){
        /*DEBUG: */Serial.println("Connection failed!");
      } else{
        /*DEBUG: */Serial.println("Connection failed. Retrying...");
        delay(3000);
      }
    }
  }
  client.stop();

  // -- Arduino As A Server AAAS ;) -- 
  server.begin();

  if (server){
    /*DEBUG: */Serial.print(">> Server is listening at ");
    /*DEBUG: */Serial.println(Ethernet.localIP());
  } else {
    /*DEBUG: */Serial.println(">> Server failed to start. Restart Arduino!");
  }
}

void loop() {
  currentTime = millis();

  EthernetClient client = server.available();
  // -- If there's a POST request forward it to the other Arduino -- 
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
                  comExtableshed = true;
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
      firstFail = true;
    } else if (comExtableshed){
      if (firstFail){
        firstFail = false;
      }
      else {
        Serial.println("> Check failed!");
        comExtableshed = false;
        checked = false;
        firstFail = true;
        // emergency STOP
      }
    }
  }
}
