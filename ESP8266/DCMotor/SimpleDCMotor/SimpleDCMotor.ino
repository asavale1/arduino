#define DIRA_M1 2
#define PWMA_M1 4
#define DIRA_M2 0
#define PWMA_M2 5
#define SPEED 1000

void setup() {
  Serial.begin(9600);
  Serial.println();
  Serial.println("Starting...");

  Serial.println("Preparing motor...");
  pinMode(PWMA_M1, OUTPUT);
  pinMode(DIRA_M1, OUTPUT);
  pinMode(PWMA_M2, OUTPUT);
  pinMode(DIRA_M2, OUTPUT);

  analogWrite(PWMA_M1, 0);  
  digitalWrite(DIRA_M1, 1);

  analogWrite(PWMA_M2, 0);  
  digitalWrite(DIRA_M2, 0);
  
  delay(5000);

  
  
}

void loop() {
  Serial.println("Starting motor...");
  digitalWrite(DIRA_M1, 1);
  digitalWrite(DIRA_M2, 0);
  analogWrite(PWMA_M1, SPEED);
  analogWrite(PWMA_M2, SPEED);
  delay(200);
  Serial.println("Stopping motor...");
  analogWrite(PWMA_M1, 0);
  analogWrite(PWMA_M2, 0);
  
  Serial.println("Switch direction");
  delay(2000);
  
  Serial.println("Starting motor...");
  digitalWrite(DIRA_M1, 0);
  digitalWrite(DIRA_M2, 1);
  analogWrite(PWMA_M1, SPEED);
  analogWrite(PWMA_M2, SPEED);
  delay(200);
  Serial.println("Stopping motor...");
  analogWrite(PWMA_M1, 0);
  analogWrite(PWMA_M2, 0);
  delay(2000);
}
