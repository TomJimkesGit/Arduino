/*
  Arduino Starter Kit example
  Project 11 - Crystal Ball

  This sketch is written to accompany Project 11 in the Arduino Starter Kit

  Parts required:
  - 220 ohm resistor
  - 10 kilohm resistor
  - 10 kilohm potentiometer
  - 16x2 LCD screen
  - tilt switch

  created 13 Sep 2012
  by Scott Fitzgerald

  https://store.arduino.cc/genuino-starter-kit

  This example code is part of the public domain.
*/

// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
//LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
//LiquidCrystal lcd(26, 25, 33, 32, 35, 34);
LiquidCrystal lcd(13, 12, 14, 27, 26, 25);

void setup() {
  // set up the number of columns and rows on the LCD
  lcd.begin(16, 2);

  // Print a message to the LCD.
  lcd.print("Ask the");
  // set the cursor to column 0, line 1
  // line 1 is the second row, since counting begins with 0
  lcd.setCursor(0, 1);
  // print to the second line
  lcd.print("Crystal Ball!");
}

void loop() {
  
}
