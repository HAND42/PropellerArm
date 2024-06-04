#ifndef SYSTEM_CONFIG_H
#define SYSTEM_CONFIG_H

#include "stm32f411xe.h"
#include <stdio.h>

typedef int bool;
#define true 1
#define false 0
	
#define HSE_VALUE ((uint32_t)8000000)
	
#if !defined  (HSE_VALUE)
#define HSE_VALUE 8 000 000
#endif

#if !defined  (HSI_VALUE)
#define HSI_VALUE 16 000 000
#endif

extern int power_on;
extern float debouncing_check_button_time;

typedef struct {
    uint32_t hseFrequency; // HSE clock frequency (in Hz)
    uint32_t plln; // PLL N value
    uint32_t pllp; // PLL P value
    uint32_t pllm; // PLL M value
    uint32_t apb1Frequency; // APB1 clock frequency (in Hz)
	 uint32_t apb1timFrequency; // APB1 timer clock frequency (in Hz)
    uint32_t apb2Frequency; // APB2 clock frequency (in Hz)
    uint32_t systemFrequency; // System clock frequency (in Hz)
} ClockConfig;

void SysClockConfig(void);
void Timer2InterruptInit(float oscillation_button_time);
void Timer2InterruptEnable(void);
void Sys_PowerMode_Config(void);
ClockConfig getSystemClockSpeed(bool show);
int convertPPRE(int ppre_bits);

#endif /* SYSTEM_CONFIG_H */

