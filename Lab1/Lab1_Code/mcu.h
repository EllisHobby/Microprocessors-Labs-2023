#ifndef MCU_H
#define MCU_H

#include "Arduino.h"



// leds
#define LIGHT_PORT    PORTF
#define LIGHT_DDR     DDRF
#define LIGHT_READ    PINF

// led pins
#define RED_1_PIN   (1 << PF0)
#define YLW_1_PIN   (1 << PF1)
#define GRN_1_PIN   (1 << PF2)
#define RED_2_PIN   (1 << PF3)
#define YLW_2_PIN   (1 << PF4)
#define GRN_2_PIN   (1 << PF5)

// led binary values
#define SET_LIGHT_CLR   0b00000000
#define SET_LIGHT_RR    0b00001001
#define SET_LIGHT_RG    0b00001100
#define SET_LIGHT_RY    0b00001010
#define SET_LIGHT_GR    0b00100001
#define SET_LIGHT_YR    0b00010001

#define LIGHT_UNUSED    0b11000000



// buzzer
#define BUZZ_PORT   PORTF
#define BUZZ_DDR    DDRF
#define BUZZ_PIN    (1 << PF6)



// button 
#define BUTTON_PORT   PORTD
#define BUTTON_DDR    DDRD
#define BUTTON_READ   PIND
#define BUTTON_PIN    (1 << PD0)
#define BUTTON_INT    INT0
#define BUTTON_FLAG   INTF0
#define BUTTON_INT_VECT   INT0_vect



// spi, used for 7 segment
#define SPI_PORT  PORTB
#define SPI_DDR   DDRB
#define SPI_LAT   (1 << PB0)  
#define SPI_SCK   (1 << PB1)
#define SPI_MOSI  (1 << PB2)



// 4 digit 7 segment select pins
#define SEG_DIG_PORT  PORTA
#define SEG_DIG_DDR   DDRA
#define SEG_DIG_READ  PINA

#define SET_DIG_D1    (1 << PA0) 
#define SET_DIG_D2    (1 << PA1)



// accurate low level delay
#define F_CLK_DELAY_CYCLES(n) __builtin_avr_delay_cycles(n)
#define F_CLK   16e6


void init_leds (void);
void init_button (void);
void init_buzzer (void);
void init_timer3 (void);
void init_7seg_spi (void);

void set_leds (uint8_t color);
void set_buzzer (bool on);
void set_7seg_spi (uint16_t value, uint8_t set_digit_pin, uint8_t digit);

#endif