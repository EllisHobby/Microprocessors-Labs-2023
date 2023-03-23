#include "Arduino.h"
#include "analog_joystick.h"




JoyStick::JoyStick (uint8_t x_chan = ADC0, uint8_t y_chan = ADC1) {

  debug_ = DEBUG_OFF;

  // save ADC refs
  x_chan_ = x_chan;
  y_chan_ = y_chan;
  
  // init buffer
  control_buf_.x = 0;
  control_buf_.y = 0;
  
  // Vref = 2.56V, left adjust (8-bit resolution)
  ADMUX = (1 << REFS1) | (1 << REFS0) | (1 << ADLAR);                            

  // enable ADC, clear any flags 
  ADCSRA = (1 << ADEN) | (1 << ADIF); 
  
  //prescale = 128 (ADCclk = 16e6 / 128 = 125 kHz)
  ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

  // disable digital inputs
  if (!(x_chan_ & 0b100000))
    DIDR0 = (1 << x_chan_);
  else
    DIDR1 = (1 << (x_chan_ & ~ADMUX_MUX_MASK));
  
  if (!(y_chan_ & 0b100000))
    DIDR0 |= (1 << y_chan_);
  else
    DIDR1 |= (1 << (y_chan_ & ~ADMUX_MUX_MASK));

}




uint16_t JoyStick::readAxis_ (uint8_t chan) {

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
  
  control_buf_.x = readAxis_(x_chan_);
  control_buf_.y = readAxis_(y_chan_);

  control_buf_.x -= 512;
  control_buf_.y -= 512;

  if (debug_ == DEBUG_CONTROL) {
    Serial.println((String) "[Control] x: " + control_buf_.x + ", y: " + control_buf_.y + ", z: " + control_buf_.z);
  }
  return control_buf_;
}