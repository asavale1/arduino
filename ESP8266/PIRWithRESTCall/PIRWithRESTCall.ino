#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

const char *ssid = "Serengeti";
const char *password = "linguisticLemur";

const String host = "192.168.0.13:8080";

const int sensor1 = 13;
const int sensor2 = 12;
int sensor1State = LOW;
int sensor2State = LOW;

int sensorOrder[2] = { 0, 0 };

const int capacity=JSON_OBJECT_SIZE(2);
StaticJsonDocument<capacity> doc;
JsonObject obj = doc.to<JsonObject>();

void setup() {
  Serial.begin(9600);

  pinMode(sensor1, INPUT);
  pinMode(sensor2, INPUT);
  
  WiFi.mode(WIFI_OFF);
  delay(1000);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting!");

  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  detectMotion();
}

void detectMotion(){
  long sensor1CurrentState = digitalRead(sensor1);
  long sensor2CurrentState = digitalRead(sensor2);

  if(sensor1CurrentState == HIGH){
    if(sensor1State == LOW){
      Serial.println("Motion detected in sensor 1");
      sensor1State = HIGH;
      
      if(sensorOrder[0] == 0){
        sensorOrder[0] = 1;
      }else if(sensorOrder[1] == 0){
        sensorOrder[1] = 1;
      }
      
    }
  }else{
    if(sensor1State == HIGH){
      Serial.println("Motion ended in sensor 1");
      sensor1State = LOW;

      if(sensorOrder[0] == 1){
        sensorOrder[0] = 0;
      }else if(sensorOrder[1] == 1){
        sensorOrder[1] = 0;
      }
      
    }
  }

  if(sensor2CurrentState == HIGH){
    if(sensor2State == LOW){
      Serial.println("Motion detected in sensor 2");
      sensor2State = HIGH;

      if(sensorOrder[0] == 0){
        sensorOrder[0] = 2;
      }else if(sensorOrder[1] == 0){
        sensorOrder[1] = 2;
      }
      
    }
  }else{
    if(sensor2State == HIGH){
      Serial.println("Motion ended in sensor 2");
      sensor2State = LOW;

      if(sensorOrder[0] == 2){
        sensorOrder[0] = 0;
      }else if(sensorOrder[1] == 2){
        sensorOrder[1] = 0;
      }
    }
  }

  if(sensorOrder[0] == 1 && sensorOrder[1] == 2){
    Serial.println("Motion from sensor 1 to sensor 2");
    turnLightOn();
    delay(1500);
  }else if(sensorOrder[0] == 2 && sensorOrder[1] == 1){
    Serial.println("Motion from sensor 2 to sensor 1");
    turnLightOff();
    delay(1500);
  }
  
}

void turnLightOn(){
  WiFiClient client;
  HTTPClient http;

  Serial.print("[HTTP] begin...\n");
  if(http.begin(client, "http://" + host + "/sensors/bedroom")){
    Serial.println("[HTTP] POST...");
    http.addHeader("Content-Type", "application/json");

    obj["start"] = "sensor1";
    obj["end"] = "sensor2";
    
    String payload;
    serializeJson(doc, payload);
     
    int httpCode = http.POST(payload);
    if(httpCode > 0){
      Serial.printf("[HTTP] POST... code: %d\n", httpCode);
      Serial.println(http.getString());
    }else{
      Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str()); 
    }
    http.end();
  }else{
    Serial.println("[HTTP} Unable to connect");
  }
}


void turnLightOff(){
  WiFiClient client;
  HTTPClient http;

  Serial.print("[HTTP] begin...\n");
  if(http.begin(client, "http://" + host + "/sensors/bedroom")){
    Serial.println("[HTTP] POST...");
    http.addHeader("Content-Type", "application/json");

    obj["start"] = "sensor2";
    obj["end"] = "sensor1";
    
    String payload;
    serializeJson(doc, payload);
    
    int httpCode = http.POST(payload);
    if(httpCode > 0){
      Serial.printf("[HTTP] POST... code: %d\n", httpCode);
      Serial.println(http.getString());
    }else{
      Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str()); 
    }
    http.end();
  }else{
    Serial.println("[HTTP} Unable to connect");
  }
}
