#include "adc.h"

#include "delay.h"

// ALIENTEK miniSTM32������
// ADC��ͨ�� ����

// #define SEND_BUF_SIZE 8200	//�������ݳ���,��õ���sizeof(TEXT_TO_SEND)+2��������.
// u8 SendBuff[SEND_BUF_SIZE];	//�������ݻ�����
__IO uint16_t ADC_ConvertedValue[2];

// u16 DMA1_MEM_LEN;//����DMAÿ�����ݴ��͵ĳ���

// DMA1�ĸ�ͨ������
// ����Ĵ�����ʽ�ǹ̶���,���Ҫ���ݲ�ͬ��������޸�
// �Ӵ洢��->����ģʽ/8λ���ݿ��/�洢������ģʽ
// DMA_CHx:DMAͨ��CHx
// cpar:�����ַ
// cmar:�洢����ַ
// cndtr:���ݴ�����
void MYDMA_Config()
{
	DMA_InitTypeDef DMA_InitStructure;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); // ʹ��DMA����

	DMA_DeInit(DMA1_Channel1); // ��DMA��ͨ��1�Ĵ�������Ϊȱʡֵ

	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&(ADC1->DR));	 // DMA�������ַ   �洢�����洢��
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ADC_ConvertedValue; // DMA�ڴ����ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;					 // ���ݴ��䷽��
	DMA_InitStructure.DMA_BufferSize = 2;								 // DMAͨ����DMA����Ĵ�С
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	 // �����ַ�Ĵ�������
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; // ����16λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;		  // �Ƿ���ѭ��ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; // DMAͨ��  ӵ�������ȼ�
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;		  // DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);		  // ����DMA_InitStruct��ָ���Ĳ�����ʼ��DMA��ͨ��USART1_Tx_DMA_Channel����ʶ�ļĴ���

	// DMA_Cmd(DMA1_Channel1, DISABLE );  //�ر�USART1 TX DMA1 ��ָʾ��ͨ��
	// DMA_SetCurrDataCounter(DMA1_Channel1,6);//DMAͨ����DMA����Ĵ�С
	DMA_Cmd(DMA1_Channel1, ENABLE);
}

// ��ʼ��ADC
// �������ǽ��Թ���ͨ��Ϊ��
// ����Ĭ�Ͻ�����ͨ��0~3
void Adc_Init(void)
{
	ADC_InitTypeDef ADC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_ADC1, ENABLE); // ʹ��ADC1ͨ��ʱ��

	MYDMA_Config();

	RCC_ADCCLKConfig(RCC_PCLK2_Div6); // ����ADC��Ƶ����6 72M/6=12,ADC���ʱ�䲻�ܳ���14M

	// PA0 PA1 ��Ϊģ��ͨ����������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; // ģ����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	ADC_DeInit(ADC1); // ��λADC1,������ ADC1 ��ȫ���Ĵ�������Ϊȱʡֵ

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; // ADC����ģʽ:ADC1��ADC2�����ڶ���ģʽ
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;	   // ģ��ת�������ڵ�ͨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; // ת��������������ⲿ��������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;				// ADC�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 2;								// ˳����й���ת����ADCͨ������Ŀ
	ADC_Init(ADC1, &ADC_InitStructure);									// ����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���

	ADC_DMACmd(ADC1, ENABLE);

	ADC_Cmd(ADC1, ENABLE); // ʹ��ָ����ADC1

	ADC_ResetCalibration(ADC1); // ʹ�ܸ�λУ׼

	while (ADC_GetResetCalibrationStatus(ADC1))
		; // �ȴ���λУ׼����

	ADC_StartCalibration(ADC1); // ����ADУ׼

	while (ADC_GetCalibrationStatus(ADC1))
		; // �ȴ�У׼����

	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_55Cycles5);

	ADC_SoftwareStartConvCmd(ADC1, ENABLE); // ʹ��ָ����ADC1�����ת����������
}
/*
#if 0
//���ADCֵ
//ch:ͨ��ֵ 0~3
u16 Get_Adc(u8 ch)
{
	//����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADCͨ��,����ʱ��Ϊ239.5����

	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//ʹ��ָ����ADC1�����ת����������

	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//�ȴ�ת������

	return ADC_GetConversionValue(ADC1);	//�������һ��ADC1�������ת�����
}

u16 Get_Adc_Average(u8 ch,u8 times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val+=Get_Adc(ch);
		delay_ms(5);
	}
	return temp_val/times;
}
#endif
*/

//	DMA_Cmd(DMA_CHx, DISABLE );  //�ر�USART1 TX DMA1 ��ָʾ��ͨ��
// 	DMA_SetCurrDataCounter(DMA_CHx,DMA1_MEM_LEN);//DMAͨ����DMA����Ĵ�С
// 	DMA_Cmd(DMA_CHx, ENABLE);  //ʹ��USART1 TX DMA1 ��ָʾ��ͨ��
