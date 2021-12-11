#include "ATCommand.h"



void fullOpen(Adafruit_NeoPixel pixels,uint32_t Color)
{
    uint16_t NumLED = pixels.numPixels();
    for (int i = 0; i < NumLED; i++)
    {
        pixels.setPixelColor(i,Color);
    }
    pixels.show();
    delay(100);
}
