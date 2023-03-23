#include "util/delay.h"
#include "analog_joystick.h"
#include "game_control.h"




JoyStick joystick;
Axis control;
GameState game;


void setup() {

  Serial.begin(9600);
  _delay_ms(500);
  
  Serial.println("Start Joystick Test");
  _delay_ms(2000);
}


void loop() {

  //debugControl();   // uncomment to see incoming control values

  control = joystick.read();

  checkDirection();
  //printDirection();
  if (Serial.available() > 0) {
    gameDispatch(Serial.read());
  }
  
  _delay_ms(100);

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
        printGameDict(game.direction, game.shake);
        break;
      }
    default:
      break;
  }
}


void printGameDict (uint8_t dir, bool shake) {
  Serial.println((String) "{\"dir\": " + dir + ", \"shake\": " + shake + "}");
}


void checkDirection (void) {

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


void printDirection (void) {

  switch(game.direction) {
    case GAME_UP:
      Serial.println("W - UP");
      break;
    case GAME_DOWN:
      Serial.println("S - DOWN");
      break;
    case GAME_LEFT:
      Serial.println("A - LEFT");
      break;
    case GAME_RIGHT:
      Serial.println("D - RIGHT");
      break;
    default:
      break;
  }
}


void debugControl (void) {

  joystick.setDebugLevel(DEBUG_CONTROL); 
  while(1) { 
    joystick.read(); 
    _delay_ms(200); 
  }  
}