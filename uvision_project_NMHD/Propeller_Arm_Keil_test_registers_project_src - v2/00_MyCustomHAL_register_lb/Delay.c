
#include "Delay.h"
#include "System_Config.h"

void TIM5Config (void){
	//1. Enable Timer Clock
	RCC->APB1ENR |= (1<<3); 
	
	//2. Set the prescaler 
	TIM5->PSC = 100-1; //100MHz/100 = 1MHz ~~ 1 us delay -- be carrefull about to look at APB1 timer and not peripheral on stm32CUBEMX
	TIM5->ARR = 0xffffff; //MAX ARR value
	
	
	//3. Enable the Timer, and wait for the update Flag to set
	TIM5	->CR1 |= (1<<0); // Enable the counter
	
	// Wait until the TIM5 Update Interrupt Flag is set
	while (!(TIM5->SR & TIM_SR_UIF));
	
}

void delay_us (uint16_t time){
	
	TIM5->CNT = 0;
	while (TIM5->CNT < time);
	
}

void delay_ms (uint16_t time){
	for (uint16_t i=0; i<time; i++){
		delay_us(1000);
	}
}

