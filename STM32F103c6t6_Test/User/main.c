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


u8 alarmFlag = 0;//�Ƿ񱨾��ı�־
u8 alarm_is_free = 10;//�������Ƿ��ֶ�������������ֶ�����������Ϊ0

u32 Height; //Һλ�߶�
u32 Flow;   //ˮ����

extern char oledBuf[20];

u8 Led_Status = 0;
u8 BEEP=0;
u8 Pump=0;
char PUB_BUF[256];//�ϴ����ݵ�buf
char devicd_id[]="qzw10086"; //�豸��
const char *devSubTopic[] = {"smartwater/sub"};
const char devPubTopic[] = "smartwater/pub";
u8 ESP8266_INIT_OK = 0;//esp8266��ʼ����ɱ�־

unsigned char snr, buf[16], TagType[2], SelectedSnr[4], DefaultKey[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
//extern void delay_init(void);

int main(void)
{

    unsigned short timeCount = 0;	//���ͼ������
    unsigned char *dataPtr = NULL;  //����ƽ̨����
    static u8 lineNow;
	unsigned char status;
	u8 num = 0;
	u8 KEY_A[6]= {0xff,0xff,0xff,0xff,0xff,0xff};
	u8 data[16] = {0};
	u8 UID[4]= {0x49, 0xe0, 0x05, 0x98};
   


    Usart1_Init(115200);//debug���ڣ��򴮿ڴ�ӡ����
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
	// RC522_data_break(); // ���������ݱ��Ƴ��򣬽���ѧϰ�ο�������Ƿ�ʹ��

	// UIDΪ��Ҫ�޸ĵĿ���UID key_type��0ΪKEYA����0ΪKEYB KEYΪ��Կ RW:1�Ƕ���0��д data_addrΪ�޸ĵĵ�ַ dataΪ��������
	//IC_RW ( UID, 0, KEY_A, 1, 0x10, data );
	PcdReset();
	PcdAntennaOff();
	PcdAntennaOn();
	M500PcdConfigISOType( 'A' );
	
	DEBUG_LOG("9.RC522ָʾ�Ƴ�ʼ��		[OK]");

    DEBUG_LOG("-------����Ӳ����ʼ�����--------");

    delay_ms(1000);
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
    while(1) {
		//RC522_Handle();
		
		// Ѱ��
		status= PcdRequest( REQ_ALL , TagType );    // REQ_ALL����Ѱ�����������п���TagTypeΪ���صĿ�����
		if(!status)
		{
            // ����ͻ
			status = PcdAnticoll(SelectedSnr);
			if(!status)
			{
                // ѡ��
				status=PcdSelect(SelectedSnr);
				if(!status)
				{
                    // ��֤
					snr = 1;    // ������1
					status = PcdAuthState(KEYA, (snr*4+3), DefaultKey, SelectedSnr);    // У��1�������룬����λ��ÿһ������3��
                                          // ��֤A��Կ�����ַ���������룬�����к�
					{
						if(!status)
						{
                            //��д��
							//status = PcdRead((snr*4+0), buf);   // ��������ȡ1����0�����ݵ�buf[0]-buf[16]
							buf[1]='a';
							buf[0]='b';
							buf[2]='c';
							status = PcdWrite((snr*4+0),buf);   // д������buf[0]-buf[16]д��1����0��
							if(!status) 
							{
                                //��д�ɹ�
                               
								DEBUG_LOG("read finish!");
								DEBUG_LOG("������ֵ��:%s",buf);
                                printf("������ֵ�ǣ� %s\n",buf);
								WaitCardOff();
							}
						}
					}
				}
			}
		}

       

#if 0
        if(timeCount % 40 == 0)//1000ms / 25 = 40 һ��ִ��һ��
        {

            /********** Һλ��������ȡ����**************/
            Height = 66;
            /********** ������������ȡ����**************/
            Flow = 66;
            /********** ��ȡˮ��״̬ **************/
            Pump = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7);
            /********** ��ȡ������״̬ **************/
            BEEP = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6);
            /********** ��ȡIC������ **************/

            /********** �����߼� **************/
            if(alarm_is_free == 10)//����������Ȩ�Ƿ���� alarm_is_free == 10 ��ʼֵΪ10
            {

            }
            if(alarm_is_free < 10)alarm_is_free++;

        }
        if(++timeCount >= 100)	// 5000ms / 25 = 200 ���ͼ��5000ms
        {
            DEBUG_LOG("�������� ----- OneNet_Publish");

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
