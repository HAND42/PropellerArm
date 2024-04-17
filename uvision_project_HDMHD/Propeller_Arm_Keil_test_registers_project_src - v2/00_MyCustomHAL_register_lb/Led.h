
// Allow to test the Clock frequency and the set up of the timer delay.

#include "stm32f411xe.h"
#include "Delay.h"

#pragma once

typedef enum {
    LED = 0,
    YELLOW =12,
	 ORANGE =13,
	RED=14,
	BLUE=15,
    LED_INDEX = 20
} Choose_Led;

void GPIO_Config (void);
void Led_Trigger(Choose_Led ledNum, uint16_t time);
void Testing_Toggle_4Led (void);
void Power_Led_On(void);
void Power_Led_Off(void);



