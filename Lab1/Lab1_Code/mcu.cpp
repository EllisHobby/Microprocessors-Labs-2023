#include "mcu.h"


void init_leds (void) {
  // led outputs, init low
  LIGHT_PORT &= ~(RED_1_PIN | YLW_1_PIN | GRN_1_PIN | RED_2_PIN | YLW_2_PIN | GRN_2_PIN);
  LIGHT_DDR  |=  (RED_1_PIN | YLW_1_PIN | GRN_1_PIN | RED_2_PIN | YLW_2_PIN | GRN_2_PIN);
}


void init_buzzer (void) {
  // buzzer output, init low
  BUZZ_PORT &= ~(BUZZ_PIN);
  BUZZ_DDR  |= (BUZZ_PIN);
}


void init_button (void) {

  // button input, internal pull up
  BUTTON_PORT |= BUTTON_PIN;
  BUTTON_DDR  &= BUTTON_DDR;

  // external int trigger on low
  EICRA = 0;
  EICRA = 0;

  // enable button interrupt
  EIMSK |=  (1 << BUTTON_INT);

  // clear any existing flags
  EIFR  &= ~(1 << BUTTON_FLAG);
}


void init_7seg_spi (void) {

  // digit control pins
  SEG_DIG_PORT &= ~(SET_DIG_D1 | SET_DIG_D2);
  SEG_DIG_DDR  |=  (SET_DIG_D1 | SET_DIG_D2);

  // spi pins output, low
  SPI_PORT &= ~(SPI_LAT | SPI_MOSI | SPI_SCK);
  SPI_DDR  |=  (SPI_LAT | SPI_MOSI | SPI_SCK);

  // set master, fclk/16, enable
  SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);

}

void init_timer3 (void) {
  
  // clear timer2 control regs
  TCCR3A = 0;
  TCCR3B = 0;
  
  // set prescale 1024 --> tclk = 16e6 / 1024 = 15625
  // clear count on compare
  TCCR3B = (1 << WGM32) | (1 << CS32) | (1 << CS30);
  
  // output compare at 1 Hz
  OCR3A = F_CLK / 1024;
  
  // clear timer count
  TCNT3 = 0;
  
  // int on OCR2A
  TIMSK3 = (1 << OCIE3A);
  
}


void set_leds (uint8_t color) {
  // set led color data, maintain state of unused pins
  LIGHT_PORT = color | (LIGHT_READ & LIGHT_UNUSED);
}


void set_buzzer (bool buzz) {

  if (buzz) {
    BUZZ_PORT |= BUZZ_PIN;
  }
  else {
    BUZZ_PORT &= ~(BUZZ_PIN);
  }

}


void set_7seg_spi (uint16_t value, uint8_t set_digit_pin, uint8_t digit) {

  // shift and mask value depending on digit
  uint8_t spi_data = (uint8_t)((value >> (digit * 8)) & 0xff);

  // set latch low
  SPI_PORT &= ~(SPI_LAT);

  // load data
  SPDR = spi_data;

  // wait until transmit finished
  while( !(SPSR & (1 << SPIF)) );

  // set latch high
  SPI_PORT |= SPI_LAT;

  // set digit control pin low
  SEG_DIG_PORT = ~(set_digit_pin);

}