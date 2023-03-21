#ifndef MPU6050_H
#define MPU6050_H

#include "mpu6050_reg.h"
#include "twi_i2c.h"


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


typedef struct {  
  signed int16_t x, y, z;
} MPU6050_Buffer;

class MPU6050 {

  private:
    uint8_t _addr
    uint8_t _gyro_range
    uint8_t _accel_range
    MPU6050_Buffer _gyro;
    MPU6050_Buffer _accel;
    I2C* _i2c;
    uint8_t _status;
    void _init (void);
    void _error_help(char* err_msg);
    int16_t MPU::_read_axis(uint8_t REG_H, uint8_t REG_L);

  public:
    MPU6050 (I2C i2c, uint8_t address = MPU6050_ADDR_LOW);
    void set_gyro_range(uint8_t range);
    void set_accel_range(uint8_t range);
    
    void read_gyro (void);
    void read_accel (void);

    int16_t get_gyro_x (void) {return _gyro.x; }
    int16_t get_gyro_y (void) {return _gyro.y; }
    int16_t get_gyro_z (void) {return _gyro.z; }

    int16_t get_accel_x (void) {return _accel.x; }
    int16_t get_accel_y (void) {return _accel.y; }
    int16_t get_accel_z (void) {return _accel.z; }
};




#endif
