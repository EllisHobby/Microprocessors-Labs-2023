#ifndef GAME_CONTROL_H
#define GAME_CONTROL_H

#define GAME_UP    0
#define GAME_DOWN  1
#define GAME_RIGHT 2
#define GAME_LEFT  3 

#define JOYSTICK_THRESHOLD  400
#define GYRO_THRESHOLD 1700
#define ACCEL_THRESHOLD 14


#define JOYSTICK_CONTROL  0
#define IMU_CONTROL       1

#define REQ_HANDSHAKE   0
#define REQ_DIRECTION   1
#define REQ_BUZZER      2
#define REQ_SHAKE       3


typedef struct {
  uint8_t direction = GAME_UP;
  uint8_t buzz_count = 0;
  bool shake = false;
  uint8_t controller = JOYSTICK_CONTROL;
}GameState;

#endif