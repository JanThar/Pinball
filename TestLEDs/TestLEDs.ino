#include <FastLED.h>

// led numebr and pin
#define NUM_LEDS 42
#define DATA_PIN 2

int currentLED = NUM_LEDS-1;
bool noInput = true;

// Define the array of leds
CRGB leds[NUM_LEDS];

void setup() { 
    FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);  // GRB ordering is assumed
    Serial.begin(9600);
}

void loop() { 
  // Turn the current LED off
  leds[currentLED] = CRGB::Black;
  currentLED++; // next LED
  if (currentLED>=NUM_LEDS) currentLED=0; // check for out of bounds
  // Turn current LED on, then wait for input
  leds[currentLED] = CRGB::Red;
  FastLED.show();
  Serial.println(currentLED);
  //wait for user input
  while(noInput) {
    if (Serial.available()) {
      int inByte = Serial.read();
      noInput = false;
    }
  }
  noInput = true;
}
