/*
  Author: Ellis Hobby
  Info:
    > LCD class definitions
*/
#include "screen.h"



void LCD::init(void) {

  // set data port, 8-bit
  DATA_DDR  = 0xFF;
  DATA_PORT =  0;

  // set register select
  RS_DDR  |= (1 << RS_BIT);
  RS_PORT &= ~(1 << RS_BIT);

  // set enable ouput
  EN_DDR  |= (1 << EN_BIT);
  EN_PORT &= ~(1 << EN_BIT);

  // set r/w ouput
  RW_DDR  |= (1 << RW_BIT);
  RW_PORT &= ~(1 << RW_BIT);

  // warm up
  _delay_ms(100);
  
  clear();
  home();
  entryIncrement();
  cursorOff();
  blinkOff();
  screenOn();
  formatTwoLine();
  fontSize5x8();
}



/*-----------------------------

      writing instructions

-----------------------------*/
void LCD::_command(uint8_t data) {

  // register select low to send instruction
  RS_PORT &= ~(1 << RS_BIT);

  // set data pins, strobe enable
  DATA_PORT = data;
  EN_PORT |= (1 << EN_BIT);
  EN_PORT &= ~(1 << EN_BIT);
  _delay_ms(2);

  // register select high for normal writes
  RS_PORT |= (1 << RS_BIT);
}



/*-----------------------------

      clear screen instruction

-----------------------------*/
void LCD::clear(void) {
  _command(LCD_CLEAR);
}



/*-----------------------------

      go home instruction

-----------------------------*/
void LCD::home(void) {
  _ddram.address = START_LINE_ONE;
  _command(LCD_HOME);
}



/*-----------------------------

      entry control

  -----------------------------*/
void LCD::entryIncrement(void) {
  _entry.increment = INCREMENT;
  _command(_entry.instruction);
}

void LCD::entryDecrement(void) {
  _entry.increment = DECREMENT;
  _command(_entry.instruction);
}

void LCD::entryShiftOn(void) {
  _entry.shift = SHIFT_ON;
  _command(_entry.instruction);
}

void LCD::entryShiftOff(void) {
  _entry.shift = SHIFT_OFF;
  _command(_entry.instruction);
}



/*-----------------------------

      display control

  -----------------------------*/
void LCD::cursorOn(void) {
  _display.cursor = CURSOR_ON;
  _command(_display.instruction);
}

void LCD::cursorOff(void) {
  _display.cursor = CURSOR_OFF;
  _command(_display.instruction);
}

void LCD::blinkOn(void) {
  _display.blink = BLINK_ON;
  _command(_display.instruction);
}

void LCD::blinkOff(void) {
  _display.cursor = BLINK_OFF;
  _command(_display.instruction);
}

void LCD::screenOn(void) {
  _display.screen = SCREEN_ON;
  _command(_display.instruction);
}

void LCD::screenOff(void) {
  _display.screen = SCREEN_OFF;
  _command(_display.instruction);
}


/*-----------------------------

      shift control

  -----------------------------*/
void LCD::cursorRight(void) {
  _shifter.direction = SHIFT_RIGHT;
  _shifter.select = SHIFT_CURSOR;
  _command(_shifter.instruction);
}

void LCD::cursorLeft(void) {
  _shifter.direction = SHIFT_LEFT;
  _shifter.select = SHIFT_CURSOR;
  _command(_shifter.instruction);
}

void LCD::moveRight(void) {
  _shifter.direction = SHIFT_RIGHT;
  _shifter.select = SHIFT_SCREEN;
  _command(_shifter.instruction);
}

void LCD::moveLeft(void) {
  _shifter.direction = SHIFT_LEFT;
  _shifter.select = SHIFT_SCREEN;
  _command(_shifter.instruction);
}


/*-----------------------------

      format control

  -----------------------------*/
void LCD::formatOneLine(void) {
  _format.lines = ONE_LINE;
  _command(_format.instruction);
}

void LCD::formatTwoLine(void) {
  _format.lines = TWO_LINE;
  _command(_format.instruction);
}

void LCD::fontSize5x8(void) {
  _format.font = FONT_5X8;
  _command(_format.instruction);
}

void LCD::fontSize5x10(void) {
  _format.font = FONT_5X10;
  _command(_format.instruction);
}



/*-----------------------------

      ddram control

  -----------------------------*/
void LCD::gotoPosition(uint8_t pos) {
  if (pos > END_LINE_ONE) {
    pos = (pos - END_LINE_ONE) + DDRAM_START_LINE_TWO - 1;
  }
  _ddram.address = pos;
  _command(_ddram.instruction);

}
void LCD::gotoLineOne(void) {
  gotoPosition(START_LINE_ONE);
}
void LCD::gotoLineTwo(void) {
  gotoPosition(START_LINE_TWO);
}



/*-----------------------------

      LCD write functions

  -----------------------------*/
void LCD::_write(uint8_t data) {
  
  // wrap lines when going off screen
  if ((_ddram.address > DDRAM_END_LINE_ONE) && (_ddram.address < DDRAM_START_LINE_TWO)) {
    gotoLineTwo();
    _ddram.address--;   // helps track properly, we increment one at end
  }
  else if ((_ddram.address > DDRAM_END_LINE_TWO)) {
    gotoLineOne();
    _ddram.address--;   // helps track properly, we increment one at end
  }
  
  // handle new line character "\n"
  if ((data == EOL_ASCII) && (_ddram.address < END_LINE_ONE)) {
    gotoLineTwo();
    return;
  }
  else if ((data == EOL_ASCII) && (_ddram.address > END_LINE_ONE)) {
    clear();
    gotoLineOne();
    return;
  }

  // set data pins, strobe enable
  DATA_PORT = data;
  EN_PORT |= (1 << EN_BIT);
  
  EN_PORT &= ~(1 << EN_BIT);
  _delay_ms(2);
  

  _ddram.address++;
}


void LCD::write(int data) {
  _buffer = data;
  _write(_buffer);
}


void LCD::write(uint8_t data) {
  _buffer = data;
  _write(_buffer);
}


void LCD::write(const char* data) {
  
  while (*data) {
    
    _buffer = *data;
    
    // handle string termination
    if (_buffer == NULL_ASCII) {
      return;
    }
    
    _write(_buffer);
    *data++;
  }
}
