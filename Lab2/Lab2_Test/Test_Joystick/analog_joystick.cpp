#include "analog_joystick.h"




JoyStick::JoyStick (uint8_t x_chan = ADC0, uint8_t y_chan = ADC1) {

  // save ADC refs
  _x_chan = x_chan;
  _y_chan = y_chan;
  
  // init buffer
  _buf.x = 0;
  _buf.y = 0;
  
  // Vref = 2.56V, left adjust (8-bit resolution)
  ADMUX = (1 << REFS1) | (1 << REFS0) | (1 << ADLAR);
  
  // Timer/Counter1 Compare Match B as trig source
  //ADCSRB = (1 << ADTS2) | (1 << ADTS0);                                 

  // enable ADC, clear any flags 
  ADCSRA = (1 << ADEN) | (1 << ADIF); 
  
  //prescale = 128 (ADCclk = 16e6 / 128 = 125 kHz)
  ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

  // disable digital inputs
  if (!(_x_chan & 0b100000))
    DIDR0 = (1 << _x_chan);
  else
    DIDR1 = (1 << (_x_chan & ~ADMUX_MUX_MASK));
  
  if (!(_y_chan & 0b100000))
    DIDR0 |= (1 << _y_chan);
  else
    DIDR1 |= (1 << (_y_chan & ~ADMUX_MUX_MASK));

  // set MUX to read x first
  //ADMUX = (ADMUX & ADMUX_MUX_MASK) | (_x_ADC);
  //ADCSRB = (ADCSRB & ADCSRB_MUX_MASK) | (_x_ADC);

}




uint16_t JoyStick::_read_axis (uint8_t chan) {

  // set MUX channel
  ADMUX = (ADMUX & ADMUX_MUX_MASK) | (chan & ~ADMUX_MUX_MASK);
  ADCSRB = (ADCSRB & ADCSRB_MUX_MASK) | (chan & ~ADCSRB_MUX_MASK);

  // start conversion
  ADCSRA |= (1 << ADSC);

  // wait for conversion to finish
  while(!(ADCSRA & (1 << ADIF)));
  ADCSRA |= (1 << ADIF);
  
  
  
  return (ADCW >> 6);
}




Axis JoyStick::read (void) {
  
  _buf.x = _read_axis(_x_chan);
  _buf.y = _read_axis(_y_chan);
  return _buf;
}
