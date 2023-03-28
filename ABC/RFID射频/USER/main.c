#include "output.h"
#include "delay.h"
#include "sys.h"
#include "rc522.h"
#include "lcd.h"			 //��ʾģ��
#include "key.h"             //
#include "usart.h"
#include "string.h" 
#include "led.h"
#include "store.h"
/*******************************
*����˵����
*1--SS  <----->PF0
*2--SCK <----->PB13
*3--MOSI<----->PB15
*4--MISO<----->PB14   
*5--����
*6--GND <----->GND
*7--RST <----->PF1
*8--VCC <----->VCC
************************************/
unsigned char NEWUSERKEYINITA[6]={1,1,1,1,1,1};
unsigned char NEWUSERKEYINITB[6]={0,0,0,0,0,0};
int main(void)
{		
	unsigned char status;
	u8 key=0;
	//u8 i=0;
  //u8 RETRY=0x10; //���ʮ����֤����Ա�û���
	//u8 flag=1;
	Stm32_Clock_Init(9);
	MY_NVIC_PriorityGroupConfig(2); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
 	delay_init(72);	    	 //��ʱ������ʼ��	
 	OUTPUT_Init();			 //���ģ���ʼ��
	uart_init(72,115200);
	LCD_Init();
	KEY_Init();
	LED_Init();
	InitRc522();				//��ʼ����Ƶ��ģ��
	POINT_COLOR=RED;
	LCD_ShowString(40,80,240,24,24,(u8*)"open the ");
	LCD_ShowString(20,110,240,24,24,(u8*)"serial assistant.");
	LCD_ShowString(0,140,240,16,16,(u8*)"KEY0:increment ");				
	LCD_ShowString(0,160,240,16,16,(u8*)"KEY1:decrement");
	LCD_ShowString(0,180,240,16,16,(u8*)"KEY2:change password");
	printf("***************************����˵��***************************************\r\n");
	printf("KEY0��ֵģʽ��KEY1����ģʽ��KEY2�޸�����\r\n");
  printf("**************************************************************************\r\n");
	while(1) 
	{
		#if 0
		ReadCard(addrUserCon,USERKEYINITB,0);
		if(status==MI_OK)
		{
			status = MI_ERR;
			status=PcdWrite(addrUserCon,NEWUSERKEYINITA);
		}
		if(status==MI_OK)
		{
			status = MI_ERR;
			status=PcdRead(addrUserCon,NEWUSERKEYINITB);
		}	
		if(status==MI_OK)
		{
			status = MI_ERR;
			printf("������1:%02x %02x %02x %02x %02x %02x \n",NEWUSERKEYINITB[0],NEWUSERKEYINITB[1],NEWUSERKEYINITB[2],NEWUSERKEYINITB[3],NEWUSERKEYINITB[4],NEWUSERKEYINITB[5]);
		}
		
		#endif
		#if 0
		key=KEY_Scan(0);
		if(key==1) 
		{
			printf("��ˢ��\r\n\r\n");
			wallet_Init(addrUserDat,CARDKEYINIT);
		}
		#endif
		#if 1
		key=KEY_Scan(0);
		if(key==1)  												   	//increment   
		{
			status=userRecharge(addrUserDat,CARDKEYINIT,Recharge,0);  	//��ֵ���
			if(status==MI_OK)
			{
				printf("���:%d ��ֵ:%d �����:%d\r\n\r\n",prevMoney,Recharge,laterMoney);
			}
		}
		else if(key==2)													//decrement
		{
			status=userConsume(addrUserDat,CARDKEYINIT,Recharge,1);  	//���ѽ��
			if(status==MI_OK)
			{
				printf("���:%d ����:%d �����:%d\r\n\r\n",prevMoney,Recharge,laterMoney);
			}
		}
		else if(key==3)													//change password
		{
			status=changeUserPwd(addrUserDat,addrUserCon,CARDKEYINIT,CARDKEYINIT,NEWUSERKEYINITA,0);
			if(status==MI_OK)
			{
				printf("�����޸ĳɹ�\r\n");
			}
		}
		#endif
	}
}
		
		
