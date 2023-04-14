/*
  Author: Ellis Hobby
  Info:
    > reads sound sensor input
    > tests for up/down patterns
*/

#include "soundsensor.h"

SoundSensor sound;

void setup() {

  Serial.begin(9600);
  _delay_ms(100);

  Serial.println("Testing Sound Sense");
  Serial.println("-------------------\n");
  sound.init();

  sound.init();
}

void loop() {
  
  uint8_t seq = sound.senseFreq();

  if (seq == SEQ_UP) {
    Serial.println("Sequence Up");
  }
  else if (seq == SEQ_DWN) {
    Serial.println("Sequence Down");
  }
  _delay_ms(100);
}
