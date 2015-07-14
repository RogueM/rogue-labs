/*********************************************************************
Author: Rogue@HAL13
Version: .15714

Original DogLcd lib: e.fa@wayoda.org
http://code.google.com/p/doglcd/

Description: test code for DOGM LCD of Display-O-Tron 3000 via SPI.
Prints "Hello World!" to the LCD and shows the time.

Channels should be wired as follows:
  MISO pin is not used
  MOSI pin to digital pin 2
  SCLK pin to digital pin 3
  CMD pin to digital pin 4
  CS0 pin to digital pin 5
see http://pi.gadgetoid.com/pinout/display_o_tron_3000 for details.
*********************************************************************/

#include <DogLcd.h> // import Dog LCD library
DogLcd lcd(2, 3, 4, 5); // initialises pins to interface 

void setup() {
  lcd.begin(DOG_LCD_M163, 0x28, DOG_LCD_VCC_3V3); // set LCD type and default contrast
  lcd.print("Hello World!"); // print message to LCD
}

void loop() {
  lcd.setCursor(0, 1); // set cursor to column 0, line 1 (line 1 is the second row)
  lcd.print(millis()/1000); // print the number of seconds since last board reset
}
