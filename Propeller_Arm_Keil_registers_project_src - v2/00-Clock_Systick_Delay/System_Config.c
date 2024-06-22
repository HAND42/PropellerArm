#include "System_Config.h"


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

volatile uint8_t sampling_byte = 0xFE;  
volatile uint8_t button_state = 1;

volatile uint32_t msTicks = 0; // Counter for milliseconds

void SysTick_Init(void) {
    // Set reload register to interrupt every 1ms
    SysTick->LOAD = (SystemCoreClock / 1000) - 1; // 100MHz / 1000 = 100000 - 1

    // Set the SysTick counter value to 0
    SysTick->VAL = 0;

    // Configure SysTick: Enable SysTick, Enable SysTick interrupt, Clock source = processor clock
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
}

uint32_t Get_Timestamp(void) {
    uint32_t currentTicks;
    __disable_irq(); // Disable interrupts to ensure atomic read
    currentTicks = msTicks;
    __enable_irq(); // Re-enable interrupts
    return currentTicks/6.251; //correct the difference in configuration of the clock frequency
}

void SysTick_Handler(void) {
    msTicks++; // Increment millisecond counter
}

void delay_systick(uint32_t time) {
    uint32_t startTick = Get_Timestamp();
    while ((Get_Timestamp() - startTick) < time);
}
  
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
	3. Configure the PULL UP/ PULL DOWN According to our requirement
	
	********************************************************/
	
	// 1. Enable GPIO Clock
	RCC->AHB1ENR |=  (1<<0);  // Enable GPIOA clock
	
	// 2. Set the required Pin in the INPUT Mode
	GPIOA->MODER &= ~(3<<0);  // Bits (1:0) = 0:0  --> PA0 in Input Mode
	
	// 3. Configure the PULL UP
	GPIOA->PUPDR |=  (1<<1);  // Bits (1:0) = 1:0  --> PA0 is in Pull down mode
}

void TIM2_IRQHandler(void){
	
	button_state = GPIOA->IDR & GPIO_IDR_ID0;
	
	// Update the sampling byte
   sampling_byte = (sampling_byte << 1) | button_state;

   // Check for a stable falling edge
   if (sampling_byte == 0xFE) {
		power_on = !power_on;
   }	
	
    // Don't forget to clear the interrupt flag
    TIM2->SR &= ~TIM_SR_UIF;
}

ClockConfig getSystemClockSpeed(int show) {
	
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
		 PrintConsole(WARNING, "	****** System Clock Speed: %lu Hz  ******\n\r",config.systemFrequency);
		 PrintConsole(WARNING, "HSE   Frequency:  %lu Hz\n\r", 				config.hseFrequency);
		 PrintConsole(WARNING, "APB1  Prescaler:  %lu \n\r", 				   apb1Prescaler);
		 PrintConsole(WARNING, "APB2  Prescaler: 	%lu \n\r",					apb2Prescaler);
		 PrintConsole(WARNING, "APB1  Periph_Frequency:   %lu Hz\n\r", 				config.apb1Frequency);
		 PrintConsole(WARNING, "APB1 	Timer_Frequency:    %lu Hz\n\r",		         config.apb1timFrequency);
		 PrintConsole(WARNING, "APB2  Periph_Frequency:   %lu Hz\n\r", 				config.apb2Frequency);
		 PrintConsole(WARNING, "APB2  Timer_Frequency:    %lu Hz\n\r", 				config.apb2Frequency);
		 PrintConsole(WARNING, "PLLM: %lu\n\r", 					config.pllm);
		 PrintConsole(WARNING, "PLLN: %lu\n\r", 					config.plln);
		 PrintConsole(WARNING, "PLLP: %lu\n\r", 					config.pllp);
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
	PrintConsole(INFO, "\r\nFreq interrupt timer for push-button osci removal: %lf",timer_freq);
	double timer_Ts = 1/timer_freq;
	PrintConsole(INFO, "\r\nExpected time of oscillation: %lf s",timer_Ts);
	

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
//	IRQn_Type IRQn = TIM2_IRQn;	
//	uint32_t prioritygroup = 0x00U;
//	uint32_t PreemptPriority = 2;
//	uint32_t SubPriority = PRIORITY_OSCILLATION_TIMER;
//	prioritygroup = NVIC_GetPriorityGrouping();
//	NVIC_SetPriority(IRQn, NVIC_EncodePriority(prioritygroup, PreemptPriority, SubPriority));

	NVIC_SetPriority(TIM2_IRQn, IT_TIM2_IRQ_PRIORITY);

	//10. Enable interrupt
	NVIC_EnableIRQ(TIM2_IRQn);

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
