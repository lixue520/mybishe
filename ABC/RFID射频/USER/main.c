#include "output.h"
#include "delay.h"
#include "sys.h"
#include "rc522.h"
#include "lcd.h"			 //显示模块
#include "key.h"             //
#include "usart.h"
#include "string.h" 
#include "led.h"
#include "store.h"
/*******************************
*连线说明：
*1--SS  <----->PF0
*2--SCK <----->PB13
*3--MOSI<----->PB15
*4--MISO<----->PB14   
*5--悬空
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
  //u8 RETRY=0x10; //最多十次验证管理员用户卡
	//u8 flag=1;
	Stm32_Clock_Init(9);
	MY_NVIC_PriorityGroupConfig(2); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
 	delay_init(72);	    	 //延时函数初始化	
 	OUTPUT_Init();			 //输出模块初始化
	uart_init(72,115200);
	LCD_Init();
	KEY_Init();
	LED_Init();
	InitRc522();				//初始化射频卡模块
	POINT_COLOR=RED;
	LCD_ShowString(40,80,240,24,24,(u8*)"open the ");
	LCD_ShowString(20,110,240,24,24,(u8*)"serial assistant.");
	LCD_ShowString(0,140,240,16,16,(u8*)"KEY0:increment ");				
	LCD_ShowString(0,160,240,16,16,(u8*)"KEY1:decrement");
	LCD_ShowString(0,180,240,16,16,(u8*)"KEY2:change password");
	printf("***************************操作说明***************************************\r\n");
	printf("KEY0充值模式、KEY1消费模式、KEY2修改密码\r\n");
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
			printf("新密码1:%02x %02x %02x %02x %02x %02x \n",NEWUSERKEYINITB[0],NEWUSERKEYINITB[1],NEWUSERKEYINITB[2],NEWUSERKEYINITB[3],NEWUSERKEYINITB[4],NEWUSERKEYINITB[5]);
		}
		
		#endif
		#if 0
		key=KEY_Scan(0);
		if(key==1) 
		{
			printf("请刷卡\r\n\r\n");
			wallet_Init(addrUserDat,CARDKEYINIT);
		}
		#endif
		#if 1
		key=KEY_Scan(0);
		if(key==1)  												   	//increment   
		{
			status=userRecharge(addrUserDat,CARDKEYINIT,Recharge,0);  	//充值金额
			if(status==MI_OK)
			{
				printf("余额:%d 充值:%d 现余额:%d\r\n\r\n",prevMoney,Recharge,laterMoney);
			}
		}
		else if(key==2)													//decrement
		{
			status=userConsume(addrUserDat,CARDKEYINIT,Recharge,1);  	//消费金额
			if(status==MI_OK)
			{
				printf("余额:%d 消费:%d 现余额:%d\r\n\r\n",prevMoney,Recharge,laterMoney);
			}
		}
		else if(key==3)													//change password
		{
			status=changeUserPwd(addrUserDat,addrUserCon,CARDKEYINIT,CARDKEYINIT,NEWUSERKEYINITA,0);
			if(status==MI_OK)
			{
				printf("密码修改成功\r\n");
			}
		}
		#endif
	}
}
		
		
