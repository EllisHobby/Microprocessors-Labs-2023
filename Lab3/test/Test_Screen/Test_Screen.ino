/*
  Author: Ellis Hobby
  Info:
    > Tests different LCD functions
    > draws zig zag start pattern
    > tests EOL handling
    > simulates clock/fan info display
*/

#include "screen.h"


LCD lcd;

void setup() {
  
  Serial.begin(9600);
  _delay_ms(100);
  
  Serial.println("LCD Test.....");
  Serial.println("Printing test lines");
  _delay_ms(1000);

  lcd.init();
}

void loop() {
  
  drawZigZag();
  testEOL();
  testStringFormat();
}


void drawZigZag(void) {
  
  for(uint8_t i=0; i < END_LINE_ONE; i=i+2) {
    
    lcd.gotoPosition(i);
    lcd.write("*");
    _delay_ms(200);
    
    lcd.gotoPosition(i+17);
    lcd.write("*");
    _delay_ms(200);
  }
  
  _delay_ms(2000);
  lcd.clear();
  lcd.home();
}


void testEOL(void) {
  
  lcd.write("Testing\nEOL");
  _delay_ms(2000);
  lcd.clear();
  lcd.home();
  
  lcd.write("Is it\nworking???");
  _delay_ms(2000);
  lcd.clear();
  lcd.home();
}


void testStringFormat(void) {
  
  char buffer[50];
  char sec_buff[3];
  char min_buff[3];
  char hr_buff[3];

  uint8_t min = 8;
  uint8_t hr  = 7;
  const char* dir = "CW ";
  const char* speed = "MAX";
  
  for(uint8_t sec=0; sec < 60; sec++) {
    
    if (sec < 10) {
      sprintf(sec_buff, "0%u", sec);
    }
    else {
      sprintf(sec_buff, "%u", sec);
    }
    if (min < 10) {
      sprintf(min_buff, "0%u", min);
    }
    else {
      sprintf(min_buff, "%u", min);
    }
    if (hr < 10) {
      sprintf(hr_buff, "0%u", hr);
    }
    else {
      sprintf(hr_buff, "%u", hr);
    }

    sprintf(buffer, "%s:%s:%s", hr_buff, min_buff, sec_buff);
    lcd.gotoLineOne();
    lcd.write(buffer);
    
    sprintf(buffer, "R:%s", dir);
    lcd.gotoPosition(END_LINE_ONE - strlen(buffer) + 1);
    lcd.write(buffer);    
    
    sprintf(buffer, "S:%s", speed);
    lcd.gotoPosition(END_LINE_TWO - strlen(buffer) + 1);
    lcd.write(buffer);
    
    _delay_ms(1000);
  }
  _delay_ms(1000);
  lcd.clear();
  lcd.home();
}
