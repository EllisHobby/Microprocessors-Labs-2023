/*
  Author: Ellis Hobby
  Info:
    > defines bit-fields for accessing HD44780U registers
    > enables easier tracking of reg states for control
*/

#ifndef LCD_INSTRUCTION_H_
#define LCD_INSTRUCTION_H_


#include "stdint.h"

#define LCD_CLEAR           0x01
#define LCD_HOME            0x02


/* 
    entry mode instructions 
*/
#define DECREMENT   0
#define INCREMENT   1
#define SHIFT_OFF   0
#define SHIFT_ON    1

typedef union _entry_mode {
    struct {
      uint8_t shift       : 1;
      uint8_t increment   : 1;
      const uint8_t id_   : 1;
      const uint8_t pad_  : 5;
    };
    uint8_t instruction;
    _entry_mode() : shift(0), increment(1), id_(1), pad_(0) {};
} Entry;


/* 
    display control instructions 
*/
#define SCREEN_OFF 0
#define SCREEN_ON  1
#define CURSOR_OFF  0
#define CURSOR_ON   1
#define BLINK_OFF   0
#define BLINK_ON    1

typedef union _display_control {
    struct {
      uint8_t blink       : 1;
      uint8_t cursor      : 1;
      uint8_t screen      : 1;
      const uint8_t id_   : 1;
      const uint8_t pad_  : 4;
    };
    uint8_t instruction;
    _display_control() : blink(0), cursor(0), screen(0), id_(1), pad_(0) {};
} Display;


/* 
    shift control instructions 
*/
#define SHIFT_LEFT    0
#define SHIFT_RIGHT   1
#define SHIFT_CURSOR  0
#define SHIFT_SCREEN  1

typedef union _shift_control {
    struct {
      const uint8_t dontcare_  : 2;
      uint8_t direction         : 1;
      uint8_t select            : 1;
      const uint8_t id_         : 1;
      const uint8_t pad_        : 3;
    };
    uint8_t instruction;
    _shift_control() : dontcare_(0), direction(0), select(0), id_(1), pad_(0) {};
} Shifter;


/* 
    functionality set instructions 
*/
#define DATA_4BIT   0
#define DATA_8BIT   1
#define ONE_LINE    0
#define TWO_LINE    1
#define FONT_5X8    0
#define FONT_5X10   1

typedef union _functionality {
    struct {
      const uint8_t dontcare_  : 2;
      uint8_t font              : 1;
      uint8_t lines             : 1;
      uint8_t data_len          : 1;
      const uint8_t id_         : 1;
      const uint8_t pad_        : 2;
    };
    uint8_t instruction;
    _functionality() : dontcare_(0), font(0), lines(0), data_len(1), id_(1), pad_(0) {};
} Format;


/* 
    CGRAM access instructions 
*/
typedef union _cgram_addr {
    struct {
      uint8_t address     : 6;
      const uint8_t id_   : 1;
      const uint8_t pad_  : 1;
    };
    uint8_t instruction;
    _cgram_addr() : address(0), id_(1), pad_(0) {};
} CGRAM;


/* 
    DDRAM access instructions 
*/
#define START_LINE_ONE  0
#define END_LINE_ONE    15
#define START_LINE_TWO  16
#define END_LINE_TWO    31
#define MAX_CHARS       31
#define DDRAM_START_LINE_ONE  0x00
#define DDRAM_END_LINE_ONE    0x0F
#define DDRAM_START_LINE_TWO  0x40
#define DDRAM_END_LINE_TWO    0x4F
typedef union _ddram_addr {
    struct {
      uint8_t address     : 7;
      const uint8_t id_   : 1;
    };
    uint8_t instruction;
    _ddram_addr() : address(0), id_(1) {};
} DDRAM;



#endif
