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

int t;


void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  
  t = 0;
}


void loop() {
  for (int i = 0; i < 120; i ++)
    strip.setPixelColor(i, Dark);
    
  if (t%500 < 100)
    for (int i = 119-t%3; i >= 0; i -= 3)
      strip.setPixelColor(i, Red);
  
  else if (t%500 < 200)
    for (int i = t%3; i < 120; i += 3)
      strip.setPixelColor(i, Green);
      
  else if (t%500 < 300) {
    for (int i = 119-t%3; i >= 60; i -= 3)
      strip.setPixelColor(i, Blue);
    for (int i = t%3; i < 60; i += 3)
      strip.setPixelColor(i, Blue);
  }
      
  else if (t%500 < 400) {
    for (int i = 59-t%3; i >= 0; i -= 3)
      strip.setPixelColor(i, Yellow);
    for (int i = 60+t%3; i < 120; i += 3)
      strip.setPixelColor(i, Yellow);
  }
  
  else if (t%500 < 500)
    for (int h = 0; h < 6; h ++) {
      if (h%2)
        for (int i = h*20+t%3; i < h*20+20; i += 3)
          strip.setPixelColor(i, White);
      else
        for (int i = h*20+19-t%3; i >= h*20; i -= 3)
          strip.setPixelColor(i, White);
    }
    
  strip.show();
  t ++;
  delay (20);
}
