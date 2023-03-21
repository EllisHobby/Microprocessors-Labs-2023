/*

*/



#include "analog_joystick.h"

#define F_CLK   16e6
#define DELAY_SEC(n) __builtin_avr_delay_cycles( (F_CLK*n))

#define HANDSHAKE     0
#define AXIS_CONTROL  1
#define EXIT          99


JoyStick joystick;
Axis axis;

void setup() {
  Serial.begin(9600);
  DELAY_SEC(1);
}

void loop() {
  if (Serial.available() > 0) {
    
    int req = Serial.read();

    switch(req) {
      
      case HANDSHAKE:
        if (Serial.availableForWrite()) 
          Serial.println("Arduino Connected");
        break;

      case AXIS_CONTROL:
        publish_axis();  
        break;

      case EXIT:
        if (Serial.availableForWrite())
          Serial.println("exit");
        break;
      
      default:
        break;
    }
  }
}


void publish_axis (void) {

  axis = joystick.read();
  
  if (Serial.availableForWrite())
    Serial.println((String) "{\"x\": " + axis.x + ", \"y\": " + axis.y + ", \"z\": " + axis.z + "}");  
}

