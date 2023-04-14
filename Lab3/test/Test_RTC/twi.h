/*
  Author: Ellis Hobby
  Info:
    > TWI class
*/

#ifndef TWI_H_
#define TWI_H_

#include "stdint.h"
#include "avr/io.h"
#include "util/twi.h"


#define TWBR_400KHZ 12

class TWI {
  
  private:
    uint8_t _debug;
    bool _running;
  
  public:
    void init(void);
    void start(void);
    void slaveRX(uint8_t addr);
    void slaveTX(uint8_t addr);
    void write(uint8_t data);
    uint8_t readACK(void);
    uint8_t readNACK(void);
    void stop (void);  
};


#endif