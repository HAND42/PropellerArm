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

the gyrometer L3GD20 pins are connected to the STM32F411:
//    CS  --> PE3   // Chip Select
//    SPC --> PA5   // Clock
//    SDI --> PA7   // Data In
//    SDO --> PA6   // Data Out

the LSM303DLHC (the accelerometer) is connected to the STM32F411 using
// the following pins for I2C1 communication:
// SCL  --> PB6
// SDA  --> PB9

I've choose to connect my LCD display to the same I2C1 bus pins 
// SCL  --> PB6
// SDA  --> PB9

Power mode interrupt pins 
// GND
// Pull-Up Mode --> PA1

Potentiometer pins
// GND 3V 
//Read value --> PA4

BAUD RATE : 115 200


SLAVE_ADDRESS_LCD 0x4E

*/

int main (void)
{
	
	SysClockConfig (); 		 // Configuration of the clock frequency -> set to the maximum frequency 100MHz
	Uart2Init();
	//Uart1Init();   			 // Try to implement the usart1 in order to send QTdata and receive commands only for/by the QT interface
	TIM5Config ();           // Configuration of the Timer for generating delays
	GPIO_Config ();		 // PowerMode light indicator / Initilization LED 
	Sys_PowerMode_Config();	 // Configuration of the power mode (Red for power off and Blue for power on)
	Testing_Toggle_4Led();	 // Testing the 4 LED Yellow Orange Red & Blue
	
	Timer3InterruptInit(T);   // Initialization of the Interrupt Tim3 for the PID controller sampling period (Ts=0.02s) config
	
	
	GyroInit(); 				 // Initialization of the Gyroscope
	AccelerometerInit();		 // Initialization of the Accelerometer
	
	
	Timer3InterruptInit(T);
	Timer3InterruptEnable();
	
	o_Motor_Initialization_o();
	o_Potentiometer_Initialization_o();
	
	
	GyroInit();
	AccelerometerInit();
	
	//lcd_init ();
	
	PrintConsole(DEFAULT,"\x0C");
	
	//Initialize the LCD, if it does print the Init done Press start we have to reset the mcu.
//	lcd_clear();
//	lcd_put_cur (0,0);
//	lcd_send_string("Init done");
//	lcd_put_cur (1,0);
//	lcd_send_string("Press Start");
//	delay_ms(1000);
	
	
	/*Show my clock speed configuration on Putty*/
	getSystemClockSpeed();

	while(1)
	{
		power_on = 1;
		//Avoid with the counter the oscillations frequencies followed by the press of the button
		if (count>0){
			count--;
		}
		if (power_on){
			Power_On();
			//MotorPWM_Set(150, 150);
			/*Displays the values of the accelerometer and gyrometer data*/
			//DisplayAxisValues();
			
			// Get Potentiometer value
			//SendString("\r\n");
			//PrintConsole(INFO, "Analog read: %lf",o_Read_Potentiometer_o(4));
			//delay_ms(500);
	
		}
		
		else {
			Power_Off();
			//lcd_clear();
			delay_ms(200);
		}
		
	}

	return 0;
}