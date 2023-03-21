#include "twi_i2c.h"




I2C::I2C (uint8_t rate = TWBR_100kHz) {
  TWBR = 0x48;
  TWSR = 0x00;
}




void I2C::setBitRate (uint8_t rate) {
  TWBR = GET_BIT_RATE_DIVIDER(rate);
  TWSR = 0x00;
}




void I2C::start (void) {

  TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN) | (1 << TWEA);
  while (!(TWCR & (1 << TWINT)));
  if ((TWSR & STATUS_MASK) != TW_START) {
    TWCR &= ~(1 << TWEN);
    stop();
    Serial.println("Error Start Condition");
    return;
  }
  else {
    running_ = true;
  }
}


void I2C::sla_w (uint8_t addr) {
  if (running_) {
    TWDR = (addr << 1) | TW_WRITE;
    TWCR = (1<<TWINT)|(1<<TWEN); // Start transmission
    while (!(TWCR & (1 << TWINT)));
    if ((TWSR & STATUS_MASK) != TW_MT_SLA_ACK) {
      TWCR &= ~(1 << TWEN);
      stop();
      Serial.println("Error SLA+W");
      return;
    }
  }
}


void I2C::sla_r (uint8_t addr) {
  if (running_) {
    TWDR = (addr << 1) | TW_READ; 
    TWCR = (1<<TWINT)|(1<<TWEN); // Start transmission
    while (!(TWCR & (1 << TWINT)));
    if ((TWSR & STATUS_MASK) != TW_MR_SLA_ACK) {
      TWCR &= ~(1 << TWEN);
      stop();
      Serial.println("Error SLA+R");
      return;
    }
  }
}




void I2C::write_ (uint8_t data) {
  if (running_) {
    TWDR = data; 
    TWCR = (1<<TWINT)|(1<<TWEN); // Start transmission
    while (!(TWCR & (1 << TWINT)));
    if ((TWSR & STATUS_MASK) != TW_MT_DATA_ACK) {
      TWCR &= ~(1 << TWEN); 
      stop();
      Serial.println("Error Write Slave");
      return;
    }
  }
}


void I2C::write (const uint8_t* data, size_t size) {
  if (running_) {
    for (size_t i = 0; i < size; i++) {
      write_(data[i]);  
    }
  }
}

void I2C::write (uint8_t data) {
  if (running_) {
    TWDR = data; 
    TWCR = (1<<TWINT)|(1<<TWEN); // Start transmission
    while (!(TWCR & (1 << TWINT)));
    if ((TWSR & STATUS_MASK) != TW_MT_DATA_ACK) {
      TWCR &= ~(1 << TWEN);  
      stop();
      Serial.println("Error Write Slave");
      return;
    }
  }
}




uint8_t I2C::readACK (void) {
  
  if (running_) {
    TWCR = (1 << TWEN) | (1 << TWINT) | (1 << TWEA);  // send ack
    while (! (TWCR & (1 << TWINT)));                    // wait for completion
    if ((TWSR & STATUS_MASK) != TW_MR_DATA_ACK) {
      TWCR &= ~(1 << TWEN);
      Serial.println(TWSR, BIN);
      Serial.println("Error Read ACK Slave");
      stop();
      return I2C_ERROR;
    }
    else {
      return TWDR; 
    }    
  }    
}




uint8_t I2C::readNACK (void) {

  if (running_) {
    TWCR = (1 << TWEN) | (1 << TWINT);  // send nack
    while (! (TWCR & (1 << TWINT))); 
    if ((TWSR & 0xF8) != TW_MR_DATA_NACK) {
      TWCR &= ~(1 << TWEN);
      Serial.println(TWSR, BIN);
      Serial.println("Error Read NACK Slave");
      stop();
      return I2C_ERROR;
    }
    else {
      return TWDR; 
    }
  }
}




void I2C::stop (void) {
  TWCR = (1<<TWINT)|(1<<TWSTO); 
  running_ = false;
  _delay_ms(1); // Allow time for stop to send
}

