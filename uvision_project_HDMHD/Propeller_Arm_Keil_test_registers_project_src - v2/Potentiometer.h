#pragma once
#include "stm32f411xe.h"
#include "Delay.h"

void ADC_Init(void);
void ADC_Enable (void);
void ADC_Start (int channel);
void o_Potentiometer_Initialization_o(void);

void ADC_WaitForConv (void);
uint16_t ADC_GetVal (void);
void ADC_Disable (void);
float o_Read_Potentiometer_o(int channel);
