/*
  Author: Ellis Hobby
  Info:
    > Uses TWI obj for I2c communication
    > defines bit-field for control register tracking
    > defines TimeKeeping stuct for storing data
    > Square wave output connects to PE4
    > Square wave set to 1Hz, used for interval based interrupts (INT4)
*/


#ifndef RTC_H_
#define RTC_H_

#include "stdint.h"
#include "avr/io.h"
#include "twi.h"

#define SQ_PORT   PORTE
#define SQ_PIN    PE4
#define SQ_DDR    DDRE
#define SQ_READ   PINE
#define SQ_INT    INT4
#define SQ_EICR   EICRB
#define SQ_ISC1   ISC41
#define SQ_ISC0   ISC40

#define CLOCK_INT_VECT   INT4_vect     


#define DS3231_ADDR 0b1101000


/*-----------------------------

      time register map

  -----------------------------*/
#define SEC_ADDR  0x00
#define MIN_ADDR  0x01
#define HR_ADDR   0x02
#define DAY_ADDR  0x03
#define DATE_ADDR 0x04
#define MO_ADDR   0x05
#define YR_ADDR   0x06

#define HOUR_12   0x40
#define HOUR_24   0x00




/*-----------------------------

      control register help

  -----------------------------*/
#define CONTROL_REG_ADDR  0x07
typedef union _control {
  struct {
    uint8_t rs1   : 1;  // rate select 1
    uint8_t rs2   : 1;  // rate select 2
    uint8_t pad1_ : 2;
    uint8_t sqwe  : 1;
    uint8_t pad2_ : 2;  // temperature convert
    uint8_t out   : 1;  // batt backup sq wave
  };
  uint8_t instruction;

  // init to default settings per ds3231 data sheet
  _control() : rs1(1), rs2(1), pad1_(0), sqwe(0), pad2_(0), out(0) {};
} ControlRegister;



/*-----------------------------

      time keeping help

  -----------------------------*/
typedef struct _time {
  uint8_t sec   = 0;
  uint8_t min   = 0;
  uint8_t hr    = 0;
} TimeKeeping;



/*-----------------------------

      DS3231 RTC Class

  -----------------------------*/
class RTC {

  private:
    
    TWI _i2c;
    uint8_t _addr;
    TimeKeeping _raw;
    TimeKeeping _time;
    ControlRegister _control;
    void _writeControl(void);

  public:
    RTC(TWI twi);
    void init(void);
    TimeKeeping getTime(void);
    void sqOutOff(void);
    void sqOut1Hz(void);
    void sqOut4kHz(void);
    void sqOut8kHz(void);
    void sqOut32kHz(void);
};


#endif