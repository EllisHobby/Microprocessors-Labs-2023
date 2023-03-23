#ifndef MPU6050_H
#define MPU6050_H

#include "Arduino.h"
#include "util/delay.h"
#include "util/twi.h"
#include "axis.h"


// used register addresses
#define SMPRT_DIV         0x19
#define CONFIG            0x1A
#define GYRO_CONFIG       0x1B
#define ACCEL_CONFIG      0x1C
#define GYRO_XOUT_H       0x43
#define GYRO_XOUT_L       0x44
#define GYRO_YOUT_H       0x45
#define GYRO_YOUT_L       0x46
#define GYRO_ZOUT_H       0x47
#define GYRO_ZOUT_L       0x48
#define ACCEL_XOUT_H      0x3B
#define ACCEL_XOUT_L      0x3C
#define ACCEL_YOUT_H      0x3D
#define ACCEL_YOUT_L      0x3E
#define ACCEL_ZOUT_H      0x3F
#define ACCEL_ZOUT_L      0x40
#define PWR_MGMT_1        0x6B
#define WHO_AM_I          0x75

// sensitivity config values
#define GYRO_RANGE_250      0
#define GYRO_RANGE_500      1
#define GYRO_RANGE_1000     2
#define GYRO_RANGE_2000     3
#define ACCEL_RANGE_2       0
#define ACCEL_RANGE_4       1
#define ACCEL_RANGE_8       2
#define ACCEL_RANGE_16      3
#define RANGE_MASK          0xE7

// conversion factors
#define GYRO_SENSITIVITY_250    131.0f
#define GYRO_SENSITIVITY_500    65.5f
#define GYRO_SENSITIVITY_1000   32.8f
#define GYRO_SENSITIVITY_2000   16.4f
#define ACCEL_SENSITIVITY_2     16384.0f       
#define ACCEL_SENSITIVITY_4     8192.0f
#define ACCEL_SENSITIVITY_8     4096.0f
#define ACCEL_SENSITIVITY_16    2048.0f

// different address options, change AD0 pin
#define MPU6050_ADDR_LOW    0x68
#define MPU6050_ADDR_HIGH   0x69

#define DEBUG_OFF         0
#define DEBUG_DATA        1
#define DEBUG_FUNC        2
#define DEBUG_I2C         3
#define DEBUG_READ        4
#define DEBUG_RAW_GYRO    5
#define DEBUG_GYRO        6
#define DEBUG_RAW_ACCEL   7
#define DEBUG_ACCEL       8
#define DEBUG_CONTROL     9

#define TWBR_400kHz   12



typedef struct {
  
  int16_t accel_x = 0;
  int16_t accel_y = 0;
  int16_t accel_z = 0;
  
  int16_t temp = 0;
  
  int16_t gyro_x = 0;
  int16_t gyro_y = 0;
  int16_t gyro_z = 0;
  
} RawData;



class IMU {

  private:
  
    uint8_t debug_;
    
    // i2c help
    uint8_t addr_;
    bool running_;
    
    void i2cInit_ (void);
    void i2cStart_ (void);
    void i2cSlaveWrite_ (void);
    void i2cSlaveRead_ (void);
    void i2cWrite_ (uint8_t data);
    uint8_t i2cReadACK_ (void);
    uint8_t i2cReadNACK_ (void);
    void i2cStop_ (void);
    
    // data storage
    RawData raw_;
    Axis gyro_buf_;
    Axis accel_buf_;
    Axis gyro_offs_;
    Axis accel_offs_;
    Axis control_buf_;
    
    // conversion help
    uint8_t accel_range_;
    uint8_t gyro_range_;
    float accel_factor_;
    float gyro_factor_;
    float getGyroConvFactor_ (uint8_t range);
    float getAccelConvFactor_ (uint8_t range);
    int16_t littleEndian_ (int16_t data);
    
    // imu config
    void setPwrMgmt1_ (void);
    void setAccelOffset_ (int16_t ax, int16_t ay, int16_t az);
    void setGyroOffset_ (int16_t gx, int16_t gy, int16_t gz);

  public:
  
    IMU ();
    
    void init (uint8_t addr = MPU6050_ADDR_LOW);
    void testConnection (void);
    void setDebugLevel (uint8_t debug) { debug_ = debug; }
    void setRange (uint8_t gyro, uint8_t accel);
    void calibrate (long iter=1000);

    Axis read (void);

    RawData getRaw (void) {return raw_; }

    Axis getGyro (void) { return gyro_buf_; }
    float getGyroX (void) { return gyro_buf_.x; }
    float getGyroY (void) { return gyro_buf_.y; }
    float getGyroZ (void) { return gyro_buf_.z; }
    int16_t getRawGyroX (void) { return raw_.gyro_x; }
    int16_t getRawGyroY (void) { return raw_.gyro_y; }
    int16_t getRawGyroZ (void) { return raw_.gyro_z; }

    Axis getAccel (void) { return accel_buf_; }
    float getAccelX (void) { return accel_buf_.x; }
    float getAccelY (void) { return accel_buf_.y; }
    float getAccelZ (void) { return accel_buf_.z; }
    int16_t getRawAccelX (void) { return raw_.accel_x; }
    int16_t getRawAccelY (void) { return raw_.accel_y; }
    int16_t getRawAccelZ (void) { return raw_.accel_z; }

    Axis getControl (void) { return control_buf_; }
};




#endif