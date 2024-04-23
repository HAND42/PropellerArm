#ifndef SYSTEM_CONFIG_H
#define SYSTEM_CONFIG_H

#include "stm32f411xe.h"

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

extern bool power_on;
extern int count;

typedef struct {
    uint32_t hseFrequency; // HSE clock frequency (in Hz)
    uint32_t plln; // PLL N value
    uint32_t pllp; // PLL P value
    uint32_t pllm; // PLL M value
    uint32_t apb1Frequency; // APB1 clock frequency (in Hz)
    uint32_t apb2Frequency; // APB2 clock frequency (in Hz)
    uint32_t systemFrequency; // System clock frequency (in Hz)
} ClockConfig;

void SysClockConfig(void);
void Sys_PowerMode_Config(void);
ClockConfig getSystemClockSpeed(void);
int convertPPRE(int ppre_bits);

#endif /* SYSTEM_CONFIG_H */

