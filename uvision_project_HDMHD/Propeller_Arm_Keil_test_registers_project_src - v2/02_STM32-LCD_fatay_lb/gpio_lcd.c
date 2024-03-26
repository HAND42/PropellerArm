#include "gpio_lcd.h"

void LCD_GPIO_Init(void){
   GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE,DATA4_Pin|DATA5_Pin|DATA6_Pin|DATA7_Pin
                          |RS_Pin|E_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : DATA4_Pin DATA5_Pin DATA6_Pin DATA7_Pin
                           RS_Pin E_Pin*/
	
  GPIO_InitStruct.Pin = DATA4_Pin|DATA5_Pin|DATA6_Pin|DATA7_Pin
                          |RS_Pin|E_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

}

void LCD_GPIO_Init_register(void){
   //1.Enable the GPIO clock
	RCC->AHB1ENR |= (1<<4); // Enable the clock of port E of the GPIO
	
	// Output mode
	GPIOE->MODER |= (1<<22); //RS_PIN
	GPIOE->MODER |= (1<<24); //E
	
	GPIOE->MODER |= (1<<12); 
	GPIOE->MODER |= (1<<10); 
	GPIOE->MODER |= (1<<8); 
	GPIOE->MODER |= (1<<6); 
	
	GPIOE->MODER |= (1<<14); //D4
	GPIOE->MODER |= (1<<16); //D5
	GPIOE->MODER |= (1<<18); //D6
	GPIOE->MODER |= (1<<20); //D7
	
	//No pull
	GPIOE->PUPDR &= ~(1<<22);
	GPIOE->PUPDR &= ~(1<<24);
	GPIOE->PUPDR &= ~(1<<14);
	GPIOE->PUPDR &= ~(1<<16);
	GPIOE->PUPDR &= ~(1<<18);
	GPIOE->PUPDR &= ~(1<<20);
	
	/*GPIOE -> BSRR = 1 << 7 << 16;
	GPIOE -> BSRR = 1 << 8 << 16;
	GPIOE -> BSRR = 1 << 9 << 16;
	GPIOE -> BSRR = 1 << 10 << 16;
	GPIOE -> BSRR = 1 << 11<< 16;
	GPIOE -> BSRR = 1 << 12<< 16;
	GPIOE -> BSRR = 1 << 10<< 16;*/
	//while(1);
   //GPIOE -> BSRR = 1 << (8 + 16);
	
	
}

