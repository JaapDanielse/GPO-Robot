
/*
  RobotLed
  Demo program for WS2812 NeoPixel Led's.

  V0.1  05-11-2016 JD
        Original Version
 
*/


#include <Adafruit_NeoPixel.h>

#define NEOPIXPIN  10 // pin 10 - motor shield servo 1 
#define NEOPIXCNT  1 // just one neopixle

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NEOPIXCNT, NEOPIXPIN, NEO_GRB + NEO_KHZ800);

void setup() 
{

pixels.begin(); // This initializes the NeoPixel library.

}

void loop() 
{
  #define STEPDELAY 8 // delay between steps. 
  
  for (int i=0; i < 255; i++) // cycle from red to green
  { 
    pixels.setPixelColor(0,pixels.Color(255-i,i,0)); // set color (R,G,B)
    pixels.show(); // This sends the updated pixel color to the hardware.
    delay(STEPDELAY); // delay between steps. 
  }

  for (int i=0; i < 255; i++) // cycle from green to blue
  { 
    pixels.setPixelColor(0,pixels.Color(0,255-i,i)); // set color (R,G,B)
    pixels.show(); // This sends the updated pixel color to the hardware.
    delay(STEPDELAY); // delay between steps. 
  }

  for (int i=0; i < 255; i++) // Cycle from blue to red
  { 
    pixels.setPixelColor(0,pixels.Color(i,0,255-i)); // set color (R,G,B)
    pixels.show(); // This sends the updated pixel color to the hardware.
    delay(STEPDELAY); // delay between steps. 
  }
}
