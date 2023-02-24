#ifndef FSM_H
#define FSM_H

#include "Arduino.h"

typedef enum {
  
  Idle_State,
  Flash_State,
  Red_Green_State,
  Red_Yellow_State,
  Green_Red_State,
  Yellow_Red_State
  
}TrafficStates;


typedef enum { 

  Timer_0_Event  = 0,
  Timer_1_Event  = 1,
  Timer_17_Event = 17,
  Timer_20_Event = 20,
  Timer_23_Event = 23

}TrafficEvent;



typedef struct {
  
  uint8_t count;
  uint8_t tseg;
  bool buzzer_on;
  TrafficStates state;
  uint8_t set_led;
  
}TrafficControl;



TrafficControl traffic_fsm (TrafficControl fsm);


#endif