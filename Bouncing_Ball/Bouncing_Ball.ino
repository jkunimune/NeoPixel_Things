#include <Adafruit_NeoPixel.h>

#define PIN 6

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(120, PIN, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

const uint32_t Red = strip.Color(255, 0, 0);
const uint32_t Yellow = strip.Color(255, 255, 0);
const uint32_t Green = strip.Color(0, 255, 0);
const uint32_t Blue = strip.Color(0, 0, 255);
const uint32_t White = strip.Color(255, 255, 255);
const uint32_t Dark = strip.Color(0, 0, 0);

float x;
float v;


void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  
  x = 109.9;
  v = 0;
}


void loop() {
  for (int i = 0; i < 120; i ++)
    strip.setPixelColor(i, Dark);
  v = v-.015;
  if (x < 13) {
    v = -v;
    v = v-.017;
  }
  x = x+v;
  for (int i = -1; i <= 1; i ++)
    strip.setPixelColor(x+i, Yellow);
  for (int i = 0; i < 13; i ++)
    strip.setPixelColor(i, Dark);
  strip.show();
  delay(7);
}
