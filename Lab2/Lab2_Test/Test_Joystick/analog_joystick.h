#ifndef JOYSTICK_H
#define JOYSTICK_H

#define ADC0    0b000000
#define ADC1    0b000001
#define ADC2    0b000010
#define ADC3    0b000011
#define ADC4    0b000100
#define ADC5    0b000101
#define ADC6    0b000110
#define ADC7    0b000111
#define ADC8    0b100000
#define ADC9    0b100001
#define ADC10   0b100010
#define ADC11   0b100011
#define ADC12   0b100100
#define ADC13   0b100101
#define ADC14   0b100110
#define ADC15   0b100111

#define ADMUX_MUX_MASK      0b11100000
#define ADCSRB_MUX_MASK     0b11110111

#define ADC_CHAN_AMT    2


#include "Arduino.h"

typedef struct {
  uint16_t x = 0;
  uint16_t y = 0;
  uint16_t z = 0;
}Axis;

class JoyStick {
  
  private:
    Axis _buf;
    uint8_t _x_chan, _y_chan;
    uint16_t _read_axis (uint8_t chan);

  public:
    JoyStick(uint8_t x_ADC = ADC0, uint8_t y_ADC = ADC1);
    Axis read (void);
};


#endif
