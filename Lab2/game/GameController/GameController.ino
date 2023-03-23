#include "analog_joystick.h"
#include "mpu6050.h"
#include "game_control.h"


// button 
#define BUTTON_PORT       PORTE
#define BUTTON_DDR        DDRE
#define BUTTON_READ       PINE
#define BUTTON_PIN        (1 << PE4)
#define BUTTON_INT        INT4
#define BUTTON_FLAG       INTF4
#define BUTTON_INT_VECT   INT4_vect

// buzzer
#define BUZZ_PORT   PORTL
#define BUZZ_DDR    DDRL
#define BUZZ_PIN    (1 << PL0)



IMU imu;
JoyStick joystick;
Axis control;
GameState game;

uint8_t counter = 0;

ISR (BUTTON_INT_VECT) {
    
  if (game.controller == JOYSTICK_CONTROL) {
    game.controller = IMU_CONTROL;
  }
  else {
    game.controller = JOYSTICK_CONTROL;
  }
}


ISR (TIMER3_COMPA_vect) { 

  game.buzz_count++;
}


void setup() {

  cli();
  
  Serial.begin(9600);
  
  imu.init();
  imu.setRange(GYRO_RANGE_2000, ACCEL_RANGE_16);
  imu.calibrate();
  
  initButton();
  initBuzzer();
  initTimer3();
  
  sei();
}


void loop() {
  
  if (game.controller == JOYSTICK_CONTROL) {
    control = joystick.read();
    joystickDirection();
  }
  else {
    control = imu.read();
    imuDirection();
  }

  if (Serial.available() > 0) {
    gameDispatch(Serial.read());
  }
  
  if (game.buzz_count > 1) {
    BUZZ_PORT &= ~(BUZZ_PIN);   // stop buzzer
    TIMSK3 = 0;                 // disable timer3 interrupt 
    game.buzz_count = 0;
  }
}


void initBuzzer (void) {
  // buzzer output, init low
  BUZZ_PORT &= ~(BUZZ_PIN);
  BUZZ_DDR  |= (BUZZ_PIN);
}


void initButton (void) {

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


void initTimer3 (void) {
  
  // clear timer3 control regs
  TCCR3A = 0;
  TCCR3B = 0;
  
  // set prescale 1024 --> tclk = 16e6 / 1024 = 15625
  // clear count on compare
  TCCR3B = (1 << WGM32) | (1 << CS32) | (1 << CS30);
  
  // output compare at 1 Hz
  OCR3A = (F_CPU) / (1024);
  
  // clear timer count
  TCNT3 = 0;
  
  // disable int on OCR2A
  TIMSK3 = 0;  
}


void joystickDirection (void) {
  if ((control.x > JOYSTICK_THRESHOLD) && (game.direction != GAME_LEFT) && (abs(control.x) > abs(control.y))) {
    game.direction = GAME_RIGHT;
  }
  if ((control.x < -JOYSTICK_THRESHOLD) && (game.direction != GAME_RIGHT) && (abs(control.x) > abs(control.y))) {
    game.direction = GAME_LEFT;
  }
  if ((control.y < -JOYSTICK_THRESHOLD) && (game.direction != GAME_DOWN) && (abs(control.y) > abs(control.x))) {
    game.direction = GAME_UP;
  }
  if ((control.y > JOYSTICK_THRESHOLD) && (game.direction != GAME_UP) && (abs(control.y) > abs(control.x))) {
    game.direction = GAME_DOWN;
  }
}


void imuDirection (void) {
  if ((control.x > GYRO_THRESHOLD) && (game.direction != GAME_LEFT) && (abs(control.x) > abs(control.y))) {
    game.direction = GAME_RIGHT;
  }
  if ((control.x < -GYRO_THRESHOLD) && (game.direction != GAME_RIGHT) && (abs(control.x) > abs(control.y))) {
    game.direction = GAME_LEFT;
  }
  if ((control.y > GYRO_THRESHOLD) && (game.direction != GAME_DOWN) && (abs(control.y) > abs(control.x))) {
    game.direction = GAME_UP;
  }
  if ((control.y < -GYRO_THRESHOLD) && (game.direction != GAME_UP) && (abs(control.y) > abs(control.x))) {
    game.direction = GAME_DOWN;
  }
  if ((abs(control.z) > ACCEL_THRESHOLD) && (abs(control.x) < GYRO_THRESHOLD) && (abs(control.y) < GYRO_THRESHOLD)) {
    game.shake = true;
  }
}


void gameDispatch (int request) {
  
  switch (request) {

    case REQ_HANDSHAKE:
      if (Serial.availableForWrite()) {
        Serial.println("Connected");
      }
      break;

    case REQ_DIRECTION:
      if (Serial.availableForWrite()) {
        Serial.println((String) "{\"dir\": " + game.direction + ", \"shake\": " + game.shake + "}");
      }
      break;

    case REQ_BUZZER:
      BUZZ_PORT |= BUZZ_PIN;    // start buzzer
      TIMSK3 = (1 << OCIE3A);   // enable timer3 interrupt
      break;

    case REQ_SHAKE:
      game.shake = false;
    
    default:
      break;
  }
}
