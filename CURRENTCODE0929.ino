#include <Wire.h>
#include "Adafruit_DRV2605.h"
#include "Arduino.h"
#include "DFRobotDFPlayerMini.h"

Adafruit_DRV2605 drvQT;
Adafruit_DRV2605 drvPins;
DFRobotDFPlayerMini myDFPlayer;

// Keyfob receiver pins
const int ButtonB = 4; // D0 on receiver
const int ButtonD = 5; // D1 on receiver
const int ButtonA = 6; // D2 on receiver
const int ButtonC = 7; // D3 on receiver

const int magnet = 8;
const uint8_t HAPTIC_EFFECT = 4;

bool lastA = false, lastB = false, lastC = false, lastD = false;
bool magnetON = false;
uint8_t distress = 0;              // which case plays NEXT
bool dfReady = false;

// Non-blocking timer for turning the magnet off after the haptic pulse
bool magnetOffPending = false;
unsigned long magnetOffAt = 0;

void magnetHIGH() { magnetON = true;  digitalWrite(magnet, HIGH); }
void magnetLOW()  { magnetON = false; digitalWrite(magnet, LOW);  }

void initHaptics() {
  drvQT.begin();
  drvQT.selectLibrary(6);      // LRA library
  drvQT.useLRA();
  drvQT.setMode(DRV2605_MODE_INTTRIG);
  drvQT.setWaveform(0, HAPTIC_EFFECT);
  drvQT.setWaveform(1, 0);     // end of sequence

  drvPins.begin();
  drvPins.selectLibrary(6);
  drvPins.useLRA();
  drvPins.setMode(DRV2605_MODE_INTTRIG);
  drvPins.setWaveform(0, HAPTIC_EFFECT);
  drvPins.setWaveform(1, 0);

  //  if (!drvQT.begin(&Wire1)) {
  //   Serial.println("Could not find QT DRV2605L");
  // } else {
  //   Serial.println("QT DRV2605L found");
  //   drvQT.selectLibrary(6);
  //   drvQT.useLRA();
  //   drvQT.setMode(DRV2605_MODE_INTTRIG);
  //   drvQT.setWaveform(0, HAPTIC_EFFECT);
  //   drvQT.setWaveform(1, 0);
  // }

  // // SDA/SCL pin DRV2605L
  // if (!drvPins.begin(&Wire)) {
  //   Serial.println("Could not find pin DRV2605L");
  // } else {
  //   Serial.println("Pin DRV2605L found");
  //   drvPins.selectLibrary(6);
  //   drvPins.useLRA();
  //   drvPins.setMode(DRV2605_MODE_INTTRIG);
  //   drvPins.setWaveform(0, HAPTIC_EFFECT);
  //   drvPins.setWaveform(1, 0);
  // }
}

void setup() {
  pinMode(ButtonA, INPUT);
  pinMode(ButtonB, INPUT);
  pinMode(ButtonC, INPUT);
  pinMode(ButtonD, INPUT);
  pinMode(magnet, OUTPUT);
  digitalWrite(magnet, LOW);

  Serial.begin(9600);
  Serial1.begin(9600);
  Wire.begin();
  Wire1.begin();
  delay(2000);

  initHaptics();

  // Reset the DFPlayer (3rd arg true) and give it time to read the SD card
  if (!myDFPlayer.begin(Serial1, true, true)) {
    Serial.println("DFPlayer failed /:  check wiring, 1k resistor on Arduino TX, SD card");
  } else {
    dfReady = true;
    myDFPlayer.volume(20);
    delay(500);
  }
}

void loop() {
  bool a = digitalRead(ButtonA) == HIGH;
  bool b = digitalRead(ButtonB) == HIGH;
  bool c = digitalRead(ButtonC) == HIGH;
  bool d = digitalRead(ButtonD) == HIGH;

  // A: magnet on
  if (a && !lastA) {
    magnetHIGH();
  }

  // B: cycle 1 -> 5 -> stop (this order, starting with track 1)
  if (b && !lastB && dfReady) {
    switch (distress) {
      case 0: myDFPlayer.play(1); break;
      case 1: myDFPlayer.play(3); break;
      case 2: myDFPlayer.stop();  break;
    }
    distress = (distress + 1) % 3;
  }

  // C: haptics now, magnet off 2 s later (no blocking delay)
  if (c && !lastC) {
    drvQT.go();
    drvPins.go();
    magnetOffPending = true;
    magnetOffAt = millis() + 2000;
  }

  if (magnetOffPending && (long)(millis() - magnetOffAt) >= 0) {
    magnetLOW();
    magnetOffPending = false;
    // Re-arm the drivers in case the magnet switching glitched the I2C bus / DRV
    initHaptics();
  }

  lastA = a; lastB = b; lastC = c; lastD = d;
  delay(20);  // short loop so quick keyfob pulses aren't missed
}
