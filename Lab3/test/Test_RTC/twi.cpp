/*
  Author: Ellis Hobby
  Info:
    > TWI class definitions
*/

#include "util/delay.h"
#include "twi.h"


void TWI::init(void) {
  TWBR = TWBR_400KHZ;
  TWSR = 0x00;
  _running = false;
}


void TWI::start(void) {
  
  // send START condition, wait for complete
  TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN) | (1 << TWEA);
  while (!(TWCR & (1 << TWINT)));
  
  // check status reg
  if ((TWSR & TW_STATUS_MASK) != TW_START) {
    TWCR &= ~(1 << TWEN);
    _running = false;
  }
  else {
    _running = true;
  }
}


void TWI::slaveRX (uint8_t addr) {
  
  if (_running) {
    
    // load address + WRITE, start transmission
    TWDR = (addr << 1) | TW_WRITE;
    TWCR = (1<<TWINT)|(1<<TWEN);
    while (!(TWCR & (1 << TWINT)));
    
    // check status reg for ACK
    if ((TWSR & TW_STATUS_MASK) != TW_MT_SLA_ACK) {
      TWCR &= ~(1 << TWEN);
      _running = false;
    }
  }
}


void TWI::slaveTX (uint8_t addr) {
  
  if (_running) {
    
    // load address + READ, start transmission
    TWDR = (addr << 1) | TW_READ; 
    TWCR = (1<<TWINT)|(1<<TWEN); 
    while (!(TWCR & (1 << TWINT)));
    
    // check status reg for ACK
    if ((TWSR & TW_STATUS_MASK) != TW_MR_SLA_ACK) {
      TWCR &= ~(1 << TWEN);
      _running = false;
    }
  }
}


void TWI::write (uint8_t data) {
  
  if (_running) {
    
    // load data, start transmission
    TWDR = data; 
    TWCR = (1<<TWINT)|(1<<TWEN); 
    while (!(TWCR & (1 << TWINT)));
    
    // check status reg for ACK
    if ((TWSR & TW_STATUS_MASK) != TW_MT_DATA_ACK) {
      TWCR &= ~(1 << TWEN);  
      _running = false;
    }
  }
}


uint8_t TWI::readACK (void) {
  
  if (_running) {
    
    // enable send ACK to slave
    TWCR = (1 << TWEN) | (1 << TWINT) | (1 << TWEA);
    while (! (TWCR & (1 << TWINT)));   
    
    // check if ACK sent
    if ((TWSR & TW_STATUS_MASK) != TW_MR_DATA_ACK) {
      TWCR &= ~(1 << TWEN);
      _running = false;
      return -1;
    }
    
    // return data on success
    else {
      return TWDR; 
    }    
  }    
}


uint8_t TWI::readNACK (void) {

  if (_running) {
    
    // enable send NACK to slave
    TWCR = (1 << TWEN) | (1 << TWINT);
    while (! (TWCR & (1 << TWINT))); 
    
    // check if NACK sent
    if ((TWSR & TW_STATUS_MASK) != TW_MR_DATA_NACK) {
      TWCR &= ~(1 << TWEN);
      _running = false;
      return -1;
    }
    
    // return data on success
    else {
      return TWDR; 
    }
  }
}


void TWI::stop (void) {
  TWCR = (1<<TWINT)|(1<<TWSTO); 
  _running = false;
  _delay_ms(1);
}