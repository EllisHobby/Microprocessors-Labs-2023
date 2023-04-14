/*
  Author: Ellis Hobby
  Info:
    > uses Timer1 for PWM output on PB5
    > L293 driver 1 connected to PH5
    > L293 driver 2 connected to PH6
    > PCINT0 for speed and rotation control
    > speed up button connected to PB0 (PCINT0)
    > speed down button connected to PB1 (PCINT1)
    > rotation button connected to PB2 (PCINT2)
*/


#ifndef PWM1_H_
#define PWM1_H_

#include "avr/io.h"

#define PWM_PORT  PORTB
#define PWM_DDR   DDRB
#define PWM_PIN   PB5

#define DRIVER_1_PORT PORTH
#define DRIVER_1_DDR  DDRH
#define DRIVER_1_PIN  PH5

#define DRIVER_2_PORT PORTH
#define DRIVER_2_DDR  DDRH
#define DRIVER_2_PIN  PH6

#define SPEED_UP_PORT PORTB  
#define SPEED_UP_DDR  DDRB 
#define SPEED_UP_PIN  PB0
#define SPEED_UP_READ PINB
#define SPEED_UP_INT  PCINT0

#define SPEED_DWN_PORT PORTB  
#define SPEED_DWN_DDR  DDRB 
#define SPEED_DWN_PIN  PB1
#define SPEED_DWN_READ PINB
#define SPEED_DWN_INT  PCINT1

#define ROTATION_PORT PORTB  
#define ROTATION_DDR  DDRB 
#define ROTATION_PIN  PB2
#define ROTATION_READ PINB
#define ROTATION_INT  PCINT2

#define CONTROL_ICR   PCICR
#define CONTROL_IE    PCIE0
#define CONTROL_IFR   PCIFR
#define CONTROL_MSK   PCMSK0
#define CONTROL_READ  PINB

#define CONTROL_INT_VECT  PCINT0_vect

#define SPEED_OFF         0x0000    // off
#define SPEED_HALF        0x0100    // 256/512 * 100 = 50%
#define SPEED_3_QU        0x0180    // 384/512 * 100 = 75%
#define SPEED_MAX         0x01FE    // 510/512 * 100 = 99% (easier to view on oscilloscope)

#define INDEX_OFF         0   
#define INDEX_HALF        1    
#define INDEX_3_QU        2    
#define INDEX_MAX         3    

#define CW    false
#define CCW   true


class PWM1{

  private:
    
    const uint16_t _speed[4] = {
      SPEED_OFF, SPEED_HALF, SPEED_3_QU, SPEED_MAX
    };
    int8_t _speed_idx = 0;
    uint16_t _current_speed = _speed[_speed_idx];
    bool _fan_rotation = CW;
  
    void _setSpeed(void);

  public:
    void init(void);
    void setOff(void);
    void setHalf(void);
    void set3Quarter(void);
    void setMax(void);
    void speedUp(void);
    void speedDown(void);
    void switchRotation(void);
    const char* getRotationLabel(void);
    const char* getSpeedLabel(void);
};

#endif
