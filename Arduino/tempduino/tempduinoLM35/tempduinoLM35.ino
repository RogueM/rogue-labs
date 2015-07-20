/*********************************************************************
Author: Rogue@HAL13
Version: .15720

Description: pseudo-morse code signalling of LM35 reading via LEDs
Digits are represented by brieves and longues according to their value
1-5 use brieves and 6-0 use longues. Inference is used to speed up signalling
for example 2(..___) is abridged '..'; 7(__...) is abridged '__'.
Includes support for thresholds and rounding indication.
*********************************************************************/

const float aVin = 5.0; // Voltage fed in the LM35 in Volts 
const float tempFlag = 25.0; // temp in C to flag if exceeded

int sReading=100;
float vReading;
float tReading;

const int pwrPin = A0; // power pin to simplify wiring
const int tmpPin = A1; // pin with LM35 attached
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
  // turn on power pin for LM35
  pinMode(pwrPin, OUTPUT);
  digitalWrite(pwrPin, HIGH);
  // initialize LED pins as output
  for (int ledUsed = 0; ledUsed < ledCount; ledUsed++)  {
    pinMode(ledPins[ledUsed], OUTPUT);      
    digitalWrite(ledUsed, LOW);
  }
  sReading=analogRead(tmpPin); // initial reading (will discard)
  delay(10000); //wait to allow circuit to settle
}

void blink(int ledUsed, int duration) {
  digitalWrite(ledUsed, HIGH);
  delay(duration);
  digitalWrite(ledUsed, LOW);
  delay(pause);
}

void loop() {
  int i; // variable for iterations

    sReading=analogRead(tmpPin);
    vReading=sReading*(aVin/1024.0); // 1024 steps but calculating a float so 1024.0!
    tReading=25+((vReading-0.25)*100); // calib 0.25v at 25C, linear 0.01V per degree C
    Serial.println(sReading);
    Serial.println(vReading);
    Serial.println(tReading);

  int degreeC = round(tReading); // rounding T reading into an integer
  int secondDigit = (int)degreeC % 10; // extract 2nd digit
  int firstDigit = ((int)degreeC-secondDigit)/10; // extract 1st digit
  // display values to serial
//  printValues(tReading);
  
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
    
  // wait to start again
  delay(wait);
}

// debug and secondary support functions follow

void serialInit() {
  Serial.begin(115200);
  Serial.println("LM35 TEST PROGRAM ");
  Serial.println();
  Serial.println("T(C)");
}

void printValues(float tReading){
  Serial.println(tReading,1);
}

