#pragma once

#include "Accelerometer.h"
#include "Gyro.h"
#include "math.h"
#include "stm32f411xe.h"
#include "System_Config.h"
#include "TwinPropellerArmSTM32F411_configuration.h"
#include "motor_pwm.h"
#include "DisplayData.h"
#include "Led.h"

#define G 9.80665
#define M_PI 3.14159265359
#define Ko 20
#define Ki 99

#define Fmoy 25


#define l1 0.363
#define I  0.07
#define K1 4
#define K2 128

#define T Period_IT_Control_loop


typedef struct {
	
	float theta_acc;
   float pre_theta_acc;
   float pre_pre_theta_acc;
    
}Acc_data;

typedef struct {
	
	float pre_theta;
   float pre_pre_theta;
   float theta;
	float biais;
	float pre_biais;
	
}Theta_estim;

typedef struct {
   float pitchTurnRate_radps;
	float gyro_y;
	float pre_gyro_y;
	float pre_pre_gyro_y;
    
}Gyro_data;

extern Gyro_data gyro_data;
extern Acc_data acc_data;
extern Theta_estim theta_data;

void GetAccelerometerValuesInMS2(float* accX, float* accY, float* accZ);
float GetThetaAcc_deg(void);
float GetThetaAcc_rad(void);
float GetThetaAcc_read_fast(void);
float GetPhiAcc_rad(void); 
float GetPhiAcc_deg(void);
float GetRollTurnRateGyro(void);
float GetThetaGyro_rad(void);
void Timer3InterruptInit(float sampling_period);
void Timer3InterruptEnable(void);
void Timer3InterruptDisable(void);
void TIM3_IRQHandler( void );
void Control_Loop_ThetaEstimation_CommandThrottle(void);
void TiltCompensatedThetaEstimation(void);
void TiltCompensatedThetaEstimation2(void);
void GetAccData_read_fast(void);
void GetGyroData_read_fast(void);

float saturate_force_input(float F);
