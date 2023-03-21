#include "mpu6050.h"




MPU6050::MPU6050 (I2C i2c, uint8_t address = MPU6050_ADDR_LOW) {
  
  _i2c = &i2c;
  _addr = address;
  _gyro_range = GYRO_RANGE_2000;
  _accel_range = ACCEL_RANGE_16;
  _init();
}




void MPU6050::_error_help (char* err_msg) {
  Serial.print(err_msg);
  Serial.println(_status);
}




void MPU6050::_init (void) {
  
  _status = _i2c->start(_addr);                // send start condition
  
  if (_status != 0) {
    _error_help("Error START (_init):  ");  
    return;
  }
  
  _status = _i2c->write(MPU6050_GYRO_CONFIG);  // access gyro config reg
  
  if (_status != 0) {
      _error_help("Error WRITE (access gyro config reg):  ");
      return;
  }

  _status = _i2c->write(_gyro_range << 3);     // set gyro range (default 2000)
  
  if (status != 0) {
    _error_help("Error WRITE (set gyro range):  ");
    return;
  }
    
  _status = _i2c->write(_accel_range << 3);    // set accel range (default 16), ** reg addr auto increments **
  
  if (_status != 0) {
    _error_help("Error WRITE (set accel range):  ");
    return;
  }

  _i2c->stop();                                // send stop condition

}




void MPU6050::set_accel_range (uint8_t range) {

  if ((range < 3) && (range > 0)) {
    _accel_range = range;

    _status = _i2c->start(_addr);                    // send start condition
  
    if (_status != 0) {
      _error_help("Error START (set_accel_range):  ");  
      return;
    }

    _status = _i2c->write(MPU6050_ACCEL_CONFIG);    // access accel config reg
  
    if (_status != 0) {
        _error_help("Error WRITE (access ACCEL_CONFIG):  ");
        return;
    }

    _status = _i2c->write(_accel_range << 3);        // set accel range
  
    if (_status != 0) {
      _error_help("Error WRITE (set accel range):  ");
      return;
    }

    _i2c->stop();                                   // send stop condition
  }
}




void MPU6050::set_gyro_range (uint8_t range) {

  if ((range < 3) && (range > 0)) {
    _gyro_range = range;

    _status = _i2c->start(_addr);                      // send start condition
  
    if (_status != 0) {
      _error_help("Error START (set_gyro_range):  ");  
      return;
    }

    _status = _i2c->write(MPU6050_GYRO_CONFIG);       // access gyro config reg
  
    if (_status != 0) {
        _error_help("Error WRITE (access GYRO_CONFIG):  ");
        return;
    }

    _status = _i2c->write(_gyro_range << 3);           // set gyro range
  
    if (_status != 0) {
      _error_help("Error WRITE (gryo range):  ");
      return;
    }

    _i2c->stop();                                     // send stop condition
  }
}




int16_t MPU::_read_axis(uint8_t REG_H, uint8_t REG_L) {
  
  int16_t result = 0;

  // READING AXIS HIGH REG
  _status = _i2c->start(_addr);                 // send start condition

  if (_status != 0) {
    _error_help("Error START (AXIS HIGH REG):  ");  
    return result;
  }

  _status = _i2c->write(REG_H);                 // access AXIS HIGH REG

  if (_status != 0) {
      _error_help("Error WRITE (access AXIS HIGH REG):  ");
      return result;
  }

  _status = _i2c->repeat_start(_addr);          // send repeat start to read

  if (_status != 0) {
    _error_help("Error REPEAT START (AXIS HIGH REG):  ");  
    return result;
  }

  result = (_i2c->read() << 8);                 // save result and shift into buf


  // READING AXIS LOW REG
  _status = _i2c->start(_addr);                 // send start condition

  if (_status != 0) {
    _error_help("Error START (AXIS LOW REG):  ");  
    return result;
  }

  _status = _i2c->write(REG_L);                 // access AXIS LOW REG

  if (_status != 0) {
      _error_help("Error WRITE (access AXIS LOW REG):  ");
      return result;
  }

  _status = _i2c->repeat_start(_addr);          // send repeat start to read

  if (_status != 0) {
    _error_help("Error REPEAT START (AXIS LOW REG):  ");  
    return result;
  }

  result = (_i2c->read(false) | result);        // save result with prev, send nack

  _i2c->stop();                                 // send stop condition

  return result; 
}




void MPU6050::read_gyro (void) {

  _gyro.x = _read_axis(MPU6050_GYRO_XOUT_H, MPU6050_GYRO_XOUT_L);
  _gyro.y = _read_axis(MPU6050_GYRO_YOUT_H, MPU6050_GYRO_YOUT_L);
  _gyro.z = _read_axis(MPU6050_GYRO_YOUT_H, MPU6050_GYRO_YOUT_L);
  
}




void MPU6050::read_accel (void) {

  _accel.x = _read_axis(MPU6050_ACCEL_XOUT_H, MPU6050_ACCEL_XOUT_L);
  _accel.y = _read_axis(MPU6050_ACCEL_YOUT_H, MPU6050_ACCEL_YOUT_L);
  _accel.z = _read_axis(MPU6050_ACCEL_ZOUT_H, MPU6050_ACCEL_ZOUT_L);
}




