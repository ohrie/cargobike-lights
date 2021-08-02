#include <FastLED.h>

#define LED_PIN     26
#define FRONT_NUM_LEDS    22
#define BRIGHTNESS  20
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB
#define FRONT_SIDE_NUM_LEDS 7
#define INDICATOR_RIGHT_PIN 22
#define INDICATOR_LEFT_PIN 2

CRGB leds[FRONT_NUM_LEDS];
CRGBPalette16 currentPalette;
TBlendType    currentBlending;

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;

const CRGB turn_indicator_color = CRGB::OrangeRed;

const int POSITION_LEFT_LEDS = FRONT_NUM_LEDS - FRONT_SIDE_NUM_LEDS;


void setup() {
    delay( 2000 ); // power-up safety delay
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, FRONT_NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness(  BRIGHTNESS );

    currentPalette = RainbowColors_p;
    currentBlending = LINEARBLEND;

    int buttonState = 0;
    pinMode(INDICATOR_LEFT_PIN, INPUT);
    pinMode(INDICATOR_RIGHT_PIN, INPUT);
    start_daylight();
}


void loop()
{
   int leftButton = digitalRead(INDICATOR_LEFT_PIN);
   int rightButton = digitalRead(INDICATOR_RIGHT_PIN);
   if(leftButton == HIGH){
    indicate(false, 5); 
   }
   else if(rightButton == HIGH){
    indicate(true, 5); 
   }
}

void indicate(bool right, int times){
  if(right == true){
    for(int i=0; i < times; i++){
      turn_indicator();
    }
  }
  if(right == false){
    for(int i=0; i < times; i++){
      turn_indicator_left();
    }
  }
  delay(200);
  reset_front_light();
}

void turn_indicator(){
  delay(300);
  // Always start with first 5 LEDs in state on
  for(int dot=FRONT_SIDE_NUM_LEDS-1; dot > FRONT_SIDE_NUM_LEDS/2; dot--) {
      leds[dot] = turn_indicator_color;
  }
  FastLED.show();

  // Then swoosh the rest on
  for(int dot=FRONT_SIDE_NUM_LEDS/2; dot >= 0; dot--) {
      leds[dot] = turn_indicator_color;
      FastLED.show();
      delay(30);
  }

  delay(300);
  reset(0, FRONT_SIDE_NUM_LEDS); 
  
}

void turn_indicator_left(){
    delay(300);
  // Always start with first half of LEDs in state on
  for(int dot=POSITION_LEFT_LEDS; dot < POSITION_LEFT_LEDS + FRONT_SIDE_NUM_LEDS / 2; dot++) {
      leds[dot] = turn_indicator_color;
  }
  FastLED.show();

  // Then swoosh the rest on
  for(int dot=POSITION_LEFT_LEDS + FRONT_SIDE_NUM_LEDS / 2; dot <= FRONT_NUM_LEDS; dot++) {
      leds[dot] = turn_indicator_color;
      FastLED.show();
      delay(30);
  }

  delay(300);
  reset(POSITION_LEFT_LEDS, FRONT_NUM_LEDS); 

}

void start_daylight() {
  int DAYLIGHT_BRIGHTNESS = 0;
  for(int dot=0; dot < FRONT_NUM_LEDS; dot++) {
      leds[dot] = CRGB::White;
  }
  FastLED.show();
  for(int sec=0; sec < BRIGHTNESS; sec++) {
    FastLED.setBrightness( DAYLIGHT_BRIGHTNESS++ );
    delay(10);
    FastLED.show();
  }
}

void start_system() {
  
}

void reset_front_light(){
  for(int dot=0; dot < FRONT_NUM_LEDS; dot++) {
      leds[dot] = CRGB::White;
  }
  FastLED.show();
}

// Resets all LEDs from start_position to end_position to black (off)
void reset(int start_position, int end_position){
  for(int dot=start_position; dot < end_position; dot++) {
      leds[dot] =  CRGB::Black;
  }
  FastLED.show();
}
