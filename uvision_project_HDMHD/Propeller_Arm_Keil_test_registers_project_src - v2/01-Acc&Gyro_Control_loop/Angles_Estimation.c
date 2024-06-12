#include "Angles_Estimation.h"

Gyro_data gyro_data = {0,0,0,0};
Acc_data acc_data = {0,0,0} ;
Theta_estim theta_data = {0,0,0,0,0};

float command = 0;
float PWMd=0; float PWMg =0;

float theta_ref = 0;

void Timer3InterruptInit(float sampling_period){
	
	// We will initialize the timer3 as an update interrupt
	// This subroutine is utilized to independently update the angle of the propeller arm relative to a reference angle.
	
	/**
	The update event happens when:

    Overflow or underflow regarding the repetition counter value and if UDIS is 0 (update event not disabled) in the TIMx_CR1.
    CNT is reinitialized by software using the UG bit in the TIMx_EGR if URS and UDIS are 0 in the TIMx_CR1.
	
	**/
	
	// https://electroprojecthub.com/blog/stm32-timer-interrupts/   -> for more information on the configuration
	
	float tim_frq = getSystemClockSpeed(false).apb1timFrequency;
	int arr = tim_frq * sampling_period;
	double interrupt_freq = tim_frq / (arr+1);
	PrintConsole(INFO, "\r\n Freq interrupt timer %.2f Hz based on the clk_tim_frq %.2f Hz",interrupt_freq,tim_frq);
	double timer_Ts = 1/interrupt_freq;
	PrintConsole(INFO, "\r\n Sampling time: %.2f s",timer_Ts);
	

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
//	IRQn_Type IRQn = TIM3_IRQn;	
//	uint32_t prioritygroup = 0x00U;
//	uint32_t PreemptPriority = 3;
//	uint32_t SubPriority = 2;
//	prioritygroup = NVIC_GetPriorityGrouping();
//	NVIC_SetPriority(IRQn, NVIC_EncodePriority(prioritygroup, PreemptPriority, SubPriority));
	
	NVIC_SetPriority(TIM3_IRQn, IT_TIM3_CONTROL_LOOP);

	//10. Enable interrupt
	NVIC_EnableIRQ(TIM3_IRQn);

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

	Control_Loop_ThetaEstimation_CommandThrottle();
	
	TIM3->SR &= ~TIM_SR_UIF;


}

void Control_Loop_ThetaEstimation_CommandThrottle(void){
	//Led_Trigger(BLUE, 0);
	//DisplayAxisValues();
	//---> Compute the reference theta_ref			
	theta_ref = 0;
			
	PrintConsole(INFO, "\r\nTheta: %.2f deg",theta_data.theta*180/M_PI); //this is the theta estimation
	theta_data.biais = (acc_data.theta_acc - theta_data.theta)*Ki*T - theta_data.pre_biais;
	theta_data.pre_biais = theta_data.biais;
	command = K2 * (K1 * (theta_ref - theta_data.theta) - theta_data.biais - gyro_data.pitchTurnRate_radps);

	PWMd =  ((Fmoy + command*l1/I)/(1000*G) +176.21)/0.17;
	PWMg =  ((Fmoy - command*l1/I)/(1000*G) +176.21)/0.17;
	MotorPWM_Set(PWMg, PWMd);
	PrintConsole(INFO, "\r\nPWMg: %lf and PWMd: %lf",PWMg,PWMd);
			
	TiltCompensatedThetaEstimation();
	GetAccData_read_fast();
	GetGyroData_read_fast();
}

void TiltCompensatedThetaEstimation(void){
	
	// Validate sensor data
   if (isnan(acc_data.theta_acc) || isnan(gyro_data.gyro_y)) {
      PrintConsole(ERROR, "Sensor data invalid: theta_acc = %.2f, gyro_y = %.2f", acc_data.theta_acc, gyro_data.gyro_y);
   }
	
	theta_data.theta = (theta_data.pre_theta + T*(gyro_data.gyro_y - gyro_data.pre_gyro_y) + T*Ko*acc_data.theta_acc)/ (Ko*T + 1);	
}

void TiltCompensatedThetaEstimation2(void){
	
	
	// Validate sensor data
   if (isnan(acc_data.theta_acc) || isnan(gyro_data.gyro_y)) {
      PrintConsole(ERROR, "Sensor data invalid: theta_acc = %.2f, gyro_y = %.2f", acc_data.theta_acc, gyro_data.gyro_y);
   }
	
	theta_data.theta = (  theta_data.pre_theta* (2/T + Ko) - 1/T * theta_data.pre_pre_theta 
															  + 1/T * (gyro_data.gyro_y - 2*gyro_data.pre_gyro_y + gyro_data.pre_pre_gyro_y) 
															  + (Ko + Ki*T)*acc_data.theta_acc 
															  - Ko  * acc_data.pre_theta_acc 
			  ) / (1/T + Ko + Ki*T);

}

float GetRollTurnRateGyro(void){
	
	short Y;
	GetGyroY(&Y);
	float gyroY = Y;
	return gyroY;
	
}

float GetThetaGyro_rad(void){
	return gyro_data.gyro_y*180/M_PI;
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

void GetAccData_read_fast(void){
	short accelX = ((ReadFromAccelerometer(0x29) << 8) | ReadFromAccelerometer(0x28));
	
	// 4,000 MilliGs / 65,535 because FS = +- 2g, convert MilliG to G, and then to m/s²
	float accX = accelX * 0.061f * G / 1000.0f;
	
	acc_data.pre_pre_theta_acc = acc_data.pre_theta_acc;
	acc_data.pre_theta_acc = acc_data.theta_acc;
	// Compute and return the angle in radians
    acc_data.theta_acc = asinf(accX / G);
}

void GetGyroData_read_fast(void){
	
	gyro_data.pre_pre_gyro_y = gyro_data.pre_gyro_y;
	gyro_data.pre_gyro_y = gyro_data.gyro_y;
	
	short Y = GetAxisValue(0x2A, 0x2B);
	gyro_data.gyro_y= gyro_data.pre_gyro_y + Y*T;
	gyro_data.pitchTurnRate_radps = Y;
	
}



