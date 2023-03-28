#include "water.h"
//#include "led.h"
#include "delay.h"
#include "timer.h"
#include "adc.h"
#include "stdio.h"
#include "math.h"
#define CAL_PPM 20		// 校准环境重的PMM值
#define RL 5			// RL阻值
static float R0 = 4.00; // 元件在洁净空气中的阻值 6.0
u16 times_mq = 1;
extern __IO uint16_t ADC_ConvertedValue[2];

// 得到ADC采样内部传感器的值
// 取10次，然后平均
u16 Get_ADCValue_Water(void)
{
	u32 val = 0;
	u8 times = 10;
	u8 count;
	for (count = 0; count < times; count++)
	{
		val += ADC_ConvertedValue[0]; // 获取DMA通道值
		delay_ms(5);
	}
	return val / times;
}

// 得到ADC采样内部传感器的值
// 取10次，然后平均
u16 Get_ADCValue_MQ2(void)
{
	u32 val = 0;
	u8 times = 10;
	u8 count;
	for (count = 0; count < times; count++)
	{
		val += ADC_ConvertedValue[1]; // 获取DMA通道的值
		delay_ms(5);
	}
	return val / times;
}

/********************************************
 * 1.651428	          200               *
 * 1.437143	          300               *
 * 1.257143	          400               *
 * 1.137143	          500               *
 * 1		              600               *
 * 0.928704	          700               *
 * 0.871296	          800               *
 * 0.816667	          900               *
 * 0.785714	          1000              *
 * 0.574393	          2000              *
 * 0.466047	          3000              *
 * 0.415581	          4000              *
 * 0.370478	          5000              *
 * 0.337031	          6000              *
 * 0.305119	          7000              *
 * 0.288169	          8000              *
 * 0.272727	          9000              *
 * 0.254795	          10000             *
 *                                      *
 * ppm = 613.9f * pow(RS/RL, -2.074f)   *
 ***************************************/

// 传感器校准函数
void MQ2_PPM_Calibration(float RS)
{
	R0 = RS / pow(CAL_PPM / 613.9f, 1 / -2.074f);
}

// MQ2传感器数据处理
float MQ2_GetPPM(void)
{
	float Vrl, RS, ppm;
	Vrl = 3.3f * Get_ADCValue_MQ2() / 4096.f;
	Vrl = ((float)((int)((Vrl + 0.005) * 100))) / 100;
	RS = (3.3f - Vrl) / Vrl * RL;

	if (times_mq == 1) // 获取系数执行时间，300ms前进行校准
	{
		R0 = RS / pow(CAL_PPM / 613.9f, 1 / -2.074f);
		times_mq = 0;
	}

	ppm = 613.9f * pow(RS / R0, -2.074f);
	// ppm = pow(11.5428 * 35.904 * Vrl/(25.5-5.1* Vrl),0.6549);
	// https://blog.csdn.net/qq_35952136/article/details/95589074
	return ppm;
}

// Water Sensor水位传感器不仅可以测量水位，还可以测量是否有水，当测量是否有水时，
// 直接检测输出端引脚，若检测为0，则显示没有水，若检测到1，则有水；

/* ---------------------------------------------------------------- */
/**
 * @Name    Water_GPIO_Config
 * @param   None
 * @author  DavidNan
 * @Data    2023-03-03
 * <description> :配置LED用到的I/O口
 **/
/* ---------------------------------------------------------------- */

void Water_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/* ---------------------------------------------------------------- */
/**
 * @Name    Water_state
 * @param   GPIOx: [输入/出]
 **			 GPIO_Pin_x: [输入/出]
 * @author  DavidNan
 * @Data    2023-03-03
 * <description> :
 **/
/* ---------------------------------------------------------------- */

int Water_state(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin_x)
{

	if (GPIO_ReadInputDataBit(GPIOx, GPIO_Pin_x) == 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void Water_state_printf()
{
	if (Water_state(GPIOA, GPIO_Pin_0) == 0)
	{
		printf("\r\n有水");
//		LED2 = 0; // D2指示灯亮
	}
	else
	{
		printf("\r\n无水");
//		LED2 = 1; // D2指示灯灭
	}
}

float Get_LiquidHeight()
{

	float temp1;
	temp1 = Get_ADCValue_Water(); // 水位获取
	temp1 = temp1 * 40 / 4096;
	
	printf("Height:%f mm ,", temp1);
	if(temp1<10){
//		BEEP = 1;
		delay_ms(500);
//		BEEP = 0;
	}else{
//		BEEP = 0;
	}
	// i = temp1;
	// temp1 -= i;							 // 把整数部分除去，留下小数部分
	// temp1 *= 1000;						 // 小数部分*1000，保留3位小数。
	return temp1;
}

void test(){
	float k=0;
	k=Get_ADCValue_Water();
	k = k*5.0/4096.0;
	printf("Height:%f mm ,", k);
}


