/*********************************************************************
Author: Rogue@HAL13
Version: .15714

Original sn3218 lib: Gadgetoid@pimoroni
https://github.com/pimoroni/sn3218

Description: test code for Joystick and bargraph (SN3218 9-17)
of Display-O-Tron 3000. Channels accessed as follows:
    SN3218_CH_09 baroled 1
    SN3218_CH_10 baroled 2
    SN3218_CH_11 baroled 3
    SN3218_CH_12 baroled 4
    SN3218_CH_13 baroled 5
    SN3218_CH_14 baroled 6
    SN3218_CH_15 baroled 7
    SN3218_CH_16 baroled 8
    SN3218_CH_17 baroled 9
    JOYSTICK_DWN Digital 6
    JOYSTICK_UP  Digital 7
    JOYSTICK_DWN Digital 8
    JOYSTICK_UP  Digital 9
    JOYSTICK_TRG Digital 10
*********************************************************************/

#include <Wire.h> // import Wire(i2c) library
#include "sn3218.h" // import SN3218 library

const int brightness = 16; // user-defined brightness for backlit
const int pace = 100;
const int downPin = 6; // pin used for joystick down
const int upPin = 7; // pin used for joystick up
const int rightPin = 8; // pin used for joystick right
const int leftPin = 9; // pin used for joystick left
const int trigPin = 10; // pin used for joystick trigger

int downState = 0; // variable used for the joystick down status
int upState = 0; // variable used for the joystick up status
int leftState = 0; // variable used for the joystick left status
int rightState = 0; // variable used for the joystick right status
int trigState = 0; // variable used for the joystick trigger status

void setup() {
  sn3218.begin(); // start sn3218 register access
  sn3218.enable_leds(SN3218_CH_ALL); // enable all channels for access
  pinMode(downPin, INPUT_PULLUP); // define joystick down input with pull up
  pinMode(upPin, INPUT_PULLUP); // define joystick up input with pull up
  pinMode(rightPin, INPUT_PULLUP); // define joystick right input with pull up
  pinMode(leftPin, INPUT_PULLUP); // define joystick left input with pull up
  pinMode(trigPin, INPUT_PULLUP); // define joystick trig input with pull up
}

void loop() {
  int i; // variable for iterations
  clear_ch_all(); // call reset function defined at the end of this sketch
  downState = digitalRead(downPin); // probing for joystick down
  upState = digitalRead(upPin); // probing for joystick up
  rightState = digitalRead(rightPin); // probing for joystick right
  leftState = digitalRead(leftPin); // probing for joystick left
  trigState = digitalRead(trigPin); // probing for joystick trig

  if (downState == LOW | leftState == LOW) { // down or left detected
    clear_ch_all(); // call reset function defined at the end of this sketch
    for( i = 17; i >= 9; i-- ){ // loop through the 9 bargraph LEDs
      sn3218.set(i,brightness); // set LED to previously defined brightness
      sn3218.update(); // update SN3218 register
      delay(pace); // pause before next LED is processed
    }
    clear_ch_all(); // call reset function defined at the end of this sketch
  }
  if (upState == LOW | rightState == LOW) { // up or right detected
    clear_ch_all(); // call reset function defined at the end of this sketch
    for( i = 9; i <= 17; i++ ){ // loop through the 9 bargraph LEDs
      sn3218.set(i,brightness); // set LED to previously defined brightness
      sn3218.update(); // update SN3218 register
      delay(pace); // pause before next LED is processed
    }
    clear_ch_all(); // call reset function defined at the end of this sketch
  }

  if (trigState == LOW) { // trigger button detected
    clear_ch_all(); // call reset function defined at the end of this sketch
    for( i = 9; i <= 17; i++ ){ // loop through the 9 bargraph LEDs
      sn3218.set(i,brightness); // set LED to previously defined brightness
      sn3218.update(); // update SN3218 register
      delay(pace); // pause before next LED is processed
    }
    for( i = 17; i >= 9; i-- ){ // loop through the 9 bargraph LEDs
      sn3218.set(i,0); // set LED to previously defined brightness
      sn3218.update(); // update SN3218 register
      delay(pace); // pause before next LED is processed
    }
    clear_ch_all(); // call reset function defined at the end of this sketch
  }
}

void clear_ch_all() { // reset function
  int i; // variable for iterations
  for( i = 0; i < SN3218_NUM_CHANNELS; i++ ){ // loop through all 18 channels (incl. backlit)
    sn3218.set(i,0); // reset all LEDs to 0/off
    sn3218.update(); // update SN3218 register
  }
}

