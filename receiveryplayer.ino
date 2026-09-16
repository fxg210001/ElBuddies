#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

SoftwareSerial mySoftwareSerial(0, 1); // RX, TX
DFRobotDFPlayerMini myDFPlayer;

// Keyfob receiver pins
const int ButtonB = 4; // D0 on receiver
const int ButtonD = 5; // D1 on receiver
const int ButtonA = 6; // D2 on receiver
const int ButtonC = 7; // D3 on receiver

// Simple debounce/edge tracking so a held button doesn't retrigger the track
bool lastA = false, lastB = false, lastC = false, lastD = false;

void setup() {

  pinMode(ButtonA, INPUT);
  pinMode(ButtonB, INPUT);
  pinMode(ButtonC, INPUT);
  pinMode(ButtonD, INPUT);

  Serial.begin(9600);
  mySoftwareSerial.begin(9600);

  Serial.println("Initializing DFPlayer...");
  if (!myDFPlayer.begin(mySoftwareSerial)) {
    Serial.println("DFPlayer not found - check wiring/SD card");
    while (true); 
  }
  Serial.println("DFPlayer ready");

  myDFPlayer.volume(20); // 0-30
}

void loop() {
  bool a = digitalRead(ButtonA) == HIGH;
  bool b = digitalRead(ButtonB) == HIGH;
  bool c = digitalRead(ButtonC) == HIGH;
  bool d = digitalRead(ButtonD) == HIGH;

  if (a && !lastA) {
    Serial.println("Button A -> track 1");
    myDFPlayer.play(1); // plays 0001.mp3
  }
  if (b && !lastB) {
    Serial.println("Button B -> track 2");
    myDFPlayer.play(2);
  }
  if (c && !lastC) {
    Serial.println("Button C -> track 3");
    myDFPlayer.play(3);
  }
  if (d && !lastD) {
    Serial.println("Button D -> track 4");
    myDFPlayer.play(4);
  }

  lastA = a; lastB = b; lastC = c; lastD = d;
  delay(20); // light debounce
}
