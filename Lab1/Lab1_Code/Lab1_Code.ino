/*

*/
#include "mcu.h"
#include "seg7.h"
#include "fsm.h"


TrafficControl fsm;


ISR (TIMER3_COMPA_vect) { 
  
  // increment counter
  fsm.count++;

  // calc 7seg countdown value
  fsm.tseg--; 
}

ISR (BUTTON_INT_VECT) {

  // disable button int, only need once
  EIMSK &= ~(1 << BUTTON_INT);
    
  // start traffic cycle state
  fsm.state = Red_Green_State;
  fsm.count = 0;
  fsm.tseg  = 23;
  fsm.set_led = SET_LIGHT_RG;
  
  // clear timer count
  TCNT3 = 0;

}

void setup (void) {
  
  cli();
  
  // initiate peripherals
  init_leds();
  init_buzzer();
  init_button();
  init_7seg_spi();
  init_timer3();
  
  // initiate fsm controller
  fsm.count = 0;
  fsm.tseg  = 1;
  fsm.state = Idle_State;
  fsm.set_led = SET_LIGHT_CLR;
  fsm.buzzer_on = false;

  // slight delay for good measure
  F_CLK_DELAY_CYCLES(F_CLK);
  
  sei();
  
}

void loop (void) {
  
  // set leds and buzzer
  set_leds(fsm.set_led);
  set_buzzer(fsm.buzzer_on);

  // set two 7 segment digits 
  set_7seg_spi(Seg_Count[fsm.tseg-1], SET_DIG_D1, 0);
  set_7seg_spi(Seg_Count[fsm.tseg-1], SET_DIG_D2, 1);

  

  // check for new state
  switch (fsm.state) {
    
    case Idle_State:
      if (fsm.count == Timer_1_Event) {
        fsm.set_led = SET_LIGHT_RR;
        fsm.count = 0;
        fsm.tseg  = 1;
        fsm.state = Flash_State;
      }
      break;
   
    case Flash_State:
      if (fsm.count == Timer_1_Event) {
        fsm.set_led = SET_LIGHT_CLR;
        fsm.count = 0;
        fsm.tseg  = 1;
        fsm.state = Idle_State;
      }
      break;
    
    case Red_Green_State:
      if (fsm.count == Timer_20_Event) {
        fsm.set_led = SET_LIGHT_RY;
        fsm.buzzer_on = true;
        fsm.state = Red_Yellow_State;
      }
      break;
    
    case Red_Yellow_State:
      if (fsm.count == Timer_23_Event) {
        fsm.set_led = SET_LIGHT_GR;
        fsm.count = 0;
        fsm.tseg  = 23;
        fsm.buzzer_on = false;
        fsm.state = Green_Red_State;
      }
      break;

    case Green_Red_State:
      if (fsm.count == Timer_20_Event) {
        fsm.set_led = SET_LIGHT_YR;
        fsm.buzzer_on = true;
        fsm.state = Yellow_Red_State;        

      }
      break;

    case Yellow_Red_State:
      if (fsm.count == Timer_23_Event) {
        fsm.set_led = SET_LIGHT_RG;
        fsm.count = 0;
        fsm.tseg  = 23;
        fsm.buzzer_on = false;
        fsm.state = Red_Green_State;
      }
      break;

    default:
      break;
  }

}