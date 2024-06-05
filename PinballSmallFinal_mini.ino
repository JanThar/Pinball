#include <FastLED.h>

#define BUTTON_COOLDOWN 500 //ms

boolean attackerOff = false;

// LED
#define NUM_LEDS 42
#define LED_PIN 10
#define BRIGHTNESS  255
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

// Button
int buttons[]    = {2,3,4,5,6,7,8,9,16};
int buttonsState[9];
long buttonsTime[9];

boolean pinChange = false;
int buttonState;

void ledOff(int selected) {
  leds[selected] = CRGB::Black;
}

void ledOn(int selected, CRGB color) {
  leds[selected] = color;
}

boolean evalButton(int selected) {
  if(!digitalRead(buttons[selected]) && millis() > buttonsTime[selected]+BUTTON_COOLDOWN) {
    buttonsTime[selected] = millis();
    //Serial.println(selected);
    return true;
  }
  return false;
}

void setup() {
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS );
  Serial.begin(9600);
  randomSeed(A3);
  Serial.println("Startup...");
  for(int i=0; i<9; i++) {
    buttonsState[i] =false;
    buttonsTime[i] = millis();
    pinMode(buttons[i],INPUT_PULLUP);
    evalButton(buttons[i]);
  }
  for(int i=0; i<NUM_LEDS; i++) {
    ledOff(i);
  }
  initSens();
  initComm();
  FastLED.show();
}

void loop() {
  runSens();
  runComm();

  FastLED.show();
}