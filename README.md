## Three data cables & 1.6 yuan & ultra-small PCB layout & ultra-low power consumption & double-layer board implementation.You can let your device get the general power management of a mobile phone.


###2022.12.8 update：
Overview: The menu function has been added, and the buttons and battery information can be switched through the menu!！！！

1.This update introduces the onebutton library, which supports key-click, double-click and long-press functions.Now the power chip PEK has realized 2s (adjustable) power-on, short press the information screen (customizable function), long press 1.5s (adjustable), similar to the voice assistant of a mobile phone to exhale (customizable function), continue to long press 6s (adjustable) to turn off, currently achieved by detecting IRQ, PEK will be added to onebutton management later, to achieve a double-click (adjustable) camera function similar to a mobile phone, and so on.At present, I intend to conceive a power supply power calculation algorithm, through the coulomb meter and battery voltage speculation, the fusion of battery temperature, etc. to accurately predict the estimated charging time and shutdown time of the battery power, etc., But there is relatively little information (no), most of them are mobile phone business secrets, so if you have a favorite friend or big brother, you can share the research with me, thank you very much!！！




2.The code update is basically completed on GitHub, and the main hardware updates are here!！！




3.Please note: If you are an Arduino development board user, when you encounter a serious error, it is because Arduino does not support "printf", please replace all with "print" or "println"!！！




4.ArduinoUNO/nano/micro/mini, etc. cannot use U8G2LOG due to memory size, so this update is not available unless the external Flash is extended. You can only use simpleDemo!！！(simpleDemo does not use U8G2, supports small memory devices)


## The following is the text：
1.The register reading and writing method of this library mainly draws on the AXP192 library on the official website of m5stack.The IIC_PORT library is from Forairaaaaa.The universal version of the chip is thanks to liuzewen for guiding the way, otherwise the default output is different due to the customized characteristics of the chip, and the IIC cannot communicate.


2.Annotated all the functions by yourself!！！And wrote other functions that I needed.For example, by identifying the IRQ interrupt pin and interrupt register, set the long press to shut down and the short press to the information screen, etc., you can also set which outputs to turn off after the information screen, and so on.


3.You can use it for: charging management of Bluetooth keyboard, charging management of development board, charging management of micro watch, charging management of small TV... and so on and so on and so on and so on and so on and so on and so on and so on and so on and so on and so on and so on and so on and so on and so on and so on


4.How to use: See the wiring diagram and the video demonstration at the bottom (please refer to the program and data sheet first, if there are too many people, you will take time to make a video tutorial)


5.Other: The documentation will continue to be improved in the future, and I have been busy recently.If you don't know how, you can come to the QQ group first to ask: 735791683


### arduino, esp8266, esp32, etc. Do not need to be ported directly, they can be used directly.It can be directly packaged into a file folder and placed in the lib library of the Arduino IDE (see the introduction to Open source for specific operations).A keyword file has been added, and functions and keywords are used to support code highlighting.


#### Basic functions of the chip (basic functions for squeezing out common functions)

![Function] (https://image.lceda.cn/pullimage/kahHHHXHiTKpMsMp9WOTKofT0jKPpmMOKs7hHRJN.png )

#### For details, see:https://oshwhub.com/mondraker/axp173

#### The following is the wiring diagram (not too detailed here, wait for a single block diagram after a while, remember that not only the common ground but also the MCU must be powered by the power supply chip, otherwise it will not be able to run, the USBttl circuit should be designed in front of the power supply chip, otherwise the USB chip will power the MCU and cause the current to pour down, and the serial port monitoring data fails or is not allowed!！！）

! [Specific wiring diagram] (https://image.lceda.cn/pullimage/0UmiCtTcMNNo9QroSF73lCTjk80fvxGcqTW8h9C7.png )

#### A picture

![Chip] (https://image.lceda.cn/pullimage/HB7w1x4u9ayl66i4vpHSZOuA4biLDCnlTWlBK2qN.jpeg )
