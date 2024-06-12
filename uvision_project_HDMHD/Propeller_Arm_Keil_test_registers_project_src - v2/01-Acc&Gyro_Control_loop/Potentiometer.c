#include "Potentiometer.h"

/**

The following code needs the implementation of the timer to be initialized.

**/

void ADC_Init(void){
	
	/************** STEPS TO FOLLOW *****************
	1. Enable ADC and GPIO clock
	2. Set the prescalar in the Common Control Register (CCR)
	3. Set the Scan Mode and Resolution in the Control Register 1 (CR1)
	4. Set the Continuous Conversion, EOC, and Data Alignment in Control Reg 2 (CR2)
	5. Set the Sampling Time for the channels in ADC_SMPRx
	6. Set the Regular channel sequence length in ADC_SQR1
	7. Set the Respective GPIO PINs in the Analog Mode
	************************************************/
	
	// 1. Enable ADC and GPIO clock
	RCC->AHB1ENR |= (1<<0); // Enable GPIOA clock
	RCC->APB2ENR |= (1<<8); // Enable ADC1 clock
	
	// 2. Set the prescalar in the Common Control Register (CCR)
	ADC->CCR |= (1<<16);	  // PCLK2 divide by 4 --> Maxfrq 36MHz with VDDA = 3.3V set by default & APB2_clk/presc = 100MHz/4 = 25MHz < 36MHz (see Datasheet STM32F411)
	
	// 3. Set the Scan Mode and Resolution in the Control Register 1 (CR1)
	ADC1->CR1 &= ~(3<<24); // 12 bit RES
	//ADC1->CR1 &= ~(3 << 24); //6 bit RES
	ADC1->CR1 &= ~(1<<8);  // SCAN mode disabled -> only one channel to control.
	
	// 4. Set the Continuous Conversion, EOC, and Data Alignment in Control Reg 2 (CR2)
	ADC1->CR2 |= (1<<1);     // enable continuous conversion mode
	ADC1->CR2 |= (1<<10);    // EOC after each conversion  -> Overrun detection is enabled.
	ADC1->CR2 &= ~(1<<11);   // Data Alignment RIGHT
	
	// 5. Set the Sampling Time for the channels in ADC_SMPRx
	ADC1->SMPR2 &= ~(1<<12);  // Sampling time of 3 cycles for channel 4
	
	//6. Set the Regular channel sequence length in ADC_SQR1
	ADC1->SQR1 |= (1<<20);   // SQR1_L =1 for 2 conversions
	
	//7. Set the Respective GPIO PINs in the Analog Mode	
	GPIOA->MODER |= (3<<8);  // analog mode for PA4 (channel 4)
	
}

void ADC_Enable (void){
	/************** STEPS TO FOLLOW *****************
	1. Enable the ADC by setting ADON bit in CR2
	2. Wait for ADC to stabilize (approx 10us) 
	************************************************/
	ADC1->CR2 |= 1<<0;   // ADON =1 enable ADC1
	
	delay_us(10);
}

void ADC_Start (int channel){
	/************** STEPS TO FOLLOW *****************
	1. Set the channel Sequence in the SQR Register
	2. Clear the Status register
	3. Start the Conversion by Setting the SWSTART bit in CR2
	************************************************/
	
	
/**	Since we will be polling for each channel, here we will keep one channel in the sequence at a time
		ADC1->SQR3 |= (channel<<0); will just keep the respective channel in the sequence for the conversion **/
	
	ADC1->SQR3 = 0;
	ADC1->SQR3 |= (channel<<0);    // conversion in regular sequence
	
	ADC1->SR = 0;        // clear the status register
	
	ADC1->CR2 |= (1<<30);  // start the conversion
}


void o_Potentiometer_Initialization_o(void){

	ADC_Init ();
	ADC_Enable ();
	
}

void ADC_WaitForConv (void){
	/*************************************************
	EOC Flag will be set, once the conversion is finished
	*************************************************/
	while (!(ADC1->SR & (1<<1)));  // wait for EOC flag to set
}

uint16_t ADC_GetVal (void){
	return ADC1->DR;  // Read the Data Register
}

void ADC_Disable (void){
	/************** STEPS TO FOLLOW *****************
	1. Disable the ADC by Clearing ADON bit in CR2
	************************************************/	
	ADC1->CR2 &= ~(1<<0);  // Disable ADC
}

float o_Read_Potentiometer_o(int channel){
	
	ADC_Start (channel);
	ADC_WaitForConv ();
	
	// Assume that we want to control our system between -45° and 45°
	int read_val = ADC_GetVal();
	
	// y = ax + b and as the potentiometer goes from 0 to 4095 so a = 90/4095 and b = -45° 
	return read_val*90.0/4095.0 -45.0;
	
}
