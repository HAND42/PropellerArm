

/**
  ******************************************************************************

  I2C Setup For STM32F446RE
  Author:   ControllersTech
  Updated:  31st Jan 2020

  ******************************************************************************
  Copyright (C) 2017 ControllersTech.com

  This is a free software under the GNU license, you can redistribute it and/or modify it under the terms
  of the GNU General Public License version 3 as published by the Free Software Foundation.
  This software library is shared with public for educational purposes, without WARRANTY and Author is not liable for any damages caused directly
  or indirectly by this software, read more about this on the GNU General Public License.

  ******************************************************************************
*/

#include "I2C-ST_Slave_Ard_Master.h"

volatile uint8_t data_received[2];
volatile uint8_t data_index = 0;


  /*****************************************************************************************************************************************/
 /*		---------------------------------------------	I2C SLAVE CONFIG - SCREEN COMMUNICATION 		------------------------------------*/
/*****************************************************************************************************************************************/

void I2C3_Begin(void){
	// Start listening on the I2C address
	I2C3->CR2 |= I2C_CR2_ITBUFEN | I2C_CR2_ITEVTEN | I2C_CR2_ITERREN; // Enable Buffer, Event, and Error interrupts
	
	// Enable ACK
	I2C3->CR1 |= I2C_CR1_ACK;
	
	// Clear ADDR flag by reading SR1 then SR2
	(void)I2C3->SR1;
	(void)I2C3->SR2;

	// Enable the buffer interrupt
	I2C3->CR2 |= I2C_CR2_ITBUFEN;
}

void I2C3_Init(void)
{
    RCC->APB1ENR |= RCC_APB1ENR_I2C3EN;  // Enable I2C3 clock
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOCEN;  // Enable GPIOA and GPIOC clocks

    // Configure PA8 (I2C3 SCL) and PC9 (I2C3 SDA)
    GPIOA->MODER |= GPIO_MODER_MODER8_1;  // Alternate function mode
    GPIOA->OTYPER |= GPIO_OTYPER_OT8;  // Open-drain
    GPIOA->AFR[1] |= 4 << GPIO_AFRH_AFSEL8_Pos;  // AF4 (I2C3)

    GPIOC->MODER |= GPIO_MODER_MODER9_1;  // Alternate function mode
    GPIOC->OTYPER |= GPIO_OTYPER_OT9;  // Open-drain
    GPIOC->AFR[1] |= 4 << GPIO_AFRH_AFSEL9_Pos;  // AF4 (I2C3)
	
	 // Reset the I2C 
	 I2C1->CR1 |= (1<<15);
	 I2C1->CR1 &= ~(1<<15);
	
	 // Configure I2C3
    I2C3->CR1 &= ~I2C_CR1_PE;  // Disable I2C3
	
	// Assuming PCLK1 is set to 50 MHz (as APB1 is typically half of the system clock)
    // Configure I2C3 for 100kHz I2C speed
    // T_high = T_low = CCR * T_PCLK1
    I2C3->CR2 = 50;  // Set PCLK1 frequency (50 MHz)
    I2C3->OAR1 = (SLAVE_ADDRESS_SCREEN_CTR_STM32 << 1);  // Set own address
    I2C3->CCR = 250;  // Set clock control (100 kHz)
    // TRISE = (PCLK1 / 1MHz) + 1 = 50MHz / 1MHz + 1 = 51
    I2C3->TRISE = 51;  // Set maximum rise time
	 
	 
    I2C3->CR1 |= I2C_CR1_PE;  // Enable I2C3
	
	 // Enable I2C3 interrupts
    NVIC_EnableIRQ(I2C3_EV_IRQn);  // Enable I2C3 event interrupt in NVIC
    NVIC_EnableIRQ(I2C3_ER_IRQn);  // Enable I2C3 error interrupt in NVIC
	 NVIC_SetPriority(I2C3_EV_IRQn, I2C3_IRQ_PRIORITY);
 
}

void I3C3_Stop(void){
    I2C3->CR2 &= ~(I2C_CR2_ITEVTEN | I2C_CR2_ITERREN);  // Disable event and error interrupts
	 NVIC_DisableIRQ(I2C3_EV_IRQn);  // Enable I2C3 event interrupt in NVIC
    NVIC_DisableIRQ(I2C3_ER_IRQn);  // Enable I2C3 error interrupt in NVIC
}

void I2C3_ReadData(void)
{
    char buffer[USART2_TX_BUFFER_SIZE];  
	 switch (data_received[0])
    {
        case 0x1:
			  sprintf(buffer, "SWITCH ON/OFF\r\n");
			  power_on = !power_on;
            break;
        case 0x02:
            sprintf(buffer, "Received command: STOP\r\n");
            break;
        case 0x03:
            sprintf(buffer, "Received command: PAUSE\r\n");
            break;
        case 0x04:
            sprintf(buffer, "Received command: RESUME\r\n");
            break;
        default:
            sprintf(buffer, "Received unknown data: 0x%02X\r\n", data_received);
            break;
    }
	 SendString(buffer);
}

// I2C3 event interrupt handler
void I2C3_EV_IRQHandler(void)
{
    if (I2C3->SR1 & I2C_SR1_ADDR)  // Address matched
    {
        volatile uint32_t temp = I2C3->SR1;  // Read SR1 and SR2 to clear the ADDR flag
        temp = I2C3->SR2;
        data_index = 0;
    }
    else if (I2C3->SR1 & I2C_SR1_RXNE)  // Data received
    {
        if (data_index < 2)  // Ensure we only read two bytes
        {
            data_received[data_index] = I2C3->DR;  // Read data
            data_index++;
        }
        if (data_index == 2)
        {
            // Process received_data array here
            data_index = 0;  // Reset index for next reception
        }
    }
    else if (I2C3->SR1 & I2C_SR1_STOPF)  // Stop condition detected
    {
        I2C3->CR1 |= I2C_CR1_PE;  // Clear STOPF flag by writing to CR1
    }
}

// I2C3 error interrupt handler
void I2C3_ER_IRQHandler(void)
{
    if (I2C3->SR1 & I2C_SR1_AF)  // Acknowledge failure
    {
        I2C3->SR1 &= ~I2C_SR1_AF;  // Clear AF flag
    }
    else
    {
        Error_Handler();  // Handle other errors
    }
}

void Error_Handler(void)
{
    while (1)
    {
        GPIOD->ODR ^= GPIO_ODR_OD14;
        for (volatile uint32_t i = 0; i < 100000; ++i);
    }
}



