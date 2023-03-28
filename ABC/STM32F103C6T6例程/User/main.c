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
*   ����˵����
*   1--SDA  <----->PA4
*   2--SCK  <----->PA5
*   3--MOSI <----->PA7
*   4--MISO <----->PA6
*   5--����
*   6--GND <----->GND
*   7--RST <----->PB0
*   8--VCC <----->VCC
**/

u8 alarmFlag = 0;//�Ƿ񱨾��ı�־
u8 alarm_is_free = 10;//�������Ƿ��ֶ�������������ֶ�����������Ϊ0

u32 Height; //Һλ�߶�
u32 Flow;   //ˮ����
u8 Led_Status = 0;
u8 BEEP=0;
u8 Pump=0;
char PUB_BUF[256];//�ϴ����ݵ�buf
char devicd_id[]="qzw10086"; //�豸��
const char *devSubTopic[] = {"smartwater/sub"};
const char devPubTopic[] = "smartwater/pub";
u8 ESP8266_INIT_OK = 0;//esp8266��ʼ����ɱ�־
//unsigned char snr, buf[16], TagType[2], SelectedSnr[4], DefaultKey[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
//extern void delay_init(void);

extern void PcdAntennaOn(void);
extern void PcdAntennaOff(void);


int main(void)
{
    unsigned short timeCount = 0;	//���ͼ������
    unsigned char *dataPtr = NULL;  //����ƽ̨����
    static u8 lineNow;

//    u8 num = 0;
//    u8 KEY_A[6]= {0xff,0xff,0xff,0xff,0xff,0xff};
//    u8 data[16] = {0};
//    u8 UID[4]= {0x49, 0xe0, 0x05, 0x98};
//    char status;
//    unsigned char snr, buf[16], TagType[2], SelectedSnr[4], DefaultKey[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

    delay_init();	    	 //��ʱ������ʼ��
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
    uart_init(115200);	 	//���ڳ�ʼ��Ϊ115200(�������)
    DEBUG_LOG("\r\n");
    DEBUG_LOG("1.UART1��ʼ��			[OK]");

    delay_init();	    	 //��ʱ������ʼ��
    DEBUG_LOG("2.��ʱ������ʼ��			[OK]");

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// �����ж����ȼ�����2
    DEBUG_LOG("3.�ж����ȳ�ʼ��			[OK]");

    EXTIX_Init();		//�ⲿ�жϳ�ʼ��
    DEBUG_LOG("4.�ⲿ�жϳ�ʼ��			[OK]");

    BEEP_Init();
    DEBUG_LOG("5.������������ʼ��			[OK]");

    Pump_Init();
    DEBUG_LOG("6.ˮ�ÿ�������ʼ��			[OK]");

    Usart2_Init(115200);//stm32-8266ͨѶ����
    DEBUG_LOG("7.UART2��ʼ��			[OK]");

    LED_Init();
    DEBUG_LOG("8.LEDָʾ�Ƴ�ʼ��		[OK]");

    RC522_Init();       //��ʼ����Ƶ��ģ��
    delay_ms(1000);
    // UIDΪ��Ҫ�޸ĵĿ���UID key_type��0ΪKEYA����0ΪKEYB KEYΪ��Կ RW:1�Ƕ���0��д data_addrΪ�޸ĵĵ�ַ dataΪ��������
    //IC_RW ( UID, 0, KEY_A, 1, 0x10, data );
    PcdReset();
    PcdAntennaOff();
    PcdAntennaOn();
    M500PcdConfigISOType( 'A' );
    DEBUG_LOG("9.RC522ָʾ�Ƴ�ʼ��		[OK]");

    DEBUG_LOG("-------����Ӳ����ʼ�����--------");

#if 0
    DEBUG_LOG("��ʼ��ESP8266 WIFIģ��...");
    //if(!ESP8266_INIT_OK) {}
    ESP8266_Init();					//��ʼ��ESP8266
    while(OneNet_DevLink()) { //����OneNET
        delay_ms(500);
    }
#endif

//    TIM2_Int_Init(4999,7199);
    TIM3_Int_Init(2499,7199);
    DEBUG_LOG("��ʼ����ʱ��3...");
#if 0
    OneNet_Subscribe(devSubTopic, 1);//���ķ���������ܵ�
#endif


    while(1)
    {

		//PcdReset();
		test_read_write();
		//RC522_Handle();
        

    }
}
