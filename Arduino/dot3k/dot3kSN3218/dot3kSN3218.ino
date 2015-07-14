/*********************************************************************
Author: Rogue@HAL13
Version: .15714

Original sn3218 lib: Gadgetoid@pimoroni
https://github.com/pimoroni/sn3218

Description: test code for SN3218 chip driving backlit and bargraph
of Display-O-Tron 3000 via i2C. Channels accessed as follows:
    SN3218_CH_00 backlit right R
    SN3218_CH_01 backlit right G(B)
    SN3218_CH_02 backlit right B(G)
    SN3218_CH_03 backlit middle R
    SN3218_CH_04 backlit middle G(B)
    SN3218_CH_05 backlit middle B(G)
    SN3218_CH_06 backlit left R
    SN3218_CH_07 backlit left G(B)
    SN3218_CH_08 backlit left B(G)
    SN3218_CH_09 baroled 1 (leftmost)
    SN3218_CH_10 baroled 2
    SN3218_CH_11 baroled 3
    SN3218_CH_12 baroled 4
    SN3218_CH_13 baroled 5
    SN3218_CH_14 baroled 6
    SN3218_CH_15 baroled 7
    SN3218_CH_16 baroled 8
    SN3218_CH_17 baroled 9 (rightmost)
    SN3218_CH_ALL affects all channels
*********************************************************************/

#include <Wire.h> // import Wire(i2c) library
#include "sn3218.h" // import SN3218 library

const int brightness = 128; // user-defined brightness for backlit

void setup() {
  sn3218.begin(); // start sn3218 register access
  sn3218.enable_leds(SN3218_CH_ALL); // enable all channels for access
}

void loop() {
  int i; // variable for iterations
  for( i = 0; i < SN3218_NUM_CHANNELS; i++ ){ // loop through all 18 channels (incl. bargraph)
    sn3218.set(i,0); // reset all LEDs to 0/off
    sn3218.update(); // update SN3218 register
  }
  for( i = 0; i < 8; i++ ){ // loop through the 9 backlit LEDs (<17 to include bargraph) 
    sn3218.set(i,brightness); // set LED to previously defined brightness
    sn3218.update(); // update SN3218 register
    delay(100); // pause before next LED is processed
  }
}

