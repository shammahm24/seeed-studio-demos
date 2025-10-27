#include <Adafruit_NeoPixel.h>

#define PIN        D10    // Connect to D0 on LED ring
#define NUMPIXELS  16

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  pixels.begin();
  pixels.show(); // Turn all LEDs off
}

void loop() {
  for(int i=0; i<NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(255, 0, 0)); // Red
    pixels.show();
    delay(100);
    pixels.setPixelColor(i, pixels.Color(0, 0, 0));   // Off
  }
}
