#ifndef _water_H
#define _water_H


#include "stm32f10x.h"
 
void Water_GPIO_Config(void);
int Water_state(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin_x);
void Water_state_printf(void);
float Get_LiquidHeight(void);
u16 Get_ADCValue_Water(void);
u16 Get_ADCValue_MQ2(void);
void MQ2_PPM_Calibration(float RS);
float MQ2_GetPPM(void);
void Water_GPIO_Config(void);

#endif
