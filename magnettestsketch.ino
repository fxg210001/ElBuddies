char input;

void setup() {

  Serial.begin(9600);

  pinMode(8, OUTPUT);
  digitalWrite(8, LOW);   // Magnet starts OFF

}

void loop() {

  Serial.println("Do you want to turn the magnet on? Y/N:");

  while (Serial.available() == 0) {
    // Wait for user input
  }

  input = Serial.read();

  if (input == 'Y' || input == 'y') {

    digitalWrite(8, HIGH);   // Magnet ON

    delay(3000);            // Keep on for 30 seconds

    digitalWrite(8, LOW);    // Magnet OFF

  }

}
