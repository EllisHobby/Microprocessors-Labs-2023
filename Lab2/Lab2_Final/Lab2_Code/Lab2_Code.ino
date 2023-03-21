
#include "util/delay.h"
#include "analog_joystick.h"

#define HANDSHAKE   0
#define CONTROL     1  


#define JOYSTICK_CONTROL  0
#define IMU_CONTROL       1


// button 
#define BUTTON_PORT   PORTE
#define BUTTON_DDR    DDRE
#define BUTTON_READ   PINE
#define BUTTON_PIN    (1 << PE4)
#define BUTTON_INT    INT4
#define BUTTON_FLAG   INTF4
#define BUTTON_INT_VECT   INT4_vect

JoyStick joystick;
Axis axis;

uint8_t control_method = 0;

ISR (BUTTON_INT_VECT) {

  // disable button int temporarily
  EIMSK &= ~(1 << BUTTON_INT);

  _delay_ms(100);
    
  if (control_method == JOYSTICK_CONTROL) {
    control_method = IMU_CONTROL;
  }
  else {
    control_method = JOYSTICK_CONTROL;
  }

  // clear any existing flags
  EIFR  &= ~(1 << BUTTON_FLAG);

  // enable button interrupt
  EIMSK |=  (1 << BUTTON_INT);
}

void setup() {

  cli();

  Serial.begin(9600);
  _delay_ms(500);

  init_button();

  sei();
}

void loop() {

  if (control_method == JOYSTICK_CONTROL) {
    axis = joystick.read();
  }
  else {
    axis.x = 0;
    axis.y = 0;
    axis.z = 0;
  }
  
  Serial.println((String) "Control: " + control_method + ", X: " + (axis.x - 512) + ", Y: " + (axis.y - 512) + ", Z: " + axis.z);
  _delay_ms(10);

}


void init_button (void) {

  // button input, internal pull up
  BUTTON_PORT |= BUTTON_PIN;
  BUTTON_DDR  &= BUTTON_DDR;

  // external int trigger on low
  EICRB |= (1 << ISC41) | (1 << ISC40);
 

  // enable button interrupt
  EIMSK |=  (1 << BUTTON_INT);

  // clear any existing flags
  EIFR  &= ~(1 << BUTTON_FLAG);
}
