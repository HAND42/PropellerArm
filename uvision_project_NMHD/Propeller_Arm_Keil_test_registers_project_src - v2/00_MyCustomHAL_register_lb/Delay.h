#ifndef DELAY_H
#define DELAY_H

#include "stm32f411xe.h"


void TIM5Config (void);
void delay_us (uint16_t time);
void delay_ms (uint16_t time);

#endif /* RCC_CONFIG_H */

