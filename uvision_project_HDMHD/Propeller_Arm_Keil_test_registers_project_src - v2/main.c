#include "TwinPropellerArmSTM32F411_configuration.h"

bool power_on = 0;
volatile int time_stamp_init;
volatile int time_of_display;

int main (void)
{
	SysClockConfig (); 		 // Configuration of the clock frequency -> set to Maximum Frequency 100MHz
	SysTick_Init();
	TIM5Config ();           // Configuration of the Timer for generating delays
	Sys_PowerMode_Config();	 // Configuration of the power mode (Red for power off and Blue for power on)
	
	GPIO_Config ();		    // PowerMode light indicator / Initilization LED 
	Testing_Toggle_4Led();	 // Testing the 4 LED Yellow Orange Red & Blue
	
	#ifdef CONFIG_DEBUG_UART_ON
		Uart2Init();
		PrintConsole(DEFAULT,"\x0C");
		PrintConsole(DEFAULT,"\033c");
		PrintConsole(DEFAULT,"\n\n\n\n\n\n\n\r");
		PrintConsole(DEFAULT," ------------------------------------------------------------------------	");
	#endif
	
	/*  Debouncing check time to prevent expected button oscillation  */
	#ifdef CONFIG_DEBOUNCING_BUTTON_ON
		Timer2InterruptInit(Period_IT_Expt_Oscill);
		Timer2InterruptEnable();
	#endif
	
	
	
	/* Implemented the usart6 in order to send QTdata and receive commands only for/by the QT interface */
	#ifdef CONFIG_QT_INTERFACE_UART_ON
		Uart6Init();  
	#endif   			 
	
	/* Initialization of the Gyroscope */
	#ifdef CONFIG_GYRO_INIT_ON
		GyroInit(); 	
	#endif
	
	/* Initialization of the Accelerometer */
	#ifdef CONFIG_ACCELEROMETER_INIT_ON
		AccelerometerInit(); 	
	#endif
	
	/* Timer, Pwm, gpio and ESC calibration */
	#ifdef CONFIG_BLDC_MOTOR_ON
		o_Motor_Initialization_o();
	#endif
	
	/* ADC Init */
	#ifdef CONFIG_POTENTIOMETER_THETA_INPUT_ON
		o_Potentiometer_Initialization_o();
	#endif

	
	#ifdef CONFIG_SLAVE_STM_MASTER_SCREEN_ON
		I2C3_Init();
	#endif
	
	#ifdef CONFIG_BLDC_MOTOR_TESTING_ON && CONFIG_BLDC_MOTOR_ON
		TestingThrustMotor();
		TestingStepInputResponse();
	#endif
	
	#ifdef CONFIG_LCD_ON
		lcd_init ();
	#endif

	
	/*Show my clock speed configuration on Putty*/
	#ifdef SHOW_CLOCK_CONFIG
		getSystemClockSpeed(true);
	#endif
	
	/*Show a little while the acc & gyro data on Putty*/
	#ifdef SHOW_ACC_GYRO_DATA
	
		PrintConsole(INFO,"**** DISPLAY OF ACCELEROMETER AND GYROMETER DATA ****\r\n");
		time_stamp_init = Get_Timestamp();
		time_of_display = 5000;
		
		while (Get_Timestamp() - time_stamp_init < time_of_display){
			DisplayAxisValues();
			delay_us(20);
		}
		PrintConsole(INFO,"\033[7B                 **** END ****                       \r\n");
	#endif
	
	/*Show a little while the potentiometer input theta value on Putty*/	
	//TODO Test the potentiometer
	#ifdef SHOW_POTENTIOMETER_THETA_REF
		PrintConsole(INFO,"**** DISPLAY OF THETA INPUT OF REFERENCE ****\r\n");
		time_stamp_init = Get_Timestamp();
		time_of_display = 5000;
		
		while (Get_Timestamp() - time_stamp_init < time_of_display){
			PrintConsole(INFO, "Analog read theta input: %lf",o_Read_Potentiometer_o(4));
			delay_us(20);
		}
		PrintConsole(INFO,"\r\n                 **** END ****                       \r\n");
		
	#endif

	#ifdef SHOW_COMPUTING_TIME_CONTROL_LOOP
		volatile int before = Get_Timestamp();
		Control_Loop_ThetaEstimation_CommandThrottle();
		Control_Loop_ThetaEstimation_CommandThrottle();
		Control_Loop_ThetaEstimation_CommandThrottle();
		Control_Loop_ThetaEstimation_CommandThrottle();
		volatile int after = Get_Timestamp();
		volatile int computing_time = (after - before)/4;
		
		PrintConsole(INFO,"\r\n Computing time of the control loop function:  %d  ms \r\n", computing_time);	
	#endif
	
		/* Initialization of the Interrupt Tim3 for the cascade control sampling period */
	#ifdef CONFIG_CONTROL_LOOP_ON
		Timer3InterruptInit(Period_IT_Control_loop);
	#endif
	
	Testing_Toggle_4Led();	 // Testing the 4 LED Yellow Orange Red & Blue
	while(1)
	{
		//power_on = 1;
		//Avoid with the counter the oscillations frequencies followed by the press of the button
		if (power_on){
			I3C3_Stop();
			Timer3InterruptEnable();
			Power_On();
			//SendValuesToQT(GetThetaAcc_deg(),0,3.245593);
			//delay_ms(1000);
			
			/*Displays the values of the accelerometer and gyrometer data*/
			//DisplayAxisValues();
	
		}
		
		else {
			Timer3InterruptDisable();
			I2C3_Begin();
			Power_Off();
			//lcd_clear();
			delay_us(20);
		}
		
	}

	return 0;
}