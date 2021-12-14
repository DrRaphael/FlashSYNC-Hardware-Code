# FlashSYNC API文档
> 严格意义上并不是一个软件，更偏向于一个通信协议，用来协商包括WS2812在内的一些的可寻址RGB灯条通过转接芯片实现和上位机通信的一种通信协议。


>目前 FlashSYNC正处于0.1内部测试版本，所用协议并没有完全定性，甚至某些API尚未实现，请各位基于此开发时请勿使用硬编码
***

### 默认参数
> 测试芯片：<kbd>ESP32-PICO-D4</kbd>@<kbd>ESP32-PCIO-KIT</kbd>

> 开发环境:<kbd>Clion @2021.3</kbd> +<kbd>PlatformIO</kbd> 

> 开发框架：<kbd>Arduino</kbd>

> 开发库：<kbd>Adafruit NeoPixel@^1.10.1</kbd>

> 串波特率：<kbd>115200</kbd>

> 默认输出引脚（控制彩灯的引脚）:<kbd>IO4</kbd>


### 通信协议
每个彩灯由3个字节控制（R,G,B）【为节省资源、降低开发难度,每通道取值为0~200，WS2812支持0-255】