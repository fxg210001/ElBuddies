#include "Arduino.h"
#include "Serial.h"
#include <DFRobotDFPlayerMini.h>

DFRobotDFPlayerMini myDFPlayer;

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);

  Serial.println("initializing dfplayer mini");
  if (!myDFPlayer.begin(Serial1, true, false)) {
    Serial.println("unable to begin :-( check connections and sd card)");
    while (true);
  }

Serial.println("dfplayer mini online");
myDFPlayer.volume(20);
myDFPlayer.play(1);
}


void loop() {
}
