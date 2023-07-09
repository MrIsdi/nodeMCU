#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>
#include "DHT.h"
#include <ArduinoJson.h>

#define DHTPIN 2
#define DHTTYPE DHT11

int KIPASPIN = 5, MISTPIN = 4, LED1 = 12, LED2 = 13, LED3 = 15, LDRPIN = A0;

const char* ssid = "MRISDI";
const char* password = "1sampai8";

DHT dht(DHTPIN, DHTTYPE);
void setup() {
  Serial.begin(115200);
  dht.begin();
  Serial.println();
  Serial.println();
  Serial.println();

  pinMode(KIPASPIN, OUTPUT);
  pinMode(MISTPIN, OUTPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
}

void loop() {
  digitalWrite(LED1, 1);
  digitalWrite(LED2, 1);
  digitalWrite(LED3, 1);
  
  float suhu = dht.readTemperature();
  float lembap = dht.readHumidity();
  float res = analogRead(LDRPIN);
  float cahaya = pow(res, -1.7118)*pow(10, 5.99368);

  String kipas, mist;

  if(suhu > 28){
    digitalWrite(KIPASPIN, 0);
    kipas = "Hidup";
  }else{
    digitalWrite(KIPASPIN, 1);
    kipas = "Mati";
  }

  if(lembap < 80){
    digitalWrite(MISTPIN, 0);
    mist = "Hidup";
  }else{
    digitalWrite(MISTPIN, 1);
    mist = "Mati";
  }
  
  // Membuat objek DynamicJsonDocument
  DynamicJsonDocument jsonDoc(1024);
  String jsonString;

  // Menambahkan data ke JSON
  jsonDoc["suhu"] = String(suhu);
  jsonDoc["lembap"] = String(lembap);
  jsonDoc["cahaya"] = String(cahaya);
  jsonDoc["kipas"] = kipas;
  jsonDoc["mist"] = mist;

  // Mengubah JSON menjadi String
  serializeJson(jsonDoc, jsonString);
  
  // wait for WiFi connection
  if ((WiFi.status() == WL_CONNECTED)) {

    std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);

    // Ignore SSL certificate validation
    client->setInsecure();
    
    //create an HTTPClient instance
    HTTPClient https;
    
    //Initializing an HTTPS communication using the secure client
    Serial.print("[HTTPS] begin...\n");
    if (https.begin(*client, "https://cvtomato.000webhostapp.com/sensor")) {  // HTTPS
      Serial.print("[HTTPS] POST...\n");
      // start connection and send HTTP header
      https.addHeader("Content-Type", "application/json");
      int httpCode = https.POST(jsonString);
      // httpCode will be negative on error
      if (httpCode > 0) {
        // HTTP header has been send and Server response header has been handled
        Serial.printf("[HTTPS] POST... code: %d\n", httpCode);
        // file found at server
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          String payload = https.getString();
          Serial.println(payload);
        }
      } else {
        Serial.printf("[HTTPS] POST... failed, error: %s\n", https.errorToString(httpCode).c_str());
      }

      https.end();
    } else {
      Serial.printf("[HTTPS] Unable to connect\n");
    }
  }
  Serial.println();
  Serial.println(jsonString);
  Serial.println("Waiting 1 hour before the next round...");
  delay(3600000);
}
