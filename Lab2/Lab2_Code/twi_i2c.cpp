#include "twi_i2c.cpp"




I2C::I2C (uint8_t rate = TWBR_400kHz) {
  _bit_rate = rate;
  _bit_rate_divider = GET_BIT_RATE_DIVIDER(_bit_rate)
  TWBR = _bit_rate_divider;
  TWSR = 0x00;
}




uint8_t I2C::get_bit_rate (void) {
  return _bit_rate;
}




uint8_t I2C::get_bit_rate_divider (void) {
  return _bit_rate_divider;
}




void set_bit_rate (uint8_t rate) {

  _bit_rate = rate;
  _bit_rate_divider = GET_BIT_RATE_DIVIDER(_bit_rate)
  TWBR = _bit_rate_divider;
  TWSR = 0x00;
}




void I2C::_internal_start (void) {
  
  // begin start condition
  TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
  
  // wait for completion
  while (! (TWCR & (1 << TWINT)));
}




uint8_t I2C::start (uint8_t addr) {

  _internal_start();
  
  // check status, handle failure
  if ((TWSR & STATUS_MASK) != MT_STATUS_START) 
    return MT_START_FAIL;   // start FAIL

  TWDR = (addr << 1) | I2C_WRITE;     // load slave address + write
  TWCR = (1 << TWINT) | (1 << TWEN);  // clear int flag and enable
  while (! (TWCR & (1 << TWINT)));    // wait for completion

  // check and return status
  if ((TWSR & STATUS_MASK) == MT_STATUS_SLA_WR_ACK) 
    return MT_SLA_WR_ACK;   // SUCCESS! write ack
  
  else if ((TWSR & STATUS_MASK) == MT_STATUS_SLA_WR_NACK) 
    return MT_SLA_WR_NACK;  // ERROR, write nack

  else
    return MT_SLA_WR_FAIL;  // read FAIL
}




uint8_t I2C::repeat_start (uint8_t addr) {

  _internal_start();

  // check status, handle failure
  if ((TWSR & STATUS_MASK) != MR_STATUS_RSTART) 
    return MR_RSTART_FAIL;   // start FAIL

  TWDR = (addr << 1) | I2C_READ;      // load slave address + read
  TWCR = (1 << TWINT) | (1 << TWEN);  // clear int flag and enable
  while (! (TWCR & (1 << TWINT)));    // wait for completion

  // check and return status
  if ((TWSR & STATUS_MASK) == MR_STATUS_SLA_RD_ACK) 
    return MR_SLA_RD_ACK;   // SUCCESS! read ack
  
  else if ((TWSR & STATUS_MASK) == MR_STATUS_SLA_RD_NACK) 
    return MR_SLA_RD_NACK;  // ERROR, read nack

  else
    return MR_SLA_RD_FAIL;  // read FAIL
}





uint8_t I2C::write (uint8_t data) {
  
  TWDR = data;
  TWCR = (1 << TWINT) | (1 << TWEN);  // clear int flag and enable
  while (! (TWCR & (1 << TWINT)));    // wait for completion

  // check and return status
  if ((TWSR & STATUS_MASK) == MT_STATUS_SLA_WR_ACK) 
    return MT_SLA_WR_ACK;   // SUCCESS! write ack
  
  else if ((TWSR & STATUS_MASK) == MT_STATUS_SLA_WR_NACK) 
    return MT_SLA_WR_NACK;  // ERROR, write nack

  else
    return MT_SLA_WR_FAIL;  // write FAIL
}




uint8_t I2C::read (bool send_ack = false) {

  if (send_ack)
    TWCR = (1 << TWEN) | (1 << TWINT) | (1 << TWEA);  // send ack
  else:
    TWCR = (1 << TWEN) | (1 << TWINT);  // send nack   
  
  while (! (TWCR & (1 << TWINT)));      // wait for completion
  
  return TWDR;  // return data

}




void I2C::stop (void) {

  // send stop condition
  TWCR = (1 << TWSTO) | (1 << TWINT) | (1 << TWEN);

  // wait for completion
  while (! (TWCR & (1 << TWINT)));
}

