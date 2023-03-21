#include "mpu6050.h"


#define HANDSHAKE     0
#define AXIS_CONTROL  1
#define RESET_SHAKE   2
#define EXIT          99

I2C i2c;
Axis axis;
int16_t shake;
int16_t shake_out = 0;
IMU imu(i2c);

void setup() {

  Serial.begin(9600);
  _delay_ms(100);

  i2c.start();
  i2c.stop();

  imu.init();
  
  axis = imu.readAccel();
  shake = axis.z;
}

void loop() {

  axis = imu.readAccel();

  shake = abs(abs(axis.z) - abs(shake));

  if (shake >= 10e3) {
    shake_out = 1;
  }
  
  if (Serial.available() > 0) {
    
    int req = Serial.read();

    switch(req) {
      
      case HANDSHAKE:
        if (Serial.availableForWrite()) 
          Serial.println("Arduino Connected");
        break;

      case AXIS_CONTROL:
        printDict((axis.x >> 8), (axis.y >> 8), shake);  
        break;

      case RESET_SHAKE:
        shake_out = 0;
        _delay_ms(100);  
        break;

      case EXIT:
        if (Serial.availableForWrite())
          Serial.println("exit");
        break;
      
      default:
        break;
    }
  }

  shake = axis.z;
}



void printDict (int16_t x, int16_t y, int16_t z) {
  Serial.println((String) "{\"x\": " + x + ", \"y\": " + y + ", \"z\": " + z + "}");  
}
