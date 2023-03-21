#include "mpu6050.h"




IMU::IMU (I2C i2c) {
  ptr_i2c_ = &i2c;
}


void IMU::setPwrMgmt1_ (void) {
  ptr_i2c_->start();            // START 
  ptr_i2c_->sla_w(addr_);       // slave write  
  ptr_i2c_->write(PWR_MGMT_1);  // access power management 1
  ptr_i2c_->write(0x01);        // write power management 1
  ptr_i2c_->stop();             // STOP                         
}




void IMU::init (uint8_t addr = MPU6050_ADDR_LOW) {
  addr_ = addr;
  setPwrMgmt1_();
  setRange(GYRO_RANGE_2000, ACCEL_RANGE_2);
}



void IMU::setRange (uint8_t gyro, uint8_t accel) {
  ptr_i2c_->start();            // START 
  ptr_i2c_->sla_w(addr_);       // slave write  
  ptr_i2c_->write(SMPRT_DIV);   // access sample rate divider
  ptr_i2c_->write(0x04);        // write sample rate divider
  ptr_i2c_->write(0x03);        // write config, auto increments
  ptr_i2c_->write(gyro);        // write gyro config, auto increments
  ptr_i2c_->write(accel);       // write accel config, auto increments
  ptr_i2c_->stop();             // STOP                  
}



int16_t IMU::readAxis_(uint8_t reg_high, uint8_t reg_low) {
  
  int16_t result = 0;

  ptr_i2c_->start();            // START 
  ptr_i2c_->sla_w(addr_);       // slave write  
  ptr_i2c_->write(reg_high);    // access high register
  ptr_i2c_->stop();             // STOP 

  ptr_i2c_->start();                  // START 
  ptr_i2c_->sla_r(addr_);             // slave read 
  result = ptr_i2c_->readACK() << 8;  // high byte and save
  result |= ptr_i2c_->readNACK();     // low byte and save
  ptr_i2c_->stop();                   // STOP

  return result;
}




Axis IMU::readGyro (void) {
  gyro_.x = readAxis_(GYRO_XOUT_H, GYRO_XOUT_L);
  gyro_.y = readAxis_(GYRO_YOUT_H, GYRO_YOUT_L);
  gyro_.z = readAxis_(GYRO_YOUT_H, GYRO_YOUT_L);
  return gyro_;
}




Axis IMU::readAccel (void) {
  accel_.x = readAxis_(ACCEL_XOUT_H, ACCEL_XOUT_L);
  accel_.y = readAxis_(ACCEL_YOUT_H, ACCEL_YOUT_L);
  accel_.z = readAxis_(ACCEL_ZOUT_H, ACCEL_ZOUT_L);
  return accel_;
}


Axis IMU::read (void) {
  buf_.x = readAxis_(GYRO_XOUT_H, GYRO_XOUT_L);     // read gyro x axis
  buf_.y = readAxis_(GYRO_YOUT_H, GYRO_YOUT_L);     // read gyro y axis
  buf_.z = readAxis_(ACCEL_ZOUT_H, ACCEL_ZOUT_L);   // read accel for z axis
  return buf_;
}




