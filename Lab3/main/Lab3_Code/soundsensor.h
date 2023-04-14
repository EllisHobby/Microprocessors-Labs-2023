/*
  Author: Ellis Hobby
  Info:
    > Sound Sensor uses ADC15 (PK7)
    > Timer4 used for setting sample rate
    > arduinoFFT object used to detect freq
*/


#ifndef SOUNDSENSOR_H_
#define SOUNDSENSOR_H_

#include "arduinoFFT.h"



/*------------------------

  FFT definitions
    
------------------------*/
#define SAMPLES       64
#define SAMPLE_FREQ   2000
#define SAMPLE_CNT    round(F_CPU / (SAMPLE_FREQ))



/*------------------------

  freq check definitions
    
------------------------*/
#define NOTE_C4     262
#define NOTE_A4     440
#define UPPER_C4    (NOTE_C4 * 1.02)
#define UPPER_A4    (NOTE_A4 * 1.02)
#define LOWER_C4    (NOTE_C4 * 0.98)
#define LOWER_A4    (NOTE_A4 * 0.98)
#define SEQ_NONE    0
#define SEQ_UP      1
#define SEQ_DWN     2



/*------------------------

  sound sensor class
    
------------------------*/
class SoundSensor {
  
  private:
    double _real[SAMPLES];
    double _imag[SAMPLES];
    arduinoFFT _FFT;
    double _last_freq = 0;
    double _this_freq = 0;
    uint8_t _ticks = 0;
    uint8_t _seq = SEQ_NONE;
    
    void _initADC(void);
    void _initTimer(void);
    void _compareFreq(void);
  
  public:
    SoundSensor(void);
    void init(void);
    uint8_t senseFreq(void); 
    uint8_t getSeq(void) { return _seq; }
    double getFreq(void) { return _this_freq; }
};

#endif