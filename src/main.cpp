#include <Arduino.h>
#include "Adafruit_NeoPixel.h"
#include "ATCommand.h"
#define TEST_DELAY  10
#define LED_PIN     4
#define LED_NUM     12


/*
 *  超级指令表
 *  类型              指令               HEX        功能
 *  单字节指令       INDEX_ENTER         0xFF      执行刷新
 *  单字节指令       CLEAR               0xFE      关闭所有灯
 *  双字节指令       INDEX_DELAY         0xFD      延时指令，范围0-0xFF（单位：10mS）
 * */


#define INDEX_ENTER       0xFF
#define INDEX_CLEAR       0xFE
#define INDEX_DELAY       0xFD

#define INDEX_SETBR       0xFB

/*
 * 设置彩灯阵列
 * */
Adafruit_NeoPixel pixels(LED_NUM,
                         LED_PIN,
                         NEO_GRB+NEO_KHZ800
                         );
void LEDtest(void);
void FillColor(void);
void setup()
{
    Serial.begin(512000);
    Serial.write(0xAA);
    pixels.begin();
    pixels.clear();
    pixels.setBrightness(20);
    pixels.setPixelColor(0,0x00);
    pixels.show();
    LEDtest();

}

uint16_t LPoint = 0;
uint8_t  CPoint = 0;
uint8_t LED_Color[3];

uint8_t LED_RGB[LED_NUM][3];        //LED数据数组，用来存储LED数据

void fullFill(uint8_t R, uint8_t G, uint8_t B);


void loop()
{
    if(Serial.available())
    {
        uint8_t colorTemp = Serial.read();
        Serial.write(colorTemp);
        if(colorTemp <= 200)
        {
            LED_RGB[LPoint][CPoint++] = colorTemp;
            if(CPoint==3)
            {
                CPoint=0;
                LPoint++;
                if(LPoint==pixels.numPixels())
                {
                    LPoint=0;
                    FillColor();
                }
            }
        } else if (colorTemp==INDEX_ENTER)
        {
            LPoint = 0;
            CPoint = 0;
            FillColor();
        } else if(colorTemp == INDEX_CLEAR)
        {
            fullFill(0,0,0);
            pixels.show();
        }
        else if(colorTemp == INDEX_DELAY)
        {
            while (!Serial.available());
            uint8_t temp = Serial.read();
            Serial.write(temp);
            delay(temp*10);
        }
    }

}

void LEDtest(void)
{
    uint16_t NumLED = pixels.numPixels();
    //库的设计上最多支持100个灯，但是理论上灯的数量是无限制的
    //为了兼顾兼容性和内存占用，使用16位存储
    //65536及以后的灯会被忽略
    for (int i = 0; i < NumLED; ++i)
    {
        pixels.setPixelColor(i,0x00);
        pixels.show();
    }
    delay(TEST_DELAY);
    for (int i = 0; i < NumLED; ++i)
    {
        pixels.setPixelColor(i,0xFF);
        pixels.show();
    }
    delay(TEST_DELAY);
    for (int i = 0; i < NumLED; ++i)
    {
        pixels.setPixelColor(i,0xFF00);
        pixels.show();
    }
    delay(TEST_DELAY);
    for (int i = 0; i < NumLED; ++i)
    {
        pixels.setPixelColor(i,0xFF0000);
        pixels.show();
    }
    delay(TEST_DELAY);

    for (int i = 0; i < NumLED; ++i)
    {
        pixels.setPixelColor(i,0x00);
        pixels.show();
    }
}

void FillColor(void)
{
    for (int i = 0; i < pixels.numPixels(); ++i)
    {
        pixels.setPixelColor(i,
                             LED_RGB[i][0],
                             LED_RGB[i][1],
                             LED_RGB[i][2]
                             );
//        LED_RGB[i][0] = 0;
//        LED_RGB[i][1] = 0;
//        LED_RGB[i][2] = 0;
    }
    pixels.show();
}



void fullFill(uint8_t R, uint8_t G, uint8_t B)
{
    uint16_t NumLED = pixels.numPixels();
    for (int i = 0; i < NumLED; ++i)
    {
        pixels.setPixelColor(i,R,G,B);
    }
    pixels.show();
}