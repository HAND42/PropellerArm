#include "Angles_Estimation.h"


float T = 0.08;  // The time taken to acquire accelerometer data is approximately 0.02977 seconds.
float pre_theta = 0;
float pre_pre_theta = 0;
float theta = 0;

float theta_acc = 0;
float pre_theta_acc = 0;
float pre_pre_theta_acc = 0;

float gyro_y = 0;
float pre_gyro_y = 0;
float pre_pre_gyro_y = 0;

void Timer3InterruptInit(float sampling_period){
	
	// We will initialize the timer3 as an update interrupt
	// This subroutine is utilized to independently update the angle of the propeller arm relative to a reference angle.
	
	/**
	The update event happens when:

    Overflow or underflow regarding the repetition counter value and if UDIS is 0 (update event not disabled) in the TIMx_CR1.
    CNT is reinitialized by software using the UG bit in the TIMx_EGR if URS and UDIS are 0 in the TIMx_CR1.
	
	**/
	
	// https://electroprojecthub.com/blog/stm32-timer-interrupts/   -> for more information on the configuration
	
	int arr = getSystemClockSpeed().apb1Frequency * sampling_period;
	double timer_freq = getSystemClockSpeed().apb1Frequency / (arr+1);
	PrintConsole(INFO, "\r\n Freq interrupt timer: %lf",timer_freq);
	double timer_Ts = 1/timer_freq;
	PrintConsole(INFO, "\r\n Sampling time: %lf",timer_Ts);
	

	//! 1. Enable the timer clock source
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

	//! 2. Set the prescaler
	TIM3->PSC = 0;

	//! 3. Set the auto reload
	TIM3->ARR = arr - 1;

	//! 4. Set the counter mode
	TIM3->CR1 &= ~TIM_CR1_DIR; // Configure as upcounter (clear the DIR bit)
	
	// 5. Set the Update request source 
	TIM3->CR1 |= TIM_CR1_URS;    // Only counter overflow/underflow generates an update interrupt and not manually 
	//(prevent user to change the sampling period)
	
	// 6. Enable auto-reload preload
   TIM3->CR1 |= TIM_CR1_ARPE;  // ARR register buffered, changes made to the ARR register take effect only at the next update event

 	// 7. Transfer the content of the preload registers to buffers
	/** 
	*	 Generating an update event ensures that the buffered values of the prescaler and auto-reload register 
	*   are updated and synchronized with the hardware registers used by the timer
	**/
	TIM3->EGR |= TIM_EGR_UG;
	
	// 8. Enable the update interrupt
	TIM3->DIER |= TIM_DIER_UIE;
	TIM3->SR &= ~TIM_SR_UIF;    // Clear the interrupt  -> good practice but not mandatory
	
	
	//9. Configure the NVIC to run a callback function when interrupt occur
	/* Set interrupt priority */
	IRQn_Type IRQn = TIM3_IRQn;	
	uint32_t prioritygroup = 0x00U;
	uint32_t PreemptPriority = 1;
	uint32_t SubPriority = 0;
	prioritygroup = NVIC_GetPriorityGrouping();
	NVIC_SetPriority(IRQn, NVIC_EncodePriority(prioritygroup, PreemptPriority, SubPriority));

	//10. Enable interrupt
	NVIC_EnableIRQ(IRQn);

	//! Optional: Stops the timer when debug is halted
	DBGMCU->APB1FZ |= DBGMCU_APB1_FZ_DBG_TIM3_STOP;
	
}

void Timer3InterruptEnable(void){
	
	//11. Enable the timer
   TIM3->CR1 |= TIM_CR1_CEN;
		
}

void Timer3InterruptDisable(void){
    // Clear the timer's interrupt enable bit
    TIM3->CR1 &= ~TIM_CR1_CEN;
}


void TIM3_IRQHandler( void ){
	if ( TIM3->SR & TIM_SR_UIF )
	{
		TIM3->SR &= ~TIM_SR_UIF;

		Led_Trigger(BLUE, 0);
		DisplayAxisValues();
		//PrintConsole(INFO, "\r\nTheta: %lf",theta); //this is the theta estimation
		//---> Compute the reference theta_ref
		TiltCompensatedThetaEstimation();
		
	}
}

float TiltCompensatedThetaEstimation(void){
	
   theta_acc = GetThetaAcc_rad();
   gyro_y = GetThetaGyro_rad();
	
	theta = Ko*pre_theta + T*gyro_y + T*Ko*(theta_acc - pre_theta_acc) ;
	theta = theta / (Ko + T);
	
	pre_theta = theta;
	pre_theta_acc = theta_acc;
	
	return theta;
}

float GetRollTurnRateGyro(void){
	
	short Y;
	GetGyroY(&Y);
	float gyroY = Y;
	return gyroY;
	
}

float GetThetaGyro_rad(void){
	short Y;
	GetGyroY(&Y);
	float gyroY= pre_gyro_y + Y*T;
	return gyroY;
}

float GetThetaAcc_rad(void){
	float accX, accY, accZ;
	GetAccelerometerValuesInMS2(&accX, &accY, &accZ);
	return asin(accX/G);
}

float GetThetaAcc_deg(void){
	float accX, accY, accZ;
	GetAccelerometerValuesInMS2(&accX, &accY, &accZ);
	return asin(accX/G)*180/M_PI;
}


float GetPhiAcc_rad(void){
	float accX, accY, accZ;
	GetAccelerometerValuesInMS2(&accX, &accY, &accZ);
	return atan (-(accY)/(accZ));
}

float GetPhiAcc_deg(void){
	float accX, accY, accZ;
	GetAccelerometerValuesInMS2(&accX, &accY, &accZ);
	return atan (-(accY)/(accZ))*180/M_PI;
}


void GetAccelerometerValuesInMS2(float* accX, float* accY, float* accZ){
	short accelX, accelY, accelZ;
	GetAccelerometerValues(&accelX, &accelY, &accelZ);
	
	//The output of the accelerometer is mG/LSB = milli-G's / Least Significant bit
	//The LSM303DLHC outputs a 16 bit value for the accelerometer readings
	//16 bits equals 65,535 and -2 to +2, this would be a total of 4g
	
	*accX = accelX*0.061;    //4,000 MilliGs / 65,535 because FS = +- 2g
	*accX = *accX/1000;     //Convert the MilliG in G.
	*accX = *accX*G;			//Convert G in M/S2
	
	*accY = accelY*0.061;   //4,000 MilliGs / 65,535 because FS = +- 2g
	*accY = *accY/1000;		//Convert the MilliG in G.
	*accY = *accY*G;			//Convert G in M/S2
	
	*accZ = accelZ*0.061;   //4,000 MilliGs / 65,535 because FS = +- 2g
	*accZ = *accZ/1000;		//Convert the MilliG in G.
	*accZ = *accZ*G;			//Convert G in M/S2
	
}



