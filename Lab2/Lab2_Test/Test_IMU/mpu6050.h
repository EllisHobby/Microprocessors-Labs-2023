#ifndef MPU6050_H
#define MPU6050_H


#include "twi_i2c.h"
#include "axis.h"

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
#define PWR_MGMT_2        0x6C

#define MPU6050_ADDR_HIGH   0b1101001
#define MPU6050_ADDR_LOW    0b1101000

#define GYRO_RANGE_250      0
#define GYRO_RANGE_500      1
#define GYRO_RANGE_1000     2
#define GYRO_RANGE_2000     3

#define ACCEL_RANGE_2       0
#define ACCEL_RANGE_4       1
#define ACCEL_RANGE_8       2
#define ACCEL_RANGE_16      3

#define RANGE_MASK          0xE7

#define READ_GYRO_AXIS      0
#define READ_ACCEL_AXIS     1



class IMU {

  private:
    bool debug_;
    uint8_t addr_;
    Axis gyro_;
    Axis accel_;
    Axis buf_;
    I2C* ptr_i2c_;
    uint8_t status_;
    
    void setPwrMgmt1_ (void);
    int16_t readAxis_ (uint8_t reg_high, uint8_t reg_low);

  public:
  
    IMU (I2C i2c);

    void init (uint8_t addr = MPU6050_ADDR_LOW);
    void setRange (uint8_t gyro, uint8_t accel);
    
    Axis readGyro (void);
    Axis readAccel (void);
    Axis read (void);

    int16_t getGryo_x (void) {return gyro_.x; }
    int16_t getGryo_y (void) {return gyro_.y; }
    int16_t getGryo_z (void) {return gyro_.z; }

    int16_t getAccel_x (void) {return accel_.x; }
    int16_t getAccel_y (void) {return accel_.y; }
    int16_t getAccel_z (void) {return accel_.z; }
};




#endif
