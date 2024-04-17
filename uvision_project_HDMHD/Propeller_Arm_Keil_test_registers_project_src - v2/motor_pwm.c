#include "motor_pwm.h"

void MotorPWM_PIN_Init(void){
    // Enable clock for GPIOD
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;

    // Alternating functions for pins
    GPIOD->AFR[1] |= (2 << (4 * 4)) | (2 << (4 * 5)); // Pins 12 and 13 are AF2 (TIM4)

    // Set pins
    GPIOD->MODER |= (2 << (12 * 2)) | (2 << (13 * 2)); // Pins 12 and 13 are AF mode
    GPIOD->OTYPER &= ~(GPIO_OTYPER_OT_12 | GPIO_OTYPER_OT_13); // Push-pull
    GPIOD->OSPEEDR |= (3 << (12 * 2)) | (3 << (13 * 2)); // High speed
    GPIOD->PUPDR &= ~((3 << (12 * 2)) | (3 << (13 * 2))); // No pull-up, no pull-down
}

void MotorPWM_TIMER_Init(void) {
    // Enable clock for TIM4
    RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;

    // Set timer prescaler
    TIM4->PSC = 999;

    // Count up
    TIM4->CR1 &= ~TIM_CR1_DIR;

    // Set timer period
    TIM4->ARR = 1999;

    // Initialize TIM4
    TIM4->EGR = TIM_EGR_UG; // Generate update event to load the prescaler value to the counter
    TIM4->CR1 |= TIM_CR1_CEN; // Start timer
}

void MotorPWM_PWM_Init() {
    // PWM mode 2 = Clear on compare match
    TIM4->CCMR1 |= (6 << TIM_CCMR1_OC1M_Pos) | (6 << TIM_CCMR1_OC2M_Pos); // PWM mode 2 for channels 1 and 2

    // Enable output for channels 1 and 2
    TIM4->CCER |= TIM_CCER_CC1E | TIM_CCER_CC2E;

    // Set PWM duty cycle by 0 to initialize the CCR1&2
    TIM4->CCR1 = 0; // pin12
    TIM4->CCR2 = 0; // pin13

    // Enable preload for channels 1 and 2
    TIM4->CCMR1 |= TIM_CCMR1_OC1PE | TIM_CCMR1_OC2PE;

    // Generate update event to load new CCR values
    TIM4->EGR = TIM_EGR_UG;
}

void MotorPWM_ESC_calibration(int min_val,int max_val){
	
	/**
	Before being able to turn the motor it is needed to calibrate the ESC.
	Merci à ce site: https://controllerstech.com/how-to-interface-bldc-motor-with-stm32/

	Here we set the maximum pulse (2ms). The ESC will sound beep indicating it has been calibrated for the maximum pulse.
	Then we send the minimum pulse (1ms) and wait for the ESC to sound the beep again.
	Once it does that it means the calibration is complete.
	**/
	
	TIM4->CCR1 = max_val;
	TIM4->CCR2 = max_val;
	delay_ms(2000);
	TIM4->CCR1 = min_val;
	TIM4->CCR2 = min_val;
	delay_ms(1000);
	
}

 

void o_MotorPWM_Initialization_o(void){
	
	/*
	* To be called in the main to initialize the pwm signals, calibrates the ESC
	*/
	
	MotorPWM_PIN_Init();
	MotorPWM_TIMER_Init();
	MotorPWM_PWM_Init();
	
	int min_val = 100;   //-> 1ms
	int max_val = 200; 	//-> 2ms
	
	MotorPWM_ESC_calibration(min_val, max_val);
	delay_ms(10);
	MotorPWM_Set(150,150);  // To put to 0 again after debugging
	
}

int saturate(int input, int min_val, int max_val) {
	/**
		* Function to perform saturation using arithmetic operations
		* Use for the MotorPWM_Set function to ensure the pwm to be between a fix range;
		* No matter what the user provid in input
	**/
	
    return (input < min_val) * min_val + (input >= min_val && input <= max_val) * input + (input > max_val) * max_val;
}

void MotorPWM_Set(int g, int d){
	
	/**
		* /!\ Range of the CCR1&2 PWM to control the motors are from 100 to 200
	**/
	
	// saturate(int input, int min_val, int max_val)
	TIM4->CCR1 = saturate(g,100, 200);
	TIM4->CCR2 = saturate(d,100, 200);
	
} 