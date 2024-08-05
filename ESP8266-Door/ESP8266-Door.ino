#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>
 
const char* ssid = "*";
const char* password = "*";

unsigned long sinceRebootMillis = 0;
unsigned long relayOnMillis = 0;

//const long rebootInterval = 43200000; // 12 hrs = 43,200,000 millis
const long rebootInterval = 120000; // 2 mins
const long relayOnInterval = 1000; // 1 sec relay on. Door opens when the switch turns off


 
 
//int ledPin = 1; // GPIO1 or TX0 of ESP8266 (Does not boot in above config ~ Tx pin held low at boot)
int ledPin = 3; // GPIO3 or RX0 of ESP8266
int relayState = LOW; 

WiFiServer server(80);//Service Port
 
void setup() {
  

  pinMode(ledPin, INPUT_PULLUP); // Enable internal pull-up resistor
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
   
  // Connect to WiFi network
  
   
  WiFi.begin(ssid, password);
   
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  
  }
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);
   
  // Start the server
  server.begin();





  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_FS
      type = "filesystem";
    }

    // NOTE: if updating FS this would be the place to unmount FS using FS.end()
    
  });
  ArduinoOTA.onEnd([]() {
  
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
   
  });
  ArduinoOTA.onError([](ota_error_t error) {
    
    if (error == OTA_AUTH_ERROR) {
      
    } else if (error == OTA_BEGIN_ERROR) {
      
    } else if (error == OTA_CONNECT_ERROR) {
      
    } else if (error == OTA_RECEIVE_ERROR) {
      
    } else if (error == OTA_END_ERROR) {
      
    }
  });
  ArduinoOTA.begin();

  
 
}
 
void loop() {
  ArduinoOTA.handle();
  unsigned long currentMillis = millis();
  //reboot if it has been up for more than set interval
if (currentMillis - sinceRebootMillis >=rebootInterval){
  ESP.restart();
  }
  
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
   
  // Wait until the client sends some data
  
  while(!client.available()){
    yield();
  }
   
  // Read the first line of the request
  String request = client.readStringUntil('\r');
  
  client.flush();
   
  // Match the request
 
  
  if (request.indexOf("/OPEN") != -1) {
    digitalWrite(ledPin, HIGH);
    relayOnMillis = millis();
relayState = HIGH;
  }
    
if (relayState==HIGH){
//if (currentMillis- relayOnMillis >=relayOnInterval){
 delay(1000);
  digitalWrite(ledPin, LOW);
   relayState=LOW;
   //}
}
  
   
  // Return the response
  //client.println("HTTP/1.1 200 OK");
  //client.println("Content-Type: text/html");
  //client.println(""); //  do not forget this one
client.print(F("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\nOK "));


  // close the connection:
    client.flush();


  

  
}
