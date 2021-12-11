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
#define INDEX_CONNT       0xFC
#define INDEX_SETBR       0xFB
#define INDEX_RESET       0xFA
#define BANDRATE          115200
/*
 * 设置彩灯阵列
 * */
Adafruit_NeoPixel pixels(LED_NUM,
                         LED_PIN,
                         NEO_GRB+NEO_KHZ800
                         );
void LEDtest(void);
void FillColor(void);

void WaitForConnect(void);



void setup()
{
    Serial.begin(BANDRATE);                       //设置波特率
    pixels.begin();                                     //启动像素灯
    pixels.clear();                                     //清空
    pixels.setBrightness(20);                           //设置初始亮度
    WaitForConnect();

//    LEDtest();                                          //测试LED灯
}

uint16_t LPoint = 0;             //LED灯指针
uint8_t  CPoint = 0;             //颜色指针

uint8_t LED_RGB[LED_NUM][3];      //LED数据数组，用来存储LED数据

void fullFill(uint8_t R, uint8_t G, uint8_t B);

void loop()                                         //指令解析
{
    if(Serial.available())
    {
        uint8_t colorTemp = Serial.read();

        if(colorTemp <= 200)                    //填充数据---同步指令
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
        } else if (colorTemp==INDEX_ENTER)      //解析确定指令---使填充数据生效
        {
            LPoint = 0;
            CPoint = 0;
            FillColor();
        } else if(colorTemp == INDEX_CLEAR)     //解析清空指令---立即执行
        {
            fullFill(0,0,0);
            pixels.show();
        }
        else if(colorTemp == INDEX_DELAY)       //解析延时指令---双字节指令
        {
            while (!Serial.available());
            uint8_t temp = Serial.read();
            Serial.write(temp);
            delay(temp*10);
        }
        else if(colorTemp==INDEX_CONNT)
        {
            WaitForConnect();
        }
    }

}

void LEDtest(void)
{
    uint16_t NumLED = pixels.numPixels();
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






/*
 * 连接函数，用于串口和上位机建立连接
 * */
void WaitForConnect(void)
{
    while (1)
    {
        if(Serial.available())
        {
            uint8_t temp = Serial.read();
            if(temp==INDEX_CONNT)
            {
                Serial.write(INDEX_CONNT);
                delay(10);
                while (Serial.read()>=0);
                break;
            }
        }
    }
}