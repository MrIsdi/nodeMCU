void setup(){
  Serial.begin(9600);
  pinMode(16, OUTPUT);
}

void loop(){
  float res = analogRead(A0);
  float lux = pow(res, -1.7118)*pow(10, 5.99368);
  Serial.print("Nilai intensitas cahaya : ");
  Serial.print(lux);
  Serial.println(" lux");

  digitalWrite(16, 1);

  delay(1000);
}