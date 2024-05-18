#include "System_Config.h"
#include "Delay.h"
#include "DisplayData.h"


/* Functions definition-----------------------------------------------*/
/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 100 000 000
  *            HCLK(Hz)                       = 100 000 000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 2
  *            APB2 Prescaler                 = 1
  *            HSE Frequency(Hz)              = 8 000 000
  *            PLL_M                          = 4
  *            PLL_N                          = 100
  *            PLL_P                          = 2
  * @param  None
  * @retval None
  */
  
/*	Priority Grouping-----------------------------------------------*/
/**

The priority grouping determines how the 4 bits are split between preemption priority and subpriority. The encoding of priority levels is controlled by the PriorityGroup parameter. Here is a breakdown of how these bits are allocated:

	* Group 0 (NVIC_PRIORITYGROUP_0): 0 bits for preemption priority, 4 bits for subpriority
	* Group 1 (NVIC_PRIORITYGROUP_1): 1 bit for preemption priority, 3 bits for subpriority
	* Group 2 (NVIC_PRIORITYGROUP_2): 2 bits for preemption priority, 2 bits for subpriority
	* Group 3 (NVIC_PRIORITYGROUP_3): 3 bits for preemption priority, 1 bit for subpriority
	* Group 4 (NVIC_PRIORITYGROUP_4): 4 bits for preemption priority, 0 bits for subpriority

*/

#define PRIORITY_OSCILLATION_TIMER 2
#define PRIORITY_BUTTON_INTERRUPTION 3


  
int power_on=0;
int count=0; //to remove

bool button_pressed = 0;

float oscillation_button_time = 0.5;
  
void SysClockConfig(void){
	
	#define  PLLM 4
	#define  PLLN 100
	#define  PLLP 0x00  //PLLP =2
	
	//1. ENABLE HSE and wait for the HSE to become Ready
	RCC->CR |= RCC_CR_HSEON; //The |= is used to set specified bits in the register without changing the other bits.
	while (!(RCC->CR & RCC_CR_HSERDY));
	
	//2. Set the POWER ENABLE CLOCK and VOLTAGE REGULATOR
	RCC->APB1ENR |= RCC_APB1ENR_PWREN; //enable the advanced Peripheral Bus 1 Enable Register to connect various peripherals
	
	PWR->CR |= PWR_CR_VOS; //PWR->CR |= 3<<14; Basically I need to write a 3 (11) in the 14th bit
	
	//3. Configure the FLASH PREFETCH and the LATENCY Related Settings
	FLASH->ACR |= FLASH_ACR_ICEN | FLASH_ACR_DCEN | FLASH_ACR_PRFTEN | FLASH_ACR_LATENCY_5WS;
	//FLASH->ACR = (1<<8) | (1<<9)| (1<<10)| (5<<0);
	
	//4. Configure the PRESCALARS HCLK, PCLK1, PCLK2
	// AHB PR
	RCC->CFGR |= RCC_CFGR_HPRE_DIV1;
	
	// APB1 PR
	RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;
	
	// APB2 PR
	RCC->CFGR |= RCC_CFGR_PPRE2_DIV1;
	
	//5. Configure the MAIN PLL
	RCC->PLLCFGR = (PLLM <<0) | PLLN <<6 | PLLP <<16 | (RCC_PLLCFGR_PLLSRC_HSE);
	
	
	//6. Enable the PLL and wait for it to become ready
	RCC->CR |= RCC_CR_PLLON;
	while (! (RCC->CR & RCC_CR_PLLRDY));
	
	//7. Select the clock source 
	RCC->CFGR |= RCC_CFGR_SW_PLL;
	while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);
	
}



void Sys_PowerMode_Config(void){
	
	/*************>>>>>>> STEPS FOLLOWED <<<<<<<<************
	
	1. Enable GPIO Clock
	2. Set the required Pin in the INPUT Mode
	3. Configure the PULL UP/ PULL DOWN According to your requirement
	
	********************************************************/
	
	RCC->AHB1ENR |=  (1<<0);  // Enable GPIOA clock
	
	GPIOA->MODER &= ~(3<<2);  // Bits (3:2) = 0:0  --> PA1 in Input Mode
	
	GPIOA->PUPDR |=  (1<<2);  // Bits (3:2) = 1:0  --> PA1 is in Pull up mode
	
		/*************>>>>>>> STEPS FOLLOWED <<<<<<<<************
	
	1. Enable the SYSCNFG bit in RCC register 
	2. Configure the EXTI configuration Regiter in the SYSCNFG
	3. Enable the EXTI using Interrupt Mask Register (IMR)
	4. Configure the Rising Edge / Falling Edge Trigger
	5. Set the Interrupt Priority
	6. Enable the interrupt
	
	********************************************************/
	
	RCC->APB2ENR |= (1<<14);  // Enable SYSCNFG
	
	SYSCFG->EXTICR[0] &= ~(0xf<<4);  // Bits[7:6:5:4] = (0:0:0:0)  -> configure EXTI1 line for PA1
	
	EXTI->IMR |= (1<<1);  // Bit[1] = 1  --> Disable the Mask on EXTI 1
	
	EXTI->RTSR |= (1<<1);  // Disable Rising Edge Trigger for PA1
	
	EXTI->FTSR &= ~(1<<1);  // Enable Falling Edge Trigger for PA1
	
	/* Set interrupt priority */
	IRQn_Type IRQn = EXTI1_IRQn;	
	uint32_t prioritygroup = 0x00U;
	uint32_t PreemptPriority = 1;
	uint32_t SubPriority = PRIORITY_BUTTON_INTERRUPTION;
	prioritygroup = NVIC_GetPriorityGrouping();
	NVIC_SetPriority(IRQn, NVIC_EncodePriority(prioritygroup, PreemptPriority, SubPriority));
	
	NVIC_EnableIRQ (EXTI1_IRQn);  // Enable Interrupt
}

void TIM2_IRQHandler(void){
	
	if (button_pressed){
		power_on = !power_on;
		button_pressed = 0;
	}
	
   //PrintConsole(INFO, "\r\n System Mode: %s",power_on?"ON":"OFF");

	
    // Don't forget to clear the interrupt flag
    TIM2->SR &= ~TIM_SR_UIF;
}

void EXTI1_IRQHandler(void){
	/*************>>>>>>> STEPS FOLLOWED <<<<<<<<************
	
	1. Check the Pin, which trgerred the Interrupt
	2. Clear the Interrupt Pending Bit
	
	********************************************************/
	
	if (EXTI->PR & (1<<1))    // If the PA1 triggered the interrupt
	{
		
		button_pressed = 1;
		
		EXTI->PR |= (1<<1);  // Clear the interrupt flag by writing a 1 
	}
}

ClockConfig getSystemClockSpeed(bool show) {
	
	//The function is not entirely correct, some problem with the prescaler apb1
	
    ClockConfig config;

    // Read the HSE clock configuration
    if (RCC->CR & RCC_CR_HSERDY) {
		config.hseFrequency = HSE_VALUE;
	 } else {
		config.hseFrequency = 0;
}

    // Read the APB1 and APB2 clock configuration
    uint32_t ahbPrescaler = ((RCC->CFGR & RCC_CFGR_HPRE) >> 4) + 1;
	 uint32_t apb1Prescaler = convertPPRE((RCC->CFGR & RCC_CFGR_PPRE1) >> 10);
	 uint32_t apb2Prescaler = ((RCC->CFGR & RCC_CFGR_PPRE2) >> 13) + 1;

    // Read the RCC clock configuration registers to determine the system clock speed
    uint32_t systemClockSpeed = 0;

    // Check the PLL configuration
    if ((RCC->CR & RCC_CR_PLLON) && (RCC->CR & RCC_CR_PLLRDY)) {
        config.pllm = (RCC->PLLCFGR & RCC_PLLCFGR_PLLM) >> 0;
        config.plln = (RCC->PLLCFGR & RCC_PLLCFGR_PLLN) >> 6;
        config.pllp = ((RCC->PLLCFGR & RCC_PLLCFGR_PLLP) >> 16) & 0x3; // Mask to get the lower 2 bits (PLLP)

		// Decode PLLP to the actual division factor
		uint32_t pllpValue = 0;
		switch (config.pllp) {
			case 0b00: pllpValue = 2; break; // PLLP = 2
			case 0b01: pllpValue = 4; break; // PLLP = 4
			case 0b10: pllpValue = 6; break; // PLLP = 6
			case 0b11: pllpValue = 8; break; // PLLP = 8
		}
		  config.pllp= pllpValue;
        uint32_t pllSource = (RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC);

        if (pllSource == RCC_PLLCFGR_PLLSRC_HSE) {
            // HSE source
            systemClockSpeed = (config.hseFrequency / config.pllm) * config.plln / config.pllp;
        } else {
            // HSI source (uncomment if needed)
            // systemClockSpeed = (HSI_VALUE / config.pllm) * config.plln / config.pllp;
        }
    } 
    else if (RCC->CFGR & RCC_CFGR_SWS_PLL) {
        // PLL is not used, but PLL source was selected
        // systemClockSpeed = HSI_VALUE;
    } else if (RCC->CFGR & RCC_CFGR_SWS_HSE) {
        // HSE source
        systemClockSpeed = config.hseFrequency;
    } else {
        // HSI source (uncomment if needed)
        // systemClockSpeed = HSI_VALUE;
    }

    config.systemFrequency = systemClockSpeed;
    config.apb1Frequency = systemClockSpeed / apb1Prescaler;
	 config.apb1timFrequency = config.apb1Frequency * 2;
    config.apb2Frequency = systemClockSpeed / apb2Prescaler;

    // Print the clock configurations
	 if (show){
		 SendString("\r");
		 PrintConsole(WARNING, "The system Clock is configured as follow :\n\r");
		 
		 PrintConsole(WARNING, "HSE Frequency: %lu Hz\n\r", 				config.hseFrequency);
		 PrintConsole(WARNING, "APB1 Prescaler: %lu \n\r", 				apb1Prescaler);
		 PrintConsole(WARNING, "APB2 Prescaler: %lu \n\r",					apb2Prescaler);
		 PrintConsole(WARNING, "APB1 Frequency: %lu Hz\n\r", 				config.apb1Frequency);
		 PrintConsole(WARNING, "APB1 	Timer Frequency: %lu Hz\n\r",		config.apb1timFrequency);
		 PrintConsole(WARNING, "APB2 Frequency: %lu Hz\n\r", 				config.apb2Frequency);
		 PrintConsole(WARNING, "System Frequency: %lu Hz\n\r", 			config.systemFrequency);
		 PrintConsole(WARNING, "PLLM: %lu\n\r", 								config.pllm);
		 PrintConsole(WARNING, "PLLN: %lu\n\r", 								config.plln);
		 PrintConsole(WARNING, "PLLP: %lu\n\r", 								config.pllp);
	 }
	 
	 return config;
 }


 void Timer2InterruptEnable(void){
	
	//11. Enable the timer
   TIM2->CR1 |= TIM_CR1_CEN;
		
}

void Timer2InterruptDisable(void){
    // Clear the timer's interrupt enable bit
    TIM2->CR1 &= ~TIM_CR1_CEN;
}
 
void Timer2InterruptInit(float oscillation_button_time){
	
	/**
	*	The role of this timer interrupt is to prevent the hardware oscillation of the button
	*	To influence his state and get a stable On/Off push-button
	
	
	* The update event happens when:

		Overflow or underflow regarding the repetition counter value and if UDIS is 0 (update event not disabled) in the TIMx_CR1.
		CNT is reinitialized by software using the UG bit in the TIMx_EGR if URS and UDIS are 0 in the TIMx_CR1.
	
	**/
	
	// https://electroprojecthub.com/blog/stm32-timer-interrupts/   -> for more information on the configuration
	
	int arr = getSystemClockSpeed(false).apb1timFrequency * oscillation_button_time;
	double timer_freq = getSystemClockSpeed(false).apb1timFrequency / (arr+1);
	PrintConsole(INFO, "\r\n Freq interrupt timer for push-button osci removal: %lf",timer_freq);
	double timer_Ts = 1/timer_freq;
	PrintConsole(INFO, "\r\n Expected time of oscillation: %lf",timer_Ts);
	

	//! 1. Enable the timer clock source
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

	//! 2. Set the prescaler
	TIM2->PSC = 0;

	//! 3. Set the auto reload
	TIM2->ARR = arr - 1;

	//! 4. Set the counter mode
	TIM2->CR1 &= ~TIM_CR1_DIR; // Configure as upcounter (clear the DIR bit)
	
	// 5. Set the Update request source 
	TIM2->CR1 |= TIM_CR1_URS;    // Only counter overflow/underflow generates an update interrupt and not manually 
	//(prevent user to change the sampling period)
	
	// 6. Enable auto-reload preload
   TIM2->CR1 |= TIM_CR1_ARPE;  // ARR register buffered, changes made to the ARR register take effect only at the next update event

 	// 7. Transfer the content of the preload registers to buffers
	/** 
	*	 Generating an update event ensures that the buffered values of the prescaler and auto-reload register 
	*   are updated and synchronized with the hardware registers used by the timer
	**/
	TIM2->EGR |= TIM_EGR_UG;
	
	// 8. Enable the update interrupt
	TIM2->DIER |= TIM_DIER_UIE;
	TIM2->SR &= ~TIM_SR_UIF;    // Clear the interrupt  -> good practice but not mandatory
	
	
	//9. Configure the NVIC to run a callback function when interrupt occur
	/* Set interrupt priority */
	IRQn_Type IRQn = TIM2_IRQn;	
	uint32_t prioritygroup = 0x00U;
	uint32_t PreemptPriority = 1;
	uint32_t SubPriority = PRIORITY_OSCILLATION_TIMER;
	prioritygroup = NVIC_GetPriorityGrouping();
	NVIC_SetPriority(IRQn, NVIC_EncodePriority(prioritygroup, PreemptPriority, SubPriority));

	//10. Enable interrupt
	NVIC_EnableIRQ(IRQn);

	//! Optional: Stops the timer when debug is halted
	DBGMCU->APB1FZ |= DBGMCU_APB1_FZ_DBG_TIM2_STOP;
		
}

 int convertPPRE(int ppre_bits) {
    switch (ppre_bits) {
        case 0b000:
            return 1; // HCLK not divided
        case 0b100:
            return 2; // HCLK divided by 2
        case 0b101:
            return 4; // HCLK divided by 4
        case 0b110:
            return 8; // HCLK divided by 8
        case 0b111:
            return 16; // HCLK divided by 16
        default:
            return -1; // Invalid bits
    }
}
