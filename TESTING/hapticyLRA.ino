#include <Wire.h>
#include "Adafruit_DRV2605.h"

Adafruit_DRV2605 drvQT;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  //Wire1.begin();
  delay(2000);

  //DRV #1 - STEMMA QT / Qwiic
  if (!drvQT.begin(&Wire)) {
    Serial.println("Could not find DRV2605 #1 on STEMMA QT");
    while (1) {
      delay(10);
    }
  }

  Serial.println("DRV2605 #1 found!");

  drvQT.useLRA();
  drvQT.selectLibrary(6);

  drvQT.setMode(DRV2605_MODE_INTTRIG);
}

uint8_t effect = 1;

void loop() {

  uint8_t effects[] = {
    47, 48
  };

  static uint8_t effectIndex = 0;

  uint8_t effect = effects[effectIndex];

  Serial.print("Playing effect #");
  Serial.println(effect);

  drvQT.setWaveform(0, effect);
  drvQT.setWaveform(1, 0);
  drvQT.go();
  
  delay(3000);

  effectIndex++;

}
