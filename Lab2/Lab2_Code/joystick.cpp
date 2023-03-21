#include "joystick.hpp"




Joystick::Joystick (uint8_t x_ADC = ADC0, uint8_t y_ADC = ADC1) {

  // save ADC refs
  _x_ADC = x_ADC;
  _y_ADC = y_ADC;


  // using Vref = 2.56V and left adj
  ADMUX = (1 << REFS1) | (1 << REFS0) | (1 << ADLAR);
  
  // Timer/Counter1 Compare Match B as trig source
  ADCSRB = (1 << ADTS2) | (1 << ADTS0);                                 

  // enable ADC, enable auto-trig, prescale = 128 (ADCclk = 16e6 / 128 = 125 kHz)
  ADCSRA = (1 << ADEN) | (1 << ADATE) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

  // disable digital inputs
  if (!(x_ADC & 0b100000)
    DIDR0 = (1 << x_ADC);
  else
    DIDR1 = (1 << (x_ADC & 0b000111))
  
  if (!(y_ADC & 0b100000)
    DIDR0 = (1 << y_ADC);
  else
    DIDR1 = (1 << (y_ADC & 0b000111))

  // set MUX to read x first
  ADMUX = (ADMUX & ADMUX_MUX_MASK) | (_x_ADC);
  ADCSRB = (ADCSRB & ADCSRB_MUX_MASK) | (_x_ADC);

}
