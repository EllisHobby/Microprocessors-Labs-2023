/*
  Author: Ellis Hobby
  Info:
    > SoundSensor class definitions
*/


#include "soundsensor.h"


SoundSensor::SoundSensor(void) {
  _FFT = arduinoFFT();  // create fft obj
}


void SoundSensor::init(void) {
  _initADC();
  _initTimer();
}



void SoundSensor::_initADC(void) {
  
  // set MUX channel
  ADMUX  = (1 << MUX2) | (1 << MUX1) | (1 << MUX0);
  ADCSRB = (1 << MUX5);

  // internal 1.1V ref
  ADMUX |= (1 << REFS1);
                            
  // enable ADC, clear any flags 
  ADCSRA = (1 << ADEN) | (1 << ADIF); 
  
  //prescale = 128 (ADCclk = 16e6 / 128 = 125 kHz)
  ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
  
  // disable digital output
  DIDR2 = (1 << ADC15D);
}


void SoundSensor::_initTimer(void) {
  // timer CTC mode
  TCCR4A = 0x00;
  TCCR4B = (1 << WGM42);
  TCCR4C = 0x00;
  
  // set output compare to sample count
  OCR4AH = (SAMPLE_CNT >> 8) & 0xFF;
  OCR4AL = (SAMPLE_CNT & 0xFF);

  // clear timer count 
  TCNT4H = 0; 
  TCNT4L = 0;
  
  // clear timer flags
  TIFR4 = 0xFF;
}


void SoundSensor::_compareFreq(void) {
  
  if ((_last_freq == NOTE_C4) && (_this_freq == NOTE_A4)) {
    _seq = SEQ_UP;
  }
  else if ((_last_freq == NOTE_A4) && (_this_freq == NOTE_C4)) {
    _seq = SEQ_DWN;
  }
  else {
    _seq = SEQ_NONE;
  }
  _last_freq = _this_freq;
}

uint8_t SoundSensor::senseFreq(void) {
  
  // clear timer flags
  TIFR4 = 0xFF;
  
  
  for (int16_t i=0; i < SAMPLES; i++) {
    
    // clear timer count 
    TCNT4H = 0; 
    TCNT4L = 0;
    
    // start timer
    TCCR4B |= (1 << CS40); 
  
    // start conversion
    ADCSRA |= (1 << ADSC);
  
    // wait for conversion to finish
    while(!(ADCSRA & (1 << ADIF)));
    ADCSRA |= (1 << ADIF);
    
    _real[i] = ADCW;
    _imag[i] = 0;
    

    // wait for output compare match
    while(!(TIFR4 & (1 << OCF4A)));
    TIFR4 = 0xFF;
    
    // disable timer4
    TCCR4B &= ~(1 << CS40);
  }
  
  _FFT.Windowing(_real, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  _FFT.Compute(_real, _imag, SAMPLES, FFT_FORWARD);
  _FFT.ComplexToMagnitude(_real, _imag, SAMPLES);
  _this_freq =  _FFT.MajorPeak(_real, SAMPLES, SAMPLE_FREQ);
  
  if ((_this_freq < UPPER_C4) && (_this_freq > LOWER_C4)) {
    _this_freq = NOTE_C4;
    _ticks = 0;
  }
  else if ((_this_freq < UPPER_A4) && (_this_freq > LOWER_A4)) {
    _this_freq = NOTE_A4;
    _ticks = 0;
  }
  else if (_ticks > 10) {
    _this_freq = 0;
    _ticks = 0;
  }
  else {
    _this_freq = _last_freq;
    _ticks++;
  }
  _compareFreq();
  return _seq;
}