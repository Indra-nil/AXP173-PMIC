/**
 * @file AXP173_U8G2.ino
 * @author By mondraker (691806052@qq.com) (qq:735791683)
 * @brief  This is AXP173 PMU multi_Test
 * @version 0.1
 * @date 2022-09-28
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "AXP173.h"
#include "log.h"

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

// U8g2 constructor
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0,U8X8_PIN_NONE);
/* IIC pin */
// SCL  5   D1
// SDA  4   D2

#define U8LOG_WIDTH 30
#define U8LOG_HEIGHT 7

uint8_t u8log_buffer[U8LOG_WIDTH*U8LOG_HEIGHT];

//U8G2LOG u8g2log;
//AXP173 pmu;       //声明pmu对象
//I2C_PORT iic;   //声明iic对象

void setup() {

    /* Init Serial */ 
    Serial.begin(115200);     // 设置波特率为 115200

    /* Init PMU */
    Wire.begin();             //串口初始化
    pmu.begin(&Wire);         //AXP173 iic初始化(不包含ADC以及电源)
    //iic.I2C_dev_scan();     //扫描IIC设备地址

    /* Set PMU Voltage */
    setPmuPower();  //写在一切IIC设备初始化前面，电源芯片必须第一个初始化，并且在其他设备iic初始化之前设置好电压，否则其他设备程序初始化完结果没供电。

    /* Init Screen */
    u8g2.begin();  
    u8g2log.begin(U8LOG_WIDTH, U8LOG_HEIGHT, u8log_buffer);

    /* Set PMU Config */
    setPmuConfig();

    lprintf(LOG_INFO,"This is AXP173 PMU multi_Test\n");
    lprintf(LOG_INFO,"AXP173 Open Success\n");

}


void KeyPressIRQEvent(){
    if (pmu.powerState()) {
        if (pmu.getShortPressIRQState()) {      //获取对应位IRQ状态信息 true or false
            lprintf(LOG_INFO,"PEK Short Press\n");
            pmu.setShortPressIRQDisabale();     //对应位写1结束中断
            static bool LOCK_STATE = 1; //初始化屏幕状态
            if (LOCK_STATE) {   // = 1 睡眠
                // pmu.prepareToSleep();
                u8g2.setPowerSave(1);
                LOCK_STATE = 0;
                lprintf(LOG_INFO,"Screen Lock\n");
            }
            else {              // = 0 唤醒
                // pmu.RestoreFromLightSleep();
                u8g2.setPowerSave(0);
                LOCK_STATE = 1;
                lprintf(LOG_INFO,"Screen UnLock\n");
            } 
            /* 息屏时设置lightsleep或者deepsleep模式 */
        }
        else if (pmu.getLongPressIRQState()) {  //获取对应位IRQ状态信息 true or false
            lprintf(LOG_INFO,"PEK Long Press\n");
            pmu.setLongPressIRQDisabale();      //对应位写1结束中断
            lprintf(LOG_INFO,"Shut Down\n");
            pmu.powerOFF();                     //长按 2 秒关机
            /* 关机设置deepsleep模式待机或者直接关机 */
        }
    }
    else {
        lprintf(LOG_ERROR,"AXP173 ERROR!\n");
        delay(1000); 
    }
}


void loop(){

    printPmuInfo();

    screenPrint();

    //delay(100); 
    
    pmu.initKeyPressIRQ(LPRESS_2_5S);   //初始化按键中断事件 （设置长按键触发时间）
    KeyPressIRQEvent();                         //按键中断事件判断

}

void screenPrint() {    //屏幕绘制设置
    u8g2.firstPage();
    do {
        u8g2.drawFrame(0,0,u8g2.getDisplayWidth(),u8g2.getDisplayHeight()/4);
        u8g2.drawFrame(0,0,u8g2.getDisplayWidth(),u8g2.getDisplayHeight());
        u8g2.setFont(u8g2_font_6x13_tr);		// font for the title
        u8g2.setCursor(2, 13);			        // title position on the display
        u8g2.print("AXP173 Log Output");        // output title
        u8g2.setFont(u8g2_font_tom_thumb_4x6_mf);		// set the font for the terminal window
        u8g2.drawLog(2, 24, u8g2log);		    // draw the terminal window on the display
    } while ( u8g2.nextPage() );
}

void setPmuPower() {    //电源通道电压输出设置，交换位置可以设置上电时序，中间加delay可以延迟上电
    /* Enable and set LDO2 voltage */
    pmu.setOutputEnable(OP_LDO2, true);     //LDO2设置为输出
    pmu.setOutputVoltage(OP_LDO2, 3000);    //LDO2电压设置为3.000V

    /* Enable and set LDO3 voltage */
    pmu.setOutputEnable(OP_LDO3, true);     //LDO3设置为输出
    pmu.setOutputVoltage(OP_LDO3, 3300);    //LDO3电压设置为3.300V

    /* Enable and set LDO4 voltage */
    pmu.setOutputEnable(OP_LDO4, true);     //LDO4设置为输出
    pmu.setOutputVoltage(OP_LDO4, 3300);    //LDO4电压设置为3.300V

    /* Enable and set DCDC1 voltage */
    pmu.setOutputEnable(OP_DCDC1, true);    //DCDC1设置为输出
    pmu.setOutputVoltage(OP_DCDC1, 3300);   //DCDC1电压设置为3.300V

    /* Enable and set DCDC2 voltage */
    pmu.setOutputEnable(OP_DCDC2, true);    //DCDC2设置为输出
    pmu.setOutputVoltage(OP_DCDC2, 2275);   //DCDC2电压设置为2.275V

    /* Enable Battery Charging */
    pmu.setChargeEnable(true);                      //充电功能使能
    pmu.setChargeCurrent(CHG_360mA);        //设置充电电流为450mA
}

void setPmuConfig() {   //电源芯片ADC，库仑计等功能设置
    /* Set off time */
    pmu.setPowerOffTime(POWEROFF_4S);       //设置关机后所有电源通道关闭时长为4S

    /* Set on time */
    pmu.setPowerOnTime(POWERON_128mS);      //设置开机后电源通道启动输出时长为128mS

    /* Enable VBUS ADC */
    pmu.setADCEnable(ADC_VBUS_V, true);     //VBUS ADC 电压使能
    pmu.setADCEnable(ADC_VBUS_C, true);     //VBUS ADC 电流使能

    /* Enable Battery ADC */
    pmu.setADCEnable(ADC_BAT_V, true);      //Battery ADC 电压使能
    pmu.setADCEnable(ADC_BAT_C, true);      //Battery ADC 电流使能

    /* Enable Coulometer and set COULOMETER_ENABLE*/
    pmu.setCoulometer(COULOMETER_ENABLE, true); //库仑计使能

    lprintf(LOG_INFO,"AXP173 Set OK\n");
}

void printPmuInfo() {   //需要打印在屏幕上的芯片信息

    /* Get PMU temp info */
    lprintf(LOG_INFO,"CoreTemp :%.2f 'C\n", pmu.getAXP173Temp());                //芯片温度

    /* Get VBUS info */
    // lprintf(LOG_INFO,"VBUS_voltage :%.2f V\n", pmu.getVBUSVoltage());         //VBUS输入电压
    // lprintf(LOG_INFO,"VBUS_current :%.2f mA\n", pmu.getVBUSCurrent());        //VBUS输入电流

    if(pmu.isBatExist()){
        //lprintf(LOG_INFO,"Battery :Battery Exist\n");                         //电池接入状态
        /* Get Battery info */
        pmu.isCharging() ? lprintf(LOG_INFO,"Charging :Is Charging s\n") : lprintf(LOG_INFO,"Charging :NO or END Charging\n");//充电状态
        lprintf(LOG_INFO,"Bat_voltage :%.2f V\n", pmu.getBatVoltage());           //电池电压
        lprintf(LOG_INFO,"Bat_Current :%.2f mA\n", pmu.getBatCurrent());          //电池电流  正为充电，负为放电
        
        //lprintf(LOG_INFO,"Bat_Level :%.2f %%\n", pmu.getBatLevel());            //电池电量百分比显示
        //lprintf(LOG_INFO,"Bat_BatPower :%.2f W", pmu.getBatPower());            //电池瞬时功率

        lprintf(LOG_INFO,"GetBatCoulombInput :%.2f C\n", pmu.GetBatCoulombInput());   //Get Coulomb charge Data
        lprintf(LOG_INFO,"GetBatCoulombOutput :%.2f C\n", pmu.GetBatCoulombOutput());       //Get Coulomb Discharge Data
        lprintf(LOG_INFO,"CoulometerData :%.2f C\n", pmu.getCoulometerData());        //get coulomb val affter calculation
    }
    else{
        lprintf(LOG_INFO,"Battery :NO Battery\n");                            //没电池就输出这
    }

}
