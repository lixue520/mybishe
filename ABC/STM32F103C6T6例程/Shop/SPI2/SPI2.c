#include "SPI2.h"

//SPI2�ĳ�ʼ��
void SPI2_Init(void)
{
//	RCC->APB2ENR|=(1<<3); //GPIOB����ʱ��
//	RCC->APB1ENR|=(1<<14);//����SPI2ʱ��
//	GPIOB->CRH&=0x000FFFFF;
//	GPIOB->CRH|=0xB4B00000;//����PB13��PB14��PB15
//	GPIOB->BSRR|=7<<13;  //����
//	GPIOF->CRL&=0xFFFFFF00;
//	GPIOF->CRL|=0x0000033;  //PF0\PF1������������50MHZ
//	GPIOF->BRR=0X02;   //����
//	SPI2->CR1|= (0<<11)| //8λ����֡
//				(0<<10)| //ȫ˫��ģʽ
//				(1<<9)|  //����������豸����	��������CS
//				(1<<8)|  //ѡ����豸
//				(0<<7)|  //������MSB
//				(1<<2)|  //����Ϊ���豸
//				(0<<1)|  //CPOL��ʱ�Ӽ���
//				(0<<0);  //CPHA��ʱ����λ
	    GPIO_InitTypeDef GPIO_InitStructure;
    SPI_InitTypeDef  SPI_InitStructure;

    RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );//PORTBʱ��ʹ��
    RCC_APB1PeriphClockCmd(	RCC_APB1Periph_SPI2,  ENABLE );//SPI2ʱ��ʹ��

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //PB13/14/15�����������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOB

    GPIO_SetBits(GPIOB,GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);  //PB13/14/15����

    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//����SPI����ģʽ:����Ϊ��SPI
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;		//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
    SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRCֵ����Ķ���ʽ
    SPI_Init(SPI2, &SPI_InitStructure);  //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���

    SPI_Cmd(SPI2, ENABLE); //ʹ��SPI����

    SPI2_ReadWriteByte(0xff);//��������
}
//SPI2�ٶȵ����� �������SPEEDΪһ����ֵ
//000�� fPCLK/2   001�� fPCLK/4   010�� fPCLK/8    011�� fPCLK/16
//100�� fPCLK/32  101�� fPCLK/64  110�� fPCLK/128  111�� fPCLK/256
//SPI2����APB1�����ϣ����ʱ��Ƶ��Ϊ36MHZ��fPCLK=fAPB1=36MHZ
void SPI2_SpeedSet(u8 speed)
{
	speed&=0x07;
	SPI2->CR1&=0xFFC7;
	SPI2->CR1|=speed<<3;
	SPI2->CR1|=(1<<6);
}
//��spi2�Ķ�д����
u8 SPI2_ReadWriteByte(u8 TxData)
{		
	u16 retry=0;				 
	while((SPI2->SR&1<<1)==0)		//�ȴ���������	
	{
		retry++;
		if(retry>=0XFFFE)return 0; 	//��ʱ�˳�
	}			  
	SPI2->DR=TxData;	 	  		//����һ��byte 
	retry=0;
	while((SPI2->SR&1<<0)==0) 		//�ȴ�������һ��byte  
	{
		retry++;
		if(retry>=0XFFFE)return 0;	//��ʱ�˳�
	}	  						    
	return SPI2->DR;          		//�����յ�������				    
}

u8 SPIWriteByte(u8 Byte)
{
	while((SPI2->SR&0X02)==0);		//�ȴ���������	  
	SPI2->DR=Byte;	 	            //����һ��byte   
	while((SPI2->SR&0X01)==0);      //�ȴ�������һ��byte  
	return SPI2->DR;          	    //�����յ�������			
}










