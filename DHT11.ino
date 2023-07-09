#include "DHT.h"
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  float suhu = dht.readTemperature();
  float lembab = dht.readHumidity();
  Serial.println("Suhu = "+String(suhu)+" Lembab = "+String(lembab));
}
