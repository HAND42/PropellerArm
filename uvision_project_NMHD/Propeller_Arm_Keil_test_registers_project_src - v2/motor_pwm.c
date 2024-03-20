#include "motor_pwm.h"



void MotorPWM_EPort_PinSetup(void){
	
	// Enable the clock for GPIOE
    RCC->AHB1ENR |= (1 << 4);  // 4 corresponds to GPIOE

    // Configure pins PE13 and PE14 as alternate function
    GPIOE->MODER |= ((2 << 26) | (2 << 28));  // Set PE13 and PE14 to alternate function mode


    // Set pins to use alternate function mode
    GPIOE->AFR[1] |= (1<<20); // GPIO_AF1_TIM1 for Pin 13
    GPIOE->AFR[1] |= (1<<24); // GPIO_AF1_TIM1 for Pin 14

    // Configure pins as push-pull outputs
    GPIOE->OTYPER &= ~(1 << 14 | 1 << 13);

    // No pull-up or pull-down
    GPIOE->PUPDR &= ~(3 << 26 | 3 << 28);

    // Set pins to high-speed mode
    GPIOE->OSPEEDR |= (3 << 26 | 3 << 28);
}

void MotorPWM_BPort_PinSetup(void){
	
	// Enable the clock for GPIOB
    RCC->AHB1ENR |= (1 << 1);  // 1 corresponds to GPIOB

    // Configure pins PE13 and PE14 as alternate function
    GPIOB->MODER |= ((2 << 0) | (2 << 2));  // Set PB0 and PB1 to alternate function mode


    // Set pins to use alternate function mode
    GPIOB->AFR[0] |= (2<<0); // GPIO_AF2_TIM3 for Pin 0
    GPIOB->AFR[0] |= (2<<4); // GPIO_AF2_TIM3 for Pin 1

    // Configure pins as push-pull outputs
    GPIOB->OTYPER &= ~(1 << 1 | 1 << 0);

    // No pull-up or pull-down
    GPIOB->PUPDR &= ~(3 << 2 | 3 << 0);

    // Set pins to high-speed mode
    GPIOB->OSPEEDR |= (3 << 2 | 3 << 0);
}

void MotorPWM_Timer1Setup(void){
	//
	// Enable the clock for TIM3
    RCC->APB2ENR |= 1<<0;

    // Set the prescaler
    TIM1->PSC = 999;

    // Configure the timer in up-counting mode
    TIM1->CR1 &= ~TIM_CR1_DIR;

    // Set the period for a 50Hz PWM signal
    TIM1->ARR = 1999; // 50 Hz

    // Update event source (UEVS) to generate update interrupt
    //TIM1->EGR |= TIM_EGR_UG;

    // Enable TIM3
    TIM1->CR1 |= TIM_CR1_CEN;
}

void MotorPWM_Timer3Setup(void){
	//
	// Enable the clock for TIM3
    RCC->APB1ENR |= 1<<1;

    // Set the prescaler
    TIM3->PSC = 999;

    // Configure the timer in up-counting mode
    TIM3->CR1 &= ~TIM_CR1_DIR;

    // Set the period for a 50Hz PWM signal
    TIM3->ARR = 1999; // 50 Hz

    // Update event source (UEVS) to generate update interrupt
    //TIM3->EGR |= TIM_EGR_UG;

    // Enable TIM3
    TIM3->CR1 |= TIM_CR1_CEN;
}

void MotorPWM_TIM1_Start(double dutyCyclePercent) {
     // Ensure the duty cycle is within the valid range (0-100)
    if (dutyCyclePercent > 100) {
        dutyCyclePercent = 100;
    }
	 
	 dutyCyclePercent = 50.0;

    // Enable the clock for TIM1
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;

    // Set the timer mode to PWM mode 1 (Set on Compare Match) for channel 3 (PE13)
    TIM1->CCMR2 = (TIM1->CCMR2 & ~TIM_CCMR2_OC3M) | TIM_CCMR2_OC3M_0;

    // Calculate the pulse length based on duty cycle
    uint16_t pulse = (uint16_t)((TIM1->ARR + 1) * (dutyCyclePercent / 100.0)) - 1;

    // Set the duty cycle for channel 3 (PE13)
    TIM1->CCR3 = pulse; // Duty cycle for Channel 3 (PE13)

    // Enable preload for CCR3: Set to 1, it changes the duty cycle in the active register at each update event and not immediately.
    // TIM1->CCMR2 |= TIM_CCMR2_OC3PE;

    // Start the timer
    TIM1->CR1 |= TIM_CR1_CEN;
}


void MotorPWM_TIM3_Start(double dutyCyclePercent) {
     // Ensure the duty cycle is within the valid range (0-100)
    if (dutyCyclePercent > 100) {
        dutyCyclePercent = 100;
    }
	 
	 dutyCyclePercent = 50.0;

    // Enable the clock for TIM3
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

    // Set the timer mode to PWM mode 1 (Set on Compare Match) for channel 3 (PE13)
    TIM3->CCMR2 = (TIM3->CCMR2 & ~TIM_CCMR2_OC3M) | TIM_CCMR2_OC3M_0;

    // Calculate the pulse length based on duty cycle
    uint16_t pulse = (uint16_t)((TIM3->ARR + 1) * (dutyCyclePercent / 100.0)) - 1;

    // Set the duty cycle for channel 3 (PE13)
    TIM3->CCR3 = pulse; // Duty cycle for Channel 3 (PE13)

    // Enable preload for CCR3: Set to 1, it changes the duty cycle in the active register at each update event and not immediately.
    // TIM3->CCMR2 |= TIM_CCMR2_OC3PE;

    // Start the timer
    TIM3->CR1 |= TIM_CR1_CEN;
}