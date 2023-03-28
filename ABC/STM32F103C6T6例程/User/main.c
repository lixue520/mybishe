#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "rc522.h"
#include "pump.h"
#include "beep.h"
#include "timer.h"
#include "exti.h"
#include "stdio.h"
#include "esp8266.h"
#include "onenet.h"
#include "led.h"
#include "pay.h"

/**
*   连线说明：
*   1--SDA  <----->PA4
*   2--SCK  <----->PA5
*   3--MOSI <----->PA7
*   4--MISO <----->PA6
*   5--悬空
*   6--GND <----->GND
*   7--RST <----->PB0
*   8--VCC <----->VCC
**/

u8 alarmFlag = 0;//是否报警的标志
u8 alarm_is_free = 10;//报警器是否被手动操作，如果被手动操作即设置为0

u32 Height; //液位高度
u32 Flow;   //水流量
u8 Led_Status = 0;
u8 BEEP=0;
u8 Pump=0;
char PUB_BUF[256];//上传数据的buf
char devicd_id[]="qzw10086"; //设备号
const char *devSubTopic[] = {"smartwater/sub"};
const char devPubTopic[] = "smartwater/pub";
u8 ESP8266_INIT_OK = 0;//esp8266初始化完成标志
//unsigned char snr, buf[16], TagType[2], SelectedSnr[4], DefaultKey[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
//extern void delay_init(void);

extern void PcdAntennaOn(void);
extern void PcdAntennaOff(void);


int main(void)
{
    unsigned short timeCount = 0;	//发送间隔变量
    unsigned char *dataPtr = NULL;  //接收平台数据
    static u8 lineNow;

//    u8 num = 0;
//    u8 KEY_A[6]= {0xff,0xff,0xff,0xff,0xff,0xff};
//    u8 data[16] = {0};
//    u8 UID[4]= {0x49, 0xe0, 0x05, 0x98};
//    char status;
//    unsigned char snr, buf[16], TagType[2], SelectedSnr[4], DefaultKey[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

    delay_init();	    	 //延时函数初始化
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
    uart_init(115200);	 	//串口初始化为115200(调试输出)
    DEBUG_LOG("\r\n");
    DEBUG_LOG("1.UART1初始化			[OK]");

    delay_init();	    	 //延时函数初始化
    DEBUG_LOG("2.延时函数初始化			[OK]");

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// 设置中断优先级分组2
    DEBUG_LOG("3.中断优先初始化			[OK]");

    EXTIX_Init();		//外部中断初始化
    DEBUG_LOG("4.外部中断初始化			[OK]");

    BEEP_Init();
    DEBUG_LOG("5.蜂鸣报警器初始化			[OK]");

    Pump_Init();
    DEBUG_LOG("6.水泵控制器初始化			[OK]");

    Usart2_Init(115200);//stm32-8266通讯串口
    DEBUG_LOG("7.UART2初始化			[OK]");

    LED_Init();
    DEBUG_LOG("8.LED指示灯初始化		[OK]");

    RC522_Init();       //初始化射频卡模块
    delay_ms(1000);
    // UID为你要修改的卡的UID key_type：0为KEYA，非0为KEYB KEY为密钥 RW:1是读，0是写 data_addr为修改的地址 data为数据内容
    //IC_RW ( UID, 0, KEY_A, 1, 0x10, data );
    PcdReset();
    PcdAntennaOff();
    PcdAntennaOn();
    M500PcdConfigISOType( 'A' );
    DEBUG_LOG("9.RC522指示灯初始化		[OK]");

    DEBUG_LOG("-------基础硬件初始化完成--------");

#if 0
    DEBUG_LOG("初始化ESP8266 WIFI模块...");
    //if(!ESP8266_INIT_OK) {}
    ESP8266_Init();					//初始化ESP8266
    while(OneNet_DevLink()) { //接入OneNET
        delay_ms(500);
    }
#endif

//    TIM2_Int_Init(4999,7199);
    TIM3_Int_Init(2499,7199);
    DEBUG_LOG("初始化定时器3...");
#if 0
    OneNet_Subscribe(devSubTopic, 1);//订阅服务器话题管道
#endif


    while(1)
    {

		//PcdReset();
		test_read_write();
		//RC522_Handle();
        

    }
}
