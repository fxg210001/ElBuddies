#include <Wire.h>
#include "Adafruit_DRV2605.h"

Adafruit_DRV2605 drvQT;
Adafruit_DRV2605 drvPins;

void setup() {
  Serial.begin(9600);
  delay(2000);

  Serial.println("Dual DRV2605 Test");

  // DRV #1 - STEMMA QT / Qwiic
  if (!drvQT.begin(&Wire1)) {
    Serial.println("Could not find DRV2605 #1 on STEMMA QT");
    while (1) {
      delay(10);
    }
  }

  Serial.println("DRV2605 #1 found!");

  // DRV #2 - normal SDA/SCL pins
  if (!drvPins.begin(&Wire)) {
    Serial.println("Could not find DRV2605 #2 on SDA/SCL");
    while (1) {
      delay(10);
    }
  }

  Serial.println("DRV2605 #2 found!");

  // Configure both for LRA
  drvQT.useLRA();
  drvPins.useLRA();

  // LRA waveform library
  drvQT.selectLibrary(6);
  drvPins.selectLibrary(6);

  // Internal trigger mode
  drvQT.setMode(DRV2605_MODE_INTTRIG);
  drvPins.setMode(DRV2605_MODE_INTTRIG);

  Serial.println("Both DRV2605s found!");
}

uint8_t effect = 1;

void loop() {

  uint8_t effects[] = {
    4, 7, 10, 12, 14, 15, 17, 27, 44, 45, 47, 48, 49
  };

  static uint8_t effectIndex = 0;

  uint8_t effect = effects[effectIndex];

  Serial.print("Playing effect #");
  Serial.println(effect);

  // Load same effect into both DRV2605Ls
  drvQT.setWaveform(0, effect);
  drvQT.setWaveform(1, 0);

  drvPins.setWaveform(0, effect);
  drvPins.setWaveform(1, 0);

  // Trigger both
  drvQT.go();
  drvPins.go();

  delay(3000);

  effectIndex++;

  if (effectIndex >= 13) {
    effectIndex = 0;
  }
}