#include "System_Config.h"
#include "Delay.h"
#include "Led.h"

#include "Accelerometer.h"
#include "Angles_Estimation.h"
#include "Gyro.h"
#include "DisplayData.h"

#include "i2c-lcd.h"
#include "motor_pwm.h"
#include "Potentiometer.h"

#include "RegisterAddresses.h"
#include <stdio.h>

/* General information

The system Clock is configured as follow :
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
 
 
UART2 communication :
	// USART2 TX --> PA2 // Write
	// USART2 RX --> PA3 // Read

Gyrometer L3GD20 pins are connected to the STM32F411 using:
//    CS  --> PE3   // Chip Select
//    SPC --> PA5   // Clock
//    SDI --> PA7   // Data In
//    SDO --> PA6   // Data Out

Accelerometer LSM303DLHC pins are connected to the STM32F411 using
// the following pins for I2C1 communication:
// SCL  --> PB6
// SDA  --> PB9

LCD display connected to the same I2C1 bus pins 
// SCL  --> PB6
// SDA  --> PB9

Power mode interrupt button pins 
// Pull-Up Mode --> PA1 & GND

LED Power On/Off
//  LED ON -> PD15
//  LED OFF ->PD14

PWM controlling the brushless motor velocity
// PWM Left Motor -> PD13
// PWM Right Motor -> PD12 or the other way around check /!\

Potentiometer ADC pin
// SIG -> PA4
// NC -> Not connected

BAUD RATE : 115 200


SLAVE_ADDRESS_LCD 0x4E

*/


int main (void)
{
	
	SysClockConfig (); 		 // Configuration of the clock frequency -> set to the maximum frequency 100MHz
	Uart2Init();
	//Uart1Init();   			 // Try to implement the usart1 in order to send QTdata and receive commands only for/by the QT interface
	TIM5Config ();           // Configuration of the Timer for generating delays
	LED_GPIO_Config ();		 // PowerMode light indicator / Initilization LED 
	Sys_PowerMode_Config();	 // Configuration of the power mode (Red for power off and Blue for power on)
	Testing_Toggle_4Led();	 // Testing the 4 LED Yellow Orange Red & Blue
	
	Timer3InterruptInit(T);   // Initialization of the Interrupt Tim3 for the PID controller sampling period (Ts=0.02s) config
	
	
	GyroInit(); 				 // Initialization of the Gyroscope
	AccelerometerInit();		 // Initialization of the Accelerometer
	
	//lcd_init ();				 // Initialization of the LCD Communication
	
	PrintConsole(DEFAULT,"\x0C");
	
	//Initialize the LCD, if it does not print the "Init done Press start" it is required to reset the mcu.
	//------------------------------------------------------------------------------------------------------
//	lcd_clear();
//	lcd_put_cur (0,0);
//	lcd_send_string("Init done");
//	lcd_put_cur (1,0);
//	lcd_send_string("Press Start");
//	delay_ms(1000);
	//------------------------------------------------------------------------------------------------------
	
	o_MotorPWM_Initialization_o();
	//o_Potentiometer_Initialization_o();
	float value = 0.0;
	
	
	/*Show my clock speed configuration on Putty*/
	getSystemClockSpeed();

	while(1)
	{
		//Avoid with the counter the oscillations frequencies followed by the press of the button
		power_on = 1; //to remove when stop debugging
		if (count>0){
			count--;
		}
		if (power_on){
			Power_On(); //to remove when stop debugging
			//MotorPWM_Set(150,150);
			
			TIM4->CCR1 = 1000;
			TIM4->CCR2 = 1000;
			
			// Get Potentiometer value
			//value = o_Read_Potentiometer_o(4);
			
			//Timer3InterruptEnable();
//			SendString("\r\n");
//			PrintConsole(INFO, "Analog read: %lf",value);
			//delay_ms(500);
			
			/*Displays the values of the accelerometer and gyrometer data*/
			//DisplayAxisValues();
			//theta = HomeMadeThetaEstimation();
			//PrintConsole(INFO, "theta: %lf",theta);
		}
		
		else {
			Power_Off();
			//lcd_clear();
			delay_ms(200);
		}
		
	}

	return 0;
}
