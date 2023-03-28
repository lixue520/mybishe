#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "rc522.h"
#include "led.h"

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

int main(void)
{
    u8 num = 0;
	u8 KEY_A[6]= {0xff,0xff,0xff,0xff,0xff,0xff};
	u8 data[16] = {0};
	u8 UID[4]= {0x49, 0xe0, 0x05, 0x98};
	 char status;
	unsigned char snr, buf[16], TagType[2], SelectedSnr[4], DefaultKey[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

    delay_init();	    	 //��ʱ������ʼ��
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
    uart_init(115200);	 	//���ڳ�ʼ��Ϊ115200
    LED_Init();
    RC522_Init();       //��ʼ����Ƶ��ģ��
	
	delay_ms(3000);
	// RC522_data_break(); // ���������ݱ��Ƴ��򣬽���ѧϰ�ο�������Ƿ�ʹ��

	// UIDΪ��Ҫ�޸ĵĿ���UID key_type��0ΪKEYA����0ΪKEYB KEYΪ��Կ RW:1�Ƕ���0��д data_addrΪ�޸ĵĵ�ַ dataΪ��������
	IC_RW ( UID, 0, KEY_A, 1, 0x10, data );
	PcdReset();
	PcdAntennaOff();
	PcdAntennaOn();
	M500PcdConfigISOType( 'A' );
	

	
    while(1)
    {
		
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
							status = PcdRead((snr*4+0), buf);   // ��������ȡ1����0�����ݵ�buf[0]-buf[16]
							buf[0]='a';
							buf[1]='b';
							buf[2]='c';
							status = PcdWrite((snr*4+0), buf);   // д������buf[0]-buf[16]д��1����0��
							if(!status) 
							{
                                //��д�ɹ�
                                printf("read finish!\n");
                                printf("������ֵ�ǣ� %s\n",buf);
								WaitCardOff();
							}
						}
					}
				}
			}
		}

        if(num % 20 == 0)
            LED0 = !LED0;
        num++;
    }
}
