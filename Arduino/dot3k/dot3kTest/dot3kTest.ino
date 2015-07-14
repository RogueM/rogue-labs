/*********************************************************************
Author: Rogue@HAL13
Version: .15714

Original DogLcd lib: e.fa@wayoda.org http://code.google.com/p/doglcd/
Original sn3218 lib: Gadgetoid@pimoroni https://github.com/pimoroni/sn3218

Description: test code Display-O-Tron 3000
Channels should be wired as follows:
  MISO - N/A
  MOSI - D2
  SCLK - D3
  CMD  - D4
  CS0  - D5
  SDA  - SDA(A4)
  SCL  - SCL(A5)
see http://pi.gadgetoid.com/pinout/display_o_tron_3000 for pinout details.
*********************************************************************/

#include <Wire.h> // import Wire(i2c) library
#include "sn3218.h" // import SN3218 library
#include <DogLcd.h> // import Dog LCD library

DogLcd lcd(2, 3, 4, 5); // initialises pins to interface 
const int brightness = 64; // user-defined brightness for backlit

void setup() {
  lcd.begin(DOG_LCD_M163, 0x28, DOG_LCD_VCC_3V3); // set LCD type and default contrast
  sn3218.begin(); // start sn3218 register access
  sn3218.enable_leds(SN3218_CH_ALL); // enable all channels for access
}
void loop() {
  
/*clear LED driver register*/
  int i; // variable for iterations
  for( i = 0; i < SN3218_NUM_CHANNELS; i++ ){
    sn3218.set(i,0); // reset all LEDs to 0/off
    sn3218.update(); // update SN3218 register
  }
  
/*activate DOT3K backlit channels*/
  for( i = 0; i < 8; i++ ){ // loop through the 9 backlit LEDs
    sn3218.set(i,brightness); // set LED to previously defined brightness
    sn3218.update(); // update SN3218 register
    delay(100); // pause before next LED is processed
  }
  
/*print text over 3 lines*/
  lcd.setCursor(0, 0); // text to print on line 1 (index 0)
  lcd.print("I could do this");
  lcd.setCursor(0, 1); // text to print on line 2 (index 1)
  lcd.print("all day and I'll");
  lcd.setCursor(0, 2); // text to print on line 3 (index 2)
  lcd.print("prove it:");
  lcd.setCursor(10,2); // place cursor at end of line 3
  lcd.print(millis()/1000); // print the number of seconds since last board reset
}

