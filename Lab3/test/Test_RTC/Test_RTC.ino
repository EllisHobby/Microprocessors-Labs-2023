/*
  Author: Ellis Hobby
  Info:
    > gets time from RTC once per second
    > prints to console
*/
#include "rtc.h"

TWI i2c;
RTC clock(i2c);
TimeKeeping timer;

bool printTime = false;

ISR (CLOCK_INT_VECT) {
  printTime = true;
}

void setup() {
  
  Serial.begin(9600);
  _delay_ms(100);
  
  Serial.println("Starting RTC Test....");
  Serial.println("-------------------------\n");
  _delay_ms(1000);

  i2c.init();
  clock.init();
}

void loop() {

  if (printTime) {
    printTime = false;
    timer = clock.getTime();
    Serial.println((String) timer.sec  + "sec");
    Serial.println((String) timer.min  + "min");
    Serial.println((String) timer.hr   + "hrs\n");
  }
}
