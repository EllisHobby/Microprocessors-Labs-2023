/*
  Author: Ellis Hobby
  Info:
    > RTC class definition
*/

#include "rtc.h"


RTC::RTC(TWI twi) {
  _i2c = twi;
  _addr = DS3231_ADDR;
}
void RTC::init(void) {

  // sq wave port input, pull-up enable
  SQ_PORT |=  (1 << SQ_PIN);
  SQ_DDR  &= ~(1 << SQ_PIN);

  // sq wave ext interrupt on rising edge
  SQ_EICR |= (1 << SQ_ISC1) | (1 << SQ_ISC0);

  // enable sq wave interrupt
  EIMSK |= (1 << SQ_INT);

  // enable sq wave 1Hz
  // will trigger ext int
  sqOut1Hz();
}


TimeKeeping RTC::getTime(void) {

  _i2c.start();         
  _i2c.slaveRX(_addr);        
  _i2c.write(SEC_ADDR);   
  _i2c.stop();          
  
  _i2c.start();          
  _i2c.slaveTX(_addr);        
  
  // read raw time
  _raw.sec = _i2c.readACK();    // seconds 0-59
  _raw.min = _i2c.readACK();    // minutes 0-59
  _raw.hr  = _i2c.readNACK();   // hours 1-12 or 0-23
  _i2c.stop();
  
  // convert raw time
  _time.sec = (((_raw.sec & 0x70) >> 4) * 10) + (_raw.sec & 0x0F);
  _time.min = (((_raw.min & 0x70) >> 4) * 10) + (_raw.min & 0x0F);
  if (_raw.hr && HOUR_12) {
    _time.hr = (((_raw.hr & 0x10) >> 4) * 10) + (_raw.hr & 0x0F);
  }
  else {
    _time.hr = (((_raw.hr & 0x30) >> 4) * 10) + (_raw.hr & 0x0F);
  }
  return _time;
}

void RTC::_writeControl(void) {
  _i2c.start();         
  _i2c.slaveRX(_addr);        
  _i2c.write(CONTROL_REG_ADDR);   
  _i2c.write(_control.instruction);
  _i2c.stop();     
}

void RTC::sqOutOff(void) {
  _control.sqwe = 0;
  _writeControl();
}
void RTC::sqOut1Hz(void) {
  _control.sqwe = 1;
  _control.rs1  = 0;
  _control.rs2  = 0;
  _writeControl();
}
void RTC::sqOut4kHz(void) {
  _control.sqwe = 1;
  _control.rs1  = 1;
  _control.rs2  = 0;
  _writeControl();
}
void RTC::sqOut8kHz(void) {
  _control.sqwe = 1;
  _control.rs1  = 0;
  _control.rs2  = 1;
  _writeControl();
}
void RTC::sqOut32kHz(void) {
  _control.sqwe = 1;
  _control.rs1  = 1;
  _control.rs2  = 1;
  _writeControl();
}

