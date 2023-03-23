#include "mpu6050.h"
#include "game_control.h"

IMU imu;
Axis control;
GameState game;

void setup() {
  
  Serial.begin(9600);
  _delay_ms(500);
  
  Serial.println("Start MPU 6050 Test");
  
  imu.init();
  //imu.setDebugLevel(DEBUG_FUNC);
  imu.setRange(GYRO_RANGE_2000, ACCEL_RANGE_16);
  imu.testConnection();
  imu.calibrate();
  //imu.setDebugLevel(DEBUG_OFF);
  
  //while(1);         // uncomment to help verify setup
}

void loop() {

  //debugControl();   // uncomment to see incoming control values
  
  control = imu.read();
  
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


void printDirection (void) {
  
  if ((abs(control.z) > ACCEL_THRESHOLD) && (abs(control.x) < GYRO_THRESHOLD) && (abs(control.y) < GYRO_THRESHOLD)) {
    Serial.println("------ SHAKE ------");
  }

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

  imu.setDebugLevel(DEBUG_CONTROL); 
  while(1) { 
    imu.read(); 
    _delay_ms(200); 
  }  
}