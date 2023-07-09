int kipas = 16;
int mist = 5;

void setup(){
  pinMode(kipas, OUTPUT);
  pinMode(mist, OUTPUT);
}

void loop(){
  relai(0);
}

void relai(int aktif){
  digitalWrite(kipas, aktif);
  digitalWrite(mist, aktif);
}