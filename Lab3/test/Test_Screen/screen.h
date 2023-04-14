/*
  Author: Ellis Hobby
  Info:
    > LCD initialized to 8-bit data communication
    > Data control DB0-DB1 connected to PORTF
    > RS pin connected to PK0
    > RW pin connected to PK1
    > EN pin connected to PK2
    > Includes LCD control command methods
    > write function overload for int, char, const char*
    > handles EOL and NULL characters for fomatting
*/

#ifndef SCREEN_H_
#define SCREEN_H_

#include "avr/io.h"
#include "util/delay.h"
#include "screen_instruction.h"

#define DATA_PORT   PORTF
#define DATA_DDR    DDRF

#define RS_PORT     PORTK
#define RS_DDR      DDRK
#define RS_BIT      PK0

#define RW_PORT     PORTK
#define RW_DDR      DDRK
#define RW_BIT      PK1

#define EN_PORT     PORTK
#define EN_DDR      DDRK
#define EN_BIT      PK2

#define EOL_ASCII   0x0A
#define NULL_ASCII  0x00

class LCD {

  private:

    Entry   _entry;
    Display _display;
    Shifter _shifter;
    Format  _format;
    CGRAM   _cgram;
    DDRAM   _ddram;

    uint8_t _buffer;

    void _command(uint8_t data);
    void _write(uint8_t data);


  public:
    void init(void);
  
    void clear(void);
    void home(void);
    void reset(void);

    void entryIncrement(void);
    void entryDecrement(void);
    void entryShiftOn(void);
    void entryShiftOff(void);

    void cursorOn(void);
    void cursorOff(void);
    void blinkOn(void);
    void blinkOff(void);
    void screenOn(void);
    void screenOff(void);
    
    void cursorRight(void);
    void cursorLeft(void);
    void moveRight(void);
    void moveLeft(void);

    void formatOneLine(void);
    void formatTwoLine(void);
    void fontSize5x8(void);
    void fontSize5x10(void);
    
    void gotoPosition(uint8_t pos);
    void gotoLineOne(void);
    void gotoLineTwo(void);

    void write(int data);
    void write(uint8_t data);
    void write(const char* data);
};

#endif
