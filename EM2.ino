const int EM = 9; 

void setup() {
  pinMode(EM, OUTPUT);
  digitalWrite(EM, LOW); 
}

void loop() {
  digitalWrite(EM, HIGH); 
  delay(5000);
  
  digitalWrite(EM, LOW);  
  delay(3000);
}
