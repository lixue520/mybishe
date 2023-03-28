#include "output.h"
//*连线说明：
//*1--SS  <----->PF0   浮空输入
//*2--SCK <----->PB13  推挽复用输出
//*3--MOSI<----->PB15  推挽复用输出
//*4--MISO<----->PB14  浮空输入
//*5--悬空
//*6--GND <----->GND   
//*7--RST <----->PF1   浮空输入
//*8--VCC <----->VCC
//////////////////////////////////////////////////////////////////////////////////	 

//输出输入驱动代码	   
									  
////////////////////////////////////////////////////////////////////////////////// 	   

void OUTPUT_Init(void)
{	
//	RCC->APB2ENR|=3<<6|1<<3|1<<2; //开启PA,PB,PE，PF时钟
//	GPIOB->CRH&=0x000FFFFF;
//	GPIOB->CRH|=0x94900000;  //PB13\14\15
//	GPIOA->CRL&=0xFFFFFFF0;


RCC->APB2ENR |= (1 << 3) | (1 << 2); // 开启PB和PA时钟
GPIOB->CRH &= 0xFFFFF0FF;
GPIOB->CRH |= 0xB00; // 将 PB13、PB14、PB15 配置为 AF push-pull 输出模式
GPIOB->CRL &= 0xFFFFFFF0;
GPIOB->CRL |= 0xB; // 将 PB0、PB1 配置为 AF push-pull 输出模式
GPIOB->BSRR |= 7 << 13; // 上拉
GPIOB->BRR |= 3; // 下拉 PB0、PB1

}
 
