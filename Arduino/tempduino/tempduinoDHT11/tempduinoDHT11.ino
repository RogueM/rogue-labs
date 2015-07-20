/*********************************************************************
Author: Rogue@HAL13
Version: .15710

Description: pseudo-morse code signalling of DHT11 reading via LEDs
Digits are represented by brieves and longues according to their value
1-5 use brieves and 6-0 use longues. Inference is used to speed up signalling
for example 2(..___) is abridged '..'; 7(__...) is abridged '__'.
Includes support for thresholds and rounding indication.
*********************************************************************/

#include <dht.h> // import dht library
dht DHT; // alias declaration
int dhtCheck = 0; // variable used to check DHT functionality
const float calibH = 0.85; // calibration factor for Humidity (typical ~0.85-8.875)
const float calibT = 0.85; // calibration factor for Temperature (typical ~0.85-8.875)
const float tempFlag = 25.0; // temp in C to flag if exceeded
const float humFlag = 40.0; // hum in % to flag if exceeded

const int dhtPin = A2; // pin with DHT11 attached
const int ledPinR = 2; // pin with R LED attached for 1st digit
const int ledPinG = 3; // pin with G LED attached for 2nd digit
const int ledPinB = 9; // on-board LED for Humidity reading
const int ledPins [ ] = {ledPinR,ledPinG,ledPinB}; // array of all LED pins
const int ledCount = 3; // variable to hold LED pins count
int ledUsed = 13; // variable to hold LED currently handled

const int pulse = 50; // duration for pulse signals
const int brieve = 250; // duration for brieve signals
const int longue = 750; // duration for long signals
const int pause = 250; // duration of pauses (blink)
const int wait = 1000; // duration for wait before next cycle
int duration = 250; // variable to hold duration of signals

void setup() {
  // start serial at 115200 bauds for debugging
  serialInit();
  // integrity check of DHT
  DHTcheck();
  // initialize LED pins as output
  for (int ledUsed = 0; ledUsed < ledCount; ledUsed++)  {
    pinMode(ledPins[ledUsed], OUTPUT);      
    digitalWrite(ledUsed, LOW);
  }
}

void blink(int ledUsed, int duration) {
  digitalWrite(ledUsed, HIGH);
  delay(duration);
  digitalWrite(ledUsed, LOW);
  delay(pause);
}

void loop() {
  int i; // variable for iterations
  
  // reading values from DHT
  float hReading = DHT.humidity*calibH; // H reading from DHT
  float tReading = DHT.temperature*calibT; // T reading from DHT
  int degreeC = round(tReading); // rounding T reading into an integer
  int secondDigit = (int)degreeC % 10; // extract 2nd digit
  int firstDigit = ((int)degreeC-secondDigit)/10; // extract 1st digit
  // display values to serial
  DHTcheck();
  printValues(hReading,tReading);
  
  // display first digit via R LED
  if (firstDigit == 0) { // special case of 0
    for (i = 1; i <= 5; i++) { // flashing longue 5 times
      blink(ledPinR, longue);
    }
  }
  else {
    if (firstDigit <= 5) { // low digits
      for (i = 1; i <= firstDigit; i++) {
        blink(ledPinR,brieve); // flashing brieves
      }
    }
    else {
      for (i = 1; i <= firstDigit-5; i++) { // high digits
        blink(ledPinR,longue); // flashing longues
      }
    }
  }
  
  // display second digit via G LED
  if (secondDigit == 0) { // special case of 0
    for (i = 1; i <= 5; i++) { // flashing longue 5 times
      blink(ledPinG, longue);
    }
  }
  else {
    if (secondDigit <= 5) { // low digits
      for (i = 1; i <= secondDigit; i++) {
        blink(ledPinG,brieve); // flashing brieves
      }
    }
    else {
      for (i = 1; i <= secondDigit-5; i++) { // high digits
        blink(ledPinG,longue); // flashing longues
      }
    }
  }
  
  // check thresholds and react via B LED
  if (degreeC > tReading) { // checking if T was rounded up
    blink(ledPinB, pulse); // flashing pulse 1 time
    delay(pause);
  }
  if (tReading > tempFlag) { // checking if T threshold exceeded
    for (i = 1; i <= 2; i++) // flashing pulse 2 times
  blink(ledPinB, pulse);  
  delay(pause);
  }
  if (hReading > humFlag) { // checking if H threshold exceeded
    for (i = 1; i <= 3; i++) // flashing pulse 3 times
  blink(ledPinB, pulse);
  delay(pause);
  }
  
  // wait to start again
  delay(wait);
}

// debug and secondary support functions follow

void serialInit() {
  Serial.begin(115200);
  Serial.println("DHT TEST PROGRAM ");
  Serial.print("LIBRARY VERSION: ");
  Serial.println(DHT_LIB_VERSION);
  Serial.println();
  Serial.println("DHT\tH(%)\tT(C)");
}

void printValues(float hReading, float tReading){
  Serial.print(hReading,1);
  Serial.print("\t");
  Serial.println(tReading,1);
}

void DHTcheck() {
    dhtCheck = DHT.read11(dhtPin);
  switch (dhtCheck)
  {
    case DHTLIB_OK:  
                Serial.print("OK\t"); 
                break;
    case DHTLIB_ERROR_CHECKSUM: 
                Serial.print("Error \t"); 
                break;
    case DHTLIB_ERROR_TIMEOUT: 
                Serial.print("Timeout\t"); 
                break;
    default: 
                Serial.print("Unknown\t"); 
                break;
 }
}

