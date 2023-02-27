
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>
#define LED1 D0
#define LED2 D8
//ssid & password
const char* ssid = "sHAH";
const char* password = "12345678";

//int LED1 = ;  
//int LED2 = 04;


int relay1 = 0;
int relay2 = 0;

//Server address
String serverName = "http://esp8266-bms.herokuapp.com/webhooks";
//String serverName = "http://192.168.0.108:8000/webhooks";

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastTime = 0;
// Timer set to 10 minutes (600000)
//unsigned long timerDelay = 600000;
// Set timer to 5 seconds (5000)
unsigned long timerDelay = 100;

void setup() {
  Serial.begin(115200); 
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");

  }
  Serial.println("moving to logic");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
 
  Serial.println("Timer set to 5 seconds (timerDelay variable), it will take 5 seconds before publishing the first reading.");
}

void loop() {
  // Send an HTTP POST request depending on timerDelay
  if ((millis() - lastTime) > timerDelay) {
    //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
//      digitalWrite(LED01, HIGH);
      WiFiClient client;
      HTTPClient http;

//      String serverPath = serverName;
      
      // Your Domain name with URL path or IP address with path
//      http.begin(client, serverPath.c_str());
      http.begin(client, serverName);
      
      // Send HTTP GET request
      int httpResponseCode = http.GET();
      
      if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.println(payload); 
        StaticJsonBuffer<200> jsonBuffer;

    JsonObject& root = jsonBuffer.parseObject(payload);


  relay1 = root["switch1"];
  relay2 = root["switch2"];
    
     
        if (relay1==1) {
          digitalWrite(LED1, HIGH);
        }
        if (relay1==0) {
          digitalWrite(LED1, LOW);
        }
        if (relay2==1) {
          digitalWrite(LED2, HIGH);
        }
        if (relay2==0) {
          digitalWrite(LED2, LOW);
        }
      }
      else {
        Serial.print("Error code1: ");
        Serial.println(httpResponseCode);
      }
      // Free resources
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}


    

//  if (request.indexOf("/LED=ON") != -1)
//  {
//    digitalWrite(LED, HIGH);
//    value = HIGH;
//  }
//  if (request.indexOf("/LED=OFF") != -1)
//  {
//    digitalWrite(LED, LOW);
//    value = LOW;
// }
