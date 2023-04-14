/*
    Author: Ellis Hobby
    
    Test fan functionality:
    > cycle speed once per second
    > print fan state: speed, direction
    > change direction every four seconds
    > pc int to change rotation/speed
*/

#include "util/delay.h"
#include "pwm1.h"

PWM1 fan;

// pc int for fan control
volatile uint8_t last_control = 0xFF;
ISR (CONTROL_INT_VECT) {
  
  uint8_t pin_change = CONTROL_READ ^ last_control;
  last_control = CONTROL_READ;

  // nested vector for multi use
  if (pin_change & (1 << SPEED_UP_PIN)) {
    if (!(CONTROL_READ & (1 << SPEED_UP_PIN))) {
      fan.speedUp();  
    }     
  }
  else if (pin_change & (1 << SPEED_DWN_PIN)) {
    if (!(CONTROL_READ & (1 << SPEED_DWN_PIN))) {
      fan.speedDown();
    }     
  }
  else if (pin_change & (1 << ROTATION_PIN)) {
    if (!(CONTROL_READ & (1 << ROTATION_PIN))) {
      fan.switchRotation();
    }   
  }
}

void setup() {
  
  Serial.begin(9600);
  _delay_ms(100);
  
  Serial.println("Starting Fan PWM Test....");
  Serial.println("-------------------------\n");
  _delay_ms(1000);

  fan.init();
}

void loop() {
  
  for(uint8_t i=0; i < 4; i++) {
    fan.speedUp();
    printFanState(fan.getSpeedLabel(), fan.getRotationLabel());
    _delay_ms(5000);
  }
  fan.switchRotation();
  
  for(uint8_t i=0; i < 4; i++) {
    fan.speedDown();
    printFanState(fan.getSpeedLabel(), fan.getRotationLabel());
    _delay_ms(5000);
  }
  fan.switchRotation();
}


void printFanState(const char* speed, const char* rotation) {
  Serial.println((String)speed + " " + rotation);
}