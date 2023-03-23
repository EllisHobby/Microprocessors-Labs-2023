#include "mpu6050.h"




IMU::IMU () {
  debug_ = DEBUG_OFF;
  running_ = false;
}


/*-----------------------------------------------

        MPU Config Functions

-----------------------------------------------*/
void IMU::init (uint8_t addr = MPU6050_ADDR_LOW) {

  if (debug_ == DEBUG_FUNC) { 
    Serial.println("Start: init"); 
  }
  addr_ = addr;
  i2cInit_();
  setPwrMgmt1_();
  setRange(GYRO_RANGE_250, ACCEL_RANGE_2);
  testConnection();
  
  if (debug_ == DEBUG_FUNC) { 
    Serial.println("\tEnd: init");
  }
}


void IMU::testConnection (void) {

  if (debug_ == DEBUG_FUNC) { 
    Serial.println("Start: testConnection"); 
  }
  uint8_t reply = 0;
  
  i2cStart_();            // START 
  i2cSlaveWrite_();       // slave write  
  i2cWrite_(WHO_AM_I);    // access Who Am I
  i2cStop_();             // STOP 

  i2cStart_();            // START 
  i2cSlaveRead_();        // slave read
  reply = i2cReadNACK_(); // read NACK
  i2cStop_();             // STOP  
  
  if ((reply != MPU6050_ADDR_LOW) && (debug_ == DEBUG_FUNC)) { 
    Serial.println("ERROR: TEST REPLY INVALID"); 
  }
  if ((debug_ == DEBUG_FUNC) ||  (debug_ == DEBUG_DATA)) { 
    Serial.print("\tReply = "); Serial.println(reply, HEX);
  }
  if (debug_ == DEBUG_FUNC) { 
    Serial.println("\tEnd: testConnection");
  }
}


void IMU::setPwrMgmt1_ (void) {
  
  if (debug_ == DEBUG_FUNC) { 
    Serial.println("Start: setPwrMgmt1_"); 
  }
  i2cStart_();            // START 
  i2cSlaveWrite_();       // slave write  
  i2cWrite_(PWR_MGMT_1);  // access power management 1
  i2cWrite_(0x80);        // reset 
  i2cStop_();             // STOP 

  i2cStart_();            // START 
  i2cSlaveWrite_();       // slave write  
  i2cWrite_(PWR_MGMT_1);  // access power management 1
  i2cWrite_(0x01);        // clock =  PLL gyro x ref 
  i2cStop_();             // STOP  
  
  if (debug_ == DEBUG_FUNC) { 
    Serial.println("\tEnd: setPwrMgmt1_"); 
  }
}


void IMU::setRange (uint8_t gyro, uint8_t accel) {
  
  if (debug_ == DEBUG_FUNC) { 
    Serial.println("Start: setRange"); 
  }
  accel_range_ = accel;
  gyro_range_ = gyro;
  
  i2cStart_();            // START 
  i2cSlaveWrite_();       // slave write  
  i2cWrite_(SMPRT_DIV);   // access sample rate divider
  i2cWrite_(0x04);         // write sample rate divider
  i2cWrite_(0x03);        // write config, auto increments
  i2cWrite_(gyro);        // write gyro config, auto increments
  i2cWrite_(accel);       // write accel config, auto increments
  i2cStop_();             // STOP
  
  gyro_factor_  = getGyroConvFactor_(gyro);
  accel_factor_ = getAccelConvFactor_(accel);
  
  if (debug_ == DEBUG_FUNC) { 
    Serial.println("\tEnd: setRange"); 
  }
}


float IMU::getGyroConvFactor_ (uint8_t range) {
  
  switch(range) {
    
    case GYRO_RANGE_250:
      return GYRO_SENSITIVITY_250;
    case GYRO_RANGE_500:
      return GYRO_SENSITIVITY_500; 
    case GYRO_RANGE_1000:
      return GYRO_SENSITIVITY_2000; 
    case GYRO_RANGE_2000:
      return GYRO_SENSITIVITY_2000;
    default:
      return 1.0f;
  }
}


float IMU::getAccelConvFactor_ (uint8_t range) {
  
  switch(range) {
    case ACCEL_RANGE_2:
      return ACCEL_SENSITIVITY_2; 
    case ACCEL_RANGE_4:
      return ACCEL_SENSITIVITY_4; 
    case ACCEL_RANGE_8:
      return ACCEL_SENSITIVITY_8; 
    case ACCEL_RANGE_16:
      return ACCEL_SENSITIVITY_16; 
    default:
      return 1.0f;
  }
}


void IMU::calibrate (long iter = 1000) {

  if (debug_ == DEBUG_FUNC ) { 
    Serial.println("Start: calibrate"); 
  }
  
  float ax = 0, ay = 0, az = 0, gx = 0, gy = 0, gz = 0;
  
  for (long i = 0; i < iter; i++) {
    
    read();
  
    ax += ((float)raw_.accel_x) / accel_factor_;
    ay += ((float)raw_.accel_y) / accel_factor_;
    az += ((float)raw_.accel_z) / accel_factor_;
    gx += ((float)raw_.gyro_x) / gyro_factor_;
    gy += ((float)raw_.gyro_y) / gyro_factor_;
    gz += ((float)raw_.gyro_z) / gyro_factor_;

    _delay_ms(1);
  }

  accel_offs_.x = -ax / (float)iter;
  accel_offs_.y = -ay / (float)iter;
  accel_offs_.z = -az / (float)iter;
  gyro_offs_.x = -gx / (float)iter;
  gyro_offs_.y = -gy / (float)iter;
  gyro_offs_.z = -gz / (float)iter;  

  
  if ((debug_ == DEBUG_DATA) || (debug_ == DEBUG_FUNC)) {
    Serial.println("\tCalibration Values");
    Serial.println((String) "\t[Accel] x: " + accel_offs_.x + ", y: " + accel_offs_.y + ", z: " + accel_offs_.z);
    Serial.println((String) "\t[Gyro] x: " + gyro_offs_.x + ", y: " + gyro_offs_.y + ", z: " + gyro_offs_.z);
    Serial.println();
  }
  
  if (debug_  == DEBUG_FUNC) { 
    Serial.println("\tEnd: calibrate"); 
  }
}
/*-----------------------------------------------
-----------------------------------------------*/




/*-----------------------------------------------

        I2C Help Functions

-----------------------------------------------*/

void IMU::i2cInit_ (void) {
  TWBR = TWBR_400kHz;
  TWSR = 0x00;
}


void IMU::i2cStart_ (void) {
  
  // send START condition, wait for complete
  TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN) | (1 << TWEA);
  while (!(TWCR & (1 << TWINT)));
  
  // check status reg
  if ((TWSR & TW_STATUS_MASK) != TW_START) {
    TWCR &= ~(1 << TWEN);
    running_ = false;
    if (debug_ == DEBUG_I2C) { 
      Serial.println("ERROR: I2C START"); 
    }
  }
  else {
    running_ = true;
  }
}


void IMU::i2cSlaveWrite_ (void) {
  
  if (running_) {
    
    // load address + WRITE, start transmission
    TWDR = (addr_ << 1) | TW_WRITE;
    TWCR = (1<<TWINT)|(1<<TWEN);
    while (!(TWCR & (1 << TWINT)));
    
    // check status reg for ACK
    if ((TWSR & TW_STATUS_MASK) != TW_MT_SLA_ACK) {
      TWCR &= ~(1 << TWEN);
      running_ = false;
      if (debug_ == DEBUG_I2C) { 
        Serial.println("ERROR: I2C SLA+W"); 
      }
    }
  }
}


void IMU::i2cSlaveRead_ (void) {
  
  if (running_) {
    
    // load address + READ, start transmission
    TWDR = (addr_ << 1) | TW_READ; 
    TWCR = (1<<TWINT)|(1<<TWEN); 
    while (!(TWCR & (1 << TWINT)));
    
    // check status reg for ACK
    if ((TWSR & TW_STATUS_MASK) != TW_MR_SLA_ACK) {
      TWCR &= ~(1 << TWEN);
      running_ = false;
      if (debug_ == DEBUG_I2C) { 
        Serial.println("ERROR: I2C SLA+R"); 
      }
    }
  }
}


void IMU::i2cWrite_ (uint8_t data) {
  
  if (running_) {
    
    // load data, start transmission
    TWDR = data; 
    TWCR = (1<<TWINT)|(1<<TWEN); 
    while (!(TWCR & (1 << TWINT)));
    
    // check status reg for ACK
    if ((TWSR & TW_STATUS_MASK) != TW_MT_DATA_ACK) {
      TWCR &= ~(1 << TWEN);  
      running_ = false;
      if (debug_ == DEBUG_I2C) { 
        Serial.println("ERROR: I2C WRITE BYTE"); 
      }
    }
  }
}


uint8_t IMU::i2cReadACK_ (void) {
  
  if (running_) {
    
    // enable send ACK to slave
    TWCR = (1 << TWEN) | (1 << TWINT) | (1 << TWEA);
    while (! (TWCR & (1 << TWINT)));   
    
    // check if ACK sent
    if ((TWSR & TW_STATUS_MASK) != TW_MR_DATA_ACK) {
      TWCR &= ~(1 << TWEN);
      running_ = false;
      if (debug_ == DEBUG_I2C) { 
        Serial.println("ERROR: I2C READ BYTE ACK"); 
      }
      return -1;
    }
    
    // return data on success
    else {
      return TWDR; 
    }    
  }    
}


uint8_t IMU::i2cReadNACK_ (void) {

  if (running_) {
    
    // enable send NACK to slave
    TWCR = (1 << TWEN) | (1 << TWINT);
    while (! (TWCR & (1 << TWINT))); 
    
    // check if NACK sent
    if ((TWSR & TW_STATUS_MASK) != TW_MR_DATA_NACK) {
      TWCR &= ~(1 << TWEN);
      running_ = false;
      if (debug_ == DEBUG_I2C) { 
        Serial.println("ERROR: I2C READ BYTE NACK"); 
      }
      return -1;
    }
    
    // return data on success
    else {
      return TWDR; 
    }
  }
}


void IMU::i2cStop_ (void) {
  TWCR = (1<<TWINT)|(1<<TWSTO); 
  running_ = false;
  _delay_ms(1);
}
/*-----------------------------------------------
-----------------------------------------------*/




/*-----------------------------------------------

        MPU6050 Measurement Functions

-----------------------------------------------*/

int16_t IMU::littleEndian_ (int16_t data) {
  return ((data << 8) | ((data >> 8) & 0xFF));
}


Axis IMU::read (void) {
  
  if (debug_ == DEBUG_READ) { 
    Serial.println("Start: IMU Read"); 
  } 
  i2cStart_();              // START 
  i2cSlaveWrite_();         // slave write  
  i2cWrite_(ACCEL_XOUT_H);  // access ACCEL_XOUT_H
  i2cStop_();               // STOP 

  i2cStart_();              // START 
  i2cSlaveRead_();          // slave read 

  // read raw accel x, y, z
  raw_.accel_x = (i2cReadACK_() << 8) | (i2cReadACK_());  
  raw_.accel_y = (i2cReadACK_() << 8) | (i2cReadACK_());  
  raw_.accel_z = (i2cReadACK_() << 8) | (i2cReadACK_());  
  
  // read raw temperature
  raw_.temp = (i2cReadACK_() << 8) | (i2cReadACK_());  
  
  // read raw gyro x, y, z
  raw_.gyro_x = (i2cReadACK_() << 8) | (i2cReadACK_());  
  raw_.gyro_y = (i2cReadACK_() << 8) | (i2cReadACK_());  
  raw_.gyro_z = (i2cReadACK_() << 8) | (i2cReadNACK_());

  i2cStop_();               // STOP 
  
  // convert accel raw to +/-g
  accel_buf_.x = ((float)raw_.accel_x / accel_factor_);
  accel_buf_.y = ((float)raw_.accel_y / accel_factor_) + accel_offs_.y;
  accel_buf_.z = ((float)raw_.accel_z / accel_factor_) + accel_offs_.z;
  
  // convert gyro raw to dps
  gyro_buf_.x = (((float)raw_.gyro_x / gyro_factor_)+ gyro_offs_.x);
  gyro_buf_.y = ((float)raw_.gyro_y / gyro_factor_) + gyro_offs_.y;
  gyro_buf_.z = ((float)raw_.gyro_z / gyro_factor_) + gyro_offs_.z;
  
  control_buf_.x = gyro_buf_.x;
  control_buf_.y = gyro_buf_.y;
  control_buf_.z = accel_buf_.x;
  
  if (debug_ == DEBUG_RAW_GYRO) {
    Serial.println((String) "[Raw Gyro] x: " + raw_.gyro_x + ", y: " + raw_.gyro_y + ", z: " + raw_.gyro_z);
  }
  if (debug_ == DEBUG_GYRO) {
    Serial.println((String) "[Gyro] x: " + gyro_buf_.x + ", y: " + gyro_buf_.y + ", z: " + gyro_buf_.z);
  }
  if (debug_ == DEBUG_RAW_ACCEL) {
    Serial.println((String) "[Accel] x: " + raw_.accel_x + ", y: " + raw_.accel_y + ", z: " + raw_.accel_z);
  }
  if (debug_ == DEBUG_ACCEL) {
    Serial.println((String) "[Accel] x: " + accel_buf_.x + ", y: " + accel_buf_.y + ", z: " + accel_buf_.z);
  }
  if (debug_ == DEBUG_CONTROL) {
    Serial.println((String) "[Control] x: " + control_buf_.x + ", y: " + control_buf_.y + ", z: " + control_buf_.z);
  }
  if (debug_ == DEBUG_READ) { 
    Serial.println("\tEnd: IMU Read"); 
  }
  
  return control_buf_;                  
}
