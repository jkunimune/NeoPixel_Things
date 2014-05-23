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

unsigned int sample;


void setup() {
  Serial.begin(9600);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  
  x = 12;
  v = 0;
}


void loop() {
  int s = Sound();
  
  for (int i = 0; i < 120; i ++)
    strip.setPixelColor(i, Red);
  for (int i = 40; i < 80; i ++)
    strip.setPixelColor(i, Yellow);
  for (int i = 54; i < 66; i ++)
    strip.setPixelColor(i, Green);
  
  if (x > 12)
    v = v-.0294;
  if (x <=12 && v < 0)
    v = abs(v*.7);
  if (x >= 110 && v > 0)
    v = -abs(v*.7);
  v = v + s/500;
  
  if (x > 12 || v > .1)
    x = x+v;
  
  if (x > 12 && x < 108 || v > .1)
    x = x+v;

  for (int i = -1; i <= 1; i ++)
    strip.setPixelColor(x+i, Blue);
  strip.show();
}


int Sound() {
  unsigned long startMillis= millis();  // Start of sample window
   unsigned int peakToPeak = 0;   // peak-to-peak level
 
   unsigned int signalMax = 0;
   unsigned int signalMin = 1024;
 
   // collect data for 10 mS
   while (millis() - startMillis < 10)
   {
      sample = analogRead(0);
      if (sample < 1024)  // toss out spurious readings
      {
         if (sample > signalMax)
         {
            signalMax = sample;  // save just the max levels
         }
         else if (sample < signalMin)
         {
            signalMin = sample;  // save just the min levels
         }
      }
   }
   peakToPeak = signalMax - signalMin;  // max - min = peak-peak amplitude
   
   return peakToPeak;
}
