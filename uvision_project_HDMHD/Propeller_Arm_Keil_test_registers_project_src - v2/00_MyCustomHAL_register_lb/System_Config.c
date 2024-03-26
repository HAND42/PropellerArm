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
  

  
bool power_on=0;
int count=0;
  
  
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
	
	NVIC_SetPriority (EXTI1_IRQn, 0);  // Set Priority
	
	NVIC_EnableIRQ (EXTI1_IRQn);  // Enable Interrupt
}

void EXTI1_IRQHandler(void)
{
	/*************>>>>>>> STEPS FOLLOWED <<<<<<<<************
	
	1. Check the Pin, which trgerred the Interrupt
	2. Clear the Interrupt Pending Bit
	
	********************************************************/
	
	if (EXTI->PR & (1<<1))    // If the PA1 triggered the interrupt
	{
		if (count==0){
		power_on = !power_on;
		count=3;
		}
		EXTI->PR |= (1<<1);  // Clear the interrupt flag by writing a 1 
	}
}

void getSystemClockSpeed(void) {
	
	//The function is not entirely correct, some problem with the prescaler apb1
	
    ClockConfig config;

    // Read the HSE clock configuration
    config.hseFrequency = (RCC->CR & RCC_CR_HSERDY) ? HSE_VALUE : 0;

    // Read the APB1 and APB2 clock configuration
    uint32_t ahbPrescaler = ((RCC->CFGR & RCC_CFGR_HPRE) >> 4) + 1;
    uint32_t apb1Prescaler = ((RCC->CFGR & RCC_CFGR_PPRE1) >> 10) + 1;
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
    config.apb2Frequency = systemClockSpeed / apb2Prescaler;

    // Print the clock configurations
	 PrintConsole(WARNING, "The system Clock is configured as follow :\n\r");
	 
    PrintConsole(WARNING, "HSE Frequency: %lu Hz\n\r", 		config.hseFrequency);
	 PrintConsole(WARNING, "APB1 Prescaler: %lu Hz\n\r", 		apb1Prescaler);
    PrintConsole(WARNING, "APB2 Prescaler: %lu Hz\n\r",		apb2Prescaler);
    PrintConsole(WARNING, "APB1 Frequency: %lu Hz\n\r", 		config.apb1Frequency);
    PrintConsole(WARNING, "APB2 Frequency: %lu Hz\n\r", 		config.apb2Frequency);
    PrintConsole(WARNING, "System Frequency: %lu Hz\n\r", 	config.systemFrequency);
    PrintConsole(WARNING, "PLLM: %lu\n\r", 						config.pllm);
    PrintConsole(WARNING, "PLLN: %lu\n\r", 						config.plln);
    PrintConsole(WARNING, "PLLP: %lu\n\r", 						config.pllp);
 }

