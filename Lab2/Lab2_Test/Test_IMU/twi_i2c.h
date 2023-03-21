#ifndef TWI_I2C_H
#define TWI_I2C_H

#include "Arduino.h"
#include "util/twi.h"

#define TWBR_100kHz     72
#define TWPS_100kHz     0

#define I2C_WRITE       0x00
#define I2C_READ        0x01

#define STATUS_MASK     0xF8

#define GET_BIT_RATE_DIVIDER(r)   (uint8_t)(((F_CPU)/(r*2)) - (8))

#define I2C_ERROR     -1
#define I2C_SUCCESS   0



class I2C {
  
  private:
    void write_ (uint8_t data);
    bool running_ = false;

  public:
    I2C (uint8_t rate = TWBR_100kHz);
    void setBitRate (uint8_t rate);
    void start (void);
    void sla_w (uint8_t addr);
    void sla_r (uint8_t addr);
    void write (const uint8_t* data, size_t size);
    void write (uint8_t data);
    uint8_t readACK (void);
    uint8_t readNACK (void);
    void stop (void);
};


#endif
