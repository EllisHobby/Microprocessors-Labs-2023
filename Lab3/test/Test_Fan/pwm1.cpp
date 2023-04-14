/*
  Author: Ellis Hobby
  Info:
    > PWM1 class definitions
*/

#include "pwm1.h"


void PWM1::init(void) {
  // set output driver pins for L293
  // initialize to CW rotation
  // driver 1 low and driver 2 high
  DRIVER_1_DDR  |=  (1 << DRIVER_1_PIN);
  DRIVER_1_PORT &= ~(1 << DRIVER_1_PIN);
  DRIVER_2_DDR  |=  (1 << DRIVER_1_PIN);
  DRIVER_2_PORT |=  (1 << DRIVER_1_PIN);

  // set fan PWM pin for output
  PWM_DDR  |=  (1 << PWM_PIN);
  PWM_PORT &= ~(1 << PWM_PIN);

  // set speed up control sw input
  SPEED_UP_PORT |= (1 << SPEED_UP_PIN);
  SPEED_UP_DDR  &= ~(1 << SPEED_UP_PIN);

  // set speed down control sw input
  SPEED_DWN_PORT |= (1 << SPEED_DWN_PIN);
  SPEED_DWN_DDR  &= ~(1 << SPEED_DWN_PIN);

  // set rotation control sw input
  ROTATION_PORT |= (1 << ROTATION_PIN);
  ROTATION_DDR  &= ~(1 << ROTATION_PIN);

  // enable pc int for control switches
  CONTROL_ICR |= (1 << CONTROL_IE);
  CONTROL_MSK |= ((1 << SPEED_UP_INT) | (1 << SPEED_DWN_INT) | (1 << ROTATION_INT));

  // set Timer1 for fast pwm 9-bit (TOP=0x01FF)
  // use OCR1A (PB7) as non-inverting output
  // set clock prescale (N=64)
  //    ->  Fpwm = (Fcpu) / (N(1+TOP)) = (16e6) / (64 * 512) = 488 Hz
  TCCR1A = (1 << COM1A1) | (1 << WGM11);
  TCCR1B = (1 << WGM12) | (1 << CS11) | (1 << CS10);

  // set the output compare for PWM
  // initial setting is off (i.e 0x00)
  OCR1AH = ((_current_speed >> 8) & 0x01);
  OCR1AL = (_current_speed & 0x0FE);
}


void PWM1::_setSpeed(void) {
  
  // get speed value from index
  _current_speed = _speed[_speed_idx];

  // set output compare
  OCR1AH = ((_current_speed >> 8) & 0x01);
  OCR1AL = (_current_speed & 0x0FE);
}


void PWM1::setOff(void) {
  _speed_idx = INDEX_OFF;
  _setSpeed();
}

void PWM1::setHalf(void) {
  _speed_idx = INDEX_HALF;
  _setSpeed();
}

void PWM1::set3Quarter(void) {
  _speed_idx = INDEX_3_QU;
  _setSpeed();
}

void PWM1::setMax(void) {
  _speed_idx = INDEX_MAX;
  _setSpeed();
}

void PWM1::speedUp(void) {

  // mod 4 stays within array bouds
  _speed_idx = (_speed_idx + 1) % 4;
  _setSpeed();
}


void PWM1::speedDown(void) {
  
  if (_speed_idx == INDEX_OFF) {
    _speed_idx = INDEX_MAX;
  }
  else {
    // mod 4 stays within array bouds
    _speed_idx = (_speed_idx - 1) % 4;    
  }
  _setSpeed();
}


void PWM1::switchRotation(void) {

  // update rotation state
  _fan_rotation = !(_fan_rotation);

  // for CW set driver1 low, driver2 high
  if (_fan_rotation == CW) {
    DRIVER_1_PORT &= ~(1 << DRIVER_1_PIN);
    DRIVER_2_PORT |=  (1 << DRIVER_2_PIN);
  }

  // for CCW set driver1 high, driver2 low 
  else {
    DRIVER_1_PORT |=  (1 << DRIVER_1_PIN);
    DRIVER_2_PORT &= ~(1 << DRIVER_2_PIN);
  }
}


const char* PWM1::getRotationLabel(void) {

  if (_fan_rotation == CW) { return "CW "; }

  else { return "CCW"; }
}


const char* PWM1::getSpeedLabel (void) {
  
  switch(_current_speed) {
    case SPEED_OFF:
      return "off";
    case SPEED_HALF:
      return "1/2";
    case SPEED_3_QU:
      return "3/4";
    case SPEED_MAX:
      return "max";
    default:
      return "err";
  }
}