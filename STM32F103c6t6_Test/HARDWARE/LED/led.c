#include "led.h"
void LED_Init(){
	GPIO_InitTypeDef GPIO_InitStructure; // ����GPIO��ʼ���ṹ�����

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); // ʹ��GPIOCʱ��

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13; // ��������
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; // ����ģʽΪ�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // ��������������Ϊ50MHz
    GPIO_Init(GPIOC, &GPIO_InitStructure); // ��ʼ��GPIOC��
}

// GPIO_SetBits(GPIOC, GPIO_Pin_13); // ����LED
// delay_ms(1000); // ��ʱ1s
// GPIO_ResetBits(GPIOC, GPIO_Pin_13); // �ر�LED
// delay_ms(1000); // ��ʱ1s