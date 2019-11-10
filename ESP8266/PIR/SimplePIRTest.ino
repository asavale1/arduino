int sensor = 13;

void setup() {
  pinMode(sensor, INPUT);
  Serial.begin(9600);
}

void loop() {
  long state = digitalRead(sensor);
  if(state == HIGH){
    Serial.println("Motion Detected!");
    delay(1000);
  }else{
    Serial.println("Motion absent!");
    delay(1000);
  }
}
