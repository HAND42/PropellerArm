#pragma once;


#include "System_Config.h"
#include "Delay.h"
#include "Led.h"

#include "Accelerometer.h"
#include "Angles_Estimation.h"
#include "Gyro.h"
#include "DisplayData.h"

#include "i2c-lcd.h"
#include "I2C-ST_Slave_Ard_Master.h"
#include "motor_pwm.h"
#include "Potentiometer.h"

#include "RegisterAddresses.h"
#include <stdio.h>



/* Clock General Information 

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
 
*/

typedef int bool;
#define true 1
#define false 0
	
extern bool power_on;

/*   Priortiy Configuration   */

#define IT_TIM3_CONTROL_LOOP 5// LOOP CONTROL OF THE MOTOR VELOCITY COMPUTING ESTIMATED THETA
#define USART2_IRQ_PRIORITY  4// PC DEBUGGING COMMUNICATION
#define USART6_IRQ_PRIORITY  3// QT INTERFACE COMMUNICATION
#define I2C3_IRQ_PRIORITY    7// I2C SLAVE STM32 MASTER ARDUINO SCREEN		  
#define IT_TIM2_IRQ_PRIORITY 10// TIMER INTERRUPTION FOR DEBOUNCING BUTTON HARDWARE OSCILLATION

/*   Timer frequency Configuration   */

#define Period_IT_Expt_Oscill		0.01 	// T = 0.01 s		/* Button with timer interruption for debouncing the hardware oscillation	*/
#define Period_IT_Control_loop	0.08	//       		/* Control loop	*/

/* ADDRESS OF THE USED SLAVES */

#define SLAVE_ADDRESS_LCD 					0x4E
#define SLAVE_ADDRESS_SCREEN_CTR_STM32 0x12

/* --------- Configuration define of peripherals  --------- */

#define CONFIG_DEBOUNCING_BUTTON_ON

#define CONFIG_DEBUG_UART_ON

#define CONFIG_QT_INTERFACE_UART_ON

#define CONFIG_GYRO_INIT_ON	

#define CONFIG_ACCELEROMETER_INIT_ON	

#define CONFIG_BLDC_MOTOR_ON

//#define CONFIG_BLDC_MOTOR_TESTING_ON

//#define CONFIG_POTENTIOMETER_THETA_INPUT_ON

#define CONFIG_CONTROL_LOOP_ON

//#define CONFIG_SLAVE_STM_MASTER_SCREEN_ON

//#define CONFIG_LCD_ON

/* --------- Configuration define of peripherals  --------- */

#define SHOW_CLOCK_CONFIG
//#define SHOW_ACC_GYRO_DATA
//#define SHOW_POTENTIOMETER_THETA_REF
//#define SHOW_COMPUTING_TIME_CONTROL_LOOP


/* Pinout mapping */

/*
 
UART2 communication :
	// USART2 TX --> PA2 // Write
	// USART2 RX --> PA3 // Read
	
USART6 communication :
	// USART6 TX --> PC6 // Write
	// USART6 RX --> PC7 // Read
	
Gyrometer L3GD20 pins are connected to the STM32F411:
//    CS  --> PE3   // Chip Select
//    SPC --> PA5   // Clock
//    SDI --> PA7   // Data In
//    SDO --> PA6   // Data Out

Accelerometer LSM303DLHC pins are connected to the STM32F411:
// SCL  --> PB6
// SDA  --> PB9

LCD display to the same I2C1 bus pins as the Accelerometer:
// SCL  --> PB6
// SDA  --> PB9

Power mode interrupt pins 
// GND
// Pull-Up Mode --> PA0

Potentiometer pins
// GND 3V 
//Read value --> PA4

BAUD RATE : 115 200

*/

