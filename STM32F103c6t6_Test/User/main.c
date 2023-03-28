#include "stm32f10x.h"                  // Device header
#include "pump.h"
#include "beep.h"
#include "delay.h"
#include "timer.h"
#include "usart.h"
#include "exti.h"
#include "stdio.h"
#include "esp8266.h"
#include "onenet.h"
#include "led.h"
#include "rc522.h"


u8 alarmFlag = 0;//是否报警的标志
u8 alarm_is_free = 10;//报警器是否被手动操作，如果被手动操作即设置为0

u32 Height; //液位高度
u32 Flow;   //水流量

extern char oledBuf[20];

u8 Led_Status = 0;
u8 BEEP=0;
u8 Pump=0;
char PUB_BUF[256];//上传数据的buf
char devicd_id[]="qzw10086"; //设备号
const char *devSubTopic[] = {"smartwater/sub"};
const char devPubTopic[] = "smartwater/pub";
u8 ESP8266_INIT_OK = 0;//esp8266初始化完成标志

unsigned char snr, buf[16], TagType[2], SelectedSnr[4], DefaultKey[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
//extern void delay_init(void);

int main(void)
{

    unsigned short timeCount = 0;	//发送间隔变量
    unsigned char *dataPtr = NULL;  //接收平台数据
    static u8 lineNow;
	unsigned char status;
	u8 num = 0;
	u8 KEY_A[6]= {0xff,0xff,0xff,0xff,0xff,0xff};
	u8 data[16] = {0};
	u8 UID[4]= {0x49, 0xe0, 0x05, 0x98};
   


    Usart1_Init(115200);//debug串口，向串口打印数据
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
	// RC522_data_break(); // 测试用数据爆破程序，仅供学习参考，请勿非法使用

	// UID为你要修改的卡的UID key_type：0为KEYA，非0为KEYB KEY为密钥 RW:1是读，0是写 data_addr为修改的地址 data为数据内容
	//IC_RW ( UID, 0, KEY_A, 1, 0x10, data );
	PcdReset();
	PcdAntennaOff();
	PcdAntennaOn();
	M500PcdConfigISOType( 'A' );
	
	DEBUG_LOG("9.RC522指示灯初始化		[OK]");

    DEBUG_LOG("-------基础硬件初始化完成--------");

    delay_ms(1000);
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
    while(1) {
		//RC522_Handle();
		
		// 寻卡
		status= PcdRequest( REQ_ALL , TagType );    // REQ_ALL代表寻天线区内所有卡。TagType为返回的卡类型
		if(!status)
		{
            // 防冲突
			status = PcdAnticoll(SelectedSnr);
			if(!status)
			{
                // 选卡
				status=PcdSelect(SelectedSnr);
				if(!status)
				{
                    // 认证
					snr = 1;    // 扇区号1
					status = PcdAuthState(KEYA, (snr*4+3), DefaultKey, SelectedSnr);    // 校验1扇区密码，密码位于每一扇区第3块
                                          // 验证A密钥，块地址，扇区密码，卡序列号
					{
						if(!status)
						{
                            //读写卡
							//status = PcdRead((snr*4+0), buf);   // 读卡，读取1扇区0块数据到buf[0]-buf[16]
							buf[1]='a';
							buf[0]='b';
							buf[2]='c';
							status = PcdWrite((snr*4+0),buf);   // 写卡，将buf[0]-buf[16]写入1扇区0块
							if(!status) 
							{
                                //读写成功
                               
								DEBUG_LOG("read finish!");
								DEBUG_LOG("读到的值是:%s",buf);
                                printf("读到的值是： %s\n",buf);
								WaitCardOff();
							}
						}
					}
				}
			}
		}

       

#if 0
        if(timeCount % 40 == 0)//1000ms / 25 = 40 一秒执行一次
        {

            /********** 液位传感器获取数据**************/
            Height = 66;
            /********** 流量传感器获取数据**************/
            Flow = 66;
            /********** 读取水泵状态 **************/
            Pump = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7);
            /********** 读取蜂鸣器状态 **************/
            BEEP = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6);
            /********** 获取IC卡数据 **************/

            /********** 报警逻辑 **************/
            if(alarm_is_free == 10)//报警器控制权是否空闲 alarm_is_free == 10 初始值为10
            {

            }
            if(alarm_is_free < 10)alarm_is_free++;

        }
        if(++timeCount >= 100)	// 5000ms / 25 = 200 发送间隔5000ms
        {
            DEBUG_LOG("发布数据 ----- OneNet_Publish");

            sprintf(PUB_BUF,"{\"Devicd\":\"%s\",\"Height\":%d,\"flow\":%d,\"Beep\":%d,\"Pump\":%d}",
                    devicd_id,Height,Flow,BEEP,Pump);
            OneNet_Publish(devPubTopic, PUB_BUF);
            DEBUG_LOG("==================================================================================");
            timeCount = 0;
            ESP8266_Clear();
        }

        dataPtr = ESP8266_GetIPD(3);
        if(dataPtr != NULL)
            OneNet_RevPro(dataPtr);
        delay_ms(10);
#endif
    }

}
