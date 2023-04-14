/*
  Author: Ellis Hobby

  Info:
    > LCD output for system state info
      - Clock (hr:min:sec)
      - Fan speed (off, 1/2, 1/4, max)
      - Fan rotation (CW, CCW)
      
    > RTC updates clock every second    
      - synced to RTC by 1Hz square wave interrupt

    > Sound sensor controls fan speed using freq patterns
      - C4 (261Hz) to A4 (440Hz) speed up
      - A4 (440Hz) to C4 (262Hz) speed down
      - 2% error margin for frequency input
*/
#include "rtc.h"
#include "twi.h"
#include "pwm1.h"
#include "screen.h"
#include "soundsensor.h"


// create objs
LCD lcd;
PWM1 fan;
TWI i2c;
RTC clock(i2c);
SoundSensor sound;

// storage for clock data
TimeKeeping timer;


// ext int once per second
// get time update from RTC
ISR (CLOCK_INT_VECT) {
  timer = clock.getTime();
}


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


void setup(void) {
  
  // no interrupts
  cli();
  
  // initialize objs
  lcd.init();
  i2c.init();
  fan.init();
  clock.init();
  sound.init();

  // en interrupts
  sei();
}

void loop(void) {

  // read input freq
  sound.senseFreq();

  // if freq pattern match control fan speed
  if(sound.getSeq() == SEQ_UP) {
    fan.speedUp();
  }
  else if (sound.getSeq() == SEQ_DWN) {
    fan.speedDown();
  }
  
  // update display
  updateLCD();
}


void updateLCD(void) {
  
  // buffers for display formatting
  char buffer[50];
  char sec_buff[3];
  char min_buff[3];
  char hr_buff[3];

  // no interrupts
  cli();
  
  // format clock display as 00:00:00
  if (timer.sec < 10) {
    sprintf(sec_buff, "0%u", timer.sec);
  }
  else {
    sprintf(sec_buff, "%u", timer.sec);
  }
  if (timer.min < 10) {
    sprintf(min_buff, "0%u", timer.min);
  }
  else {
    sprintf(min_buff, "%u", timer.min);
  }
  if (timer.hr < 10) {
    sprintf(hr_buff, "0%u", timer.hr);
  }
  else {
    sprintf(hr_buff, "%u", timer.hr);
  }

  // create clock string
  sprintf(buffer, "%s:%s:%s", hr_buff, min_buff, sec_buff);
  
  // display clock at beginning of LCD line one
  lcd.gotoLineOne();
  lcd.write(buffer);

  // get rotation string, display at end of LCD line one
  sprintf(buffer, "R:%s", fan.getRotationLabel());
  lcd.gotoPosition(END_LINE_ONE - strlen(buffer) + 1);
  lcd.write(buffer);   

  // get speed string, display at end of LCD line two
  sprintf(buffer, "S:%s", fan.getSpeedLabel());
  lcd.gotoPosition(END_LINE_TWO - strlen(buffer) + 1);
  lcd.write(buffer);

  // en interrupts
  sei();
}



