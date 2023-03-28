#include "output.h"
//*����˵����
//*1--SS  <----->PF0   ��������
//*2--SCK <----->PB13  ���츴�����
//*3--MOSI<----->PB15  ���츴�����
//*4--MISO<----->PB14  ��������
//*5--����
//*6--GND <----->GND   
//*7--RST <----->PF1   ��������
//*8--VCC <----->VCC
//////////////////////////////////////////////////////////////////////////////////	 

//���������������	   
									  
////////////////////////////////////////////////////////////////////////////////// 	   

void OUTPUT_Init(void)
{	
//	RCC->APB2ENR|=3<<6|1<<3|1<<2; //����PA,PB,PE��PFʱ��
//	GPIOB->CRH&=0x000FFFFF;
//	GPIOB->CRH|=0x94900000;  //PB13\14\15
//	GPIOA->CRL&=0xFFFFFFF0;


RCC->APB2ENR |= (1 << 3) | (1 << 2); // ����PB��PAʱ��
GPIOB->CRH &= 0xFFFFF0FF;
GPIOB->CRH |= 0xB00; // �� PB13��PB14��PB15 ����Ϊ AF push-pull ���ģʽ
GPIOB->CRL &= 0xFFFFFFF0;
GPIOB->CRL |= 0xB; // �� PB0��PB1 ����Ϊ AF push-pull ���ģʽ
GPIOB->BSRR |= 7 << 13; // ����
GPIOB->BRR |= 3; // ���� PB0��PB1

}
 
