#include <SPI.h>
#include <Ethernet.h>
#include <string.h>
#include <ArduinoJson.h>

//Connectivity Test IP
int currIPNode = 4;

//Arduino Server
int arduinoPort = 80;

// Network Settings
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };  // MAC address
IPAddress ip(192, 168, 0, 2); // IP address
IPAddress nodeServer(192, 168, 0, currIPNode);  // IP address of the server you want to connect to

EthernetClient client;
EthernetServer server(80);

void setup() {
  Serial.begin(9600);
  
  // -- Start the Ethernet connection --
  Ethernet.begin(mac);
  Ethernet.setLocalIP(ip);

  // -- Testing connectivity --
  //DEBUG: Serial.println(">> Testing connectivity (using Node server connection on port 3000)");
  
  if (Ethernet.linkStatus() == LinkOFF){
    Serial.println("Ethernet cable is not connected!");
  }

  // -- HTTP GET request to test connectivity (using retries) --
  for (int retryCount = 0; retryCount < 3; retryCount++){
    if (client.connect(nodeServer, 3000)) {
      //DEBUG: Serial.println("Connected to server");

      client.println("GET /");
      String connectionString = String("Host: 192.168.0." + String(currIPNode));
      client.println(connectionString);   // Replace with the actual IP address or domain of Node server
      client.println("Connection: keep-alive");
      client.println();
      break;
    } else{
      if (retryCount == 2){
        //DEBUG: Serial.println("Connection failed!");
      } else{
        //DEBUG: Serial.println("Connection failed. Retrying...");
        delay(3000);
      }
    }
  }
  client.stop();

  // -- Arduino As A Server AAAS ;) -- 
  server.begin();

  if (server){
    //DEBUG: Serial.print(">> Server is listening at ");
    //DEBUG: Serial.println(Ethernet.localIP());
  } else {
    //DEBUG: Serial.println(">> Server failed to start. Restart Arduino!");
  }
}



void loop() {
  //do all the server functions only if the server is on
  //otherwise do nothing 
  if (server){   //TODO: test this
    EthernetClient client = server.available();
    // -- If there's a POST request forward it to the other Arduino -- 
    if (client) {
      while (client.connected()) {
        if (client.available()) {
          String request = client.readStringUntil('\r');
          if (request.indexOf("POST") != -1) {
            client.println("HTTP/1.1 204 No Content");
            client.println("Content-Type: application/json");
            client.println("Access-Control-Allow-Origin: *");
            client.println();
            client.println("{\"response\":\"ok\"}");
            String body = client.readString();
            Serial1.println(body);
            break;
          }
        }
      }
      client.stop();
    }
  }
}
