#include "Led.h"

void LED_GPIO_Config (void){
	//1.Enable the GPIO clock
	RCC->AHB1ENR |= (1<<3); // Enable the clock of port D of the GPIO
	
	//2. Set the Pin GPIO CLOCK
	GPIOD->MODER |= (1<<26);
	GPIOD->MODER |= (1<<24);
	GPIOD->MODER |= (1<<28);
	GPIOD->MODER |= (1<<30);
	
}



void Led_Trigger(Choose_Led ledNum, uint16_t time){
	GPIOD->ODR ^= (1 << ledNum);
	delay_ms(time);
}

void Testing_Toggle_4Led (void){
	
	Led_Trigger(YELLOW,200);
	Led_Trigger(ORANGE, 200);
	Led_Trigger(RED, 200);
	Led_Trigger(BLUE, 200);
	Led_Trigger(YELLOW,200);
	Led_Trigger(ORANGE, 200);
	Led_Trigger(RED, 200);
	Led_Trigger(BLUE, 200);
}

void Blinking_4Led (void){
	Led_Trigger(YELLOW,0);
	Led_Trigger(ORANGE,0);
	Led_Trigger(RED,0);
	Led_Trigger(BLUE,0);
	delay_ms(1000);
}

void Power_On(void){
	GPIOD->BSRR = GPIO_BSRR_BS_15;
	GPIOD->BSRR = GPIO_BSRR_BR_14;
}

void Power_Off(void){
	GPIOD->BSRR = GPIO_BSRR_BS_14;
	GPIOD->BSRR = GPIO_BSRR_BR_15;
}
