#pragma once

#include "Accelerometer.h"
#include "Gyro.h"
#include "math.h"
#include "stm32f411xe.h"
#include "System_Config.h"
#include "DisplayData.h"
#include "Led.h"

#define G 9.80665
#define M_PI 3.14159265359
#define Ko 20
#define Ki 99

#define K1 4
#define K2 128

#define coeff_a 0.49
#define coeff_b 1.23

extern float T;

extern float previous_theta;
extern float pre_pre_theta;
extern float theta;

extern float theta_acc;
extern float pre_theta_acc;
extern float pre_pre_theta_acc;

extern float gyro_y;
extern float pre_gyro_y;
extern float pre_pre_gyroy;

void GetAccelerometerValuesInMS2(float* accX, float* accY, float* accZ);
float GetThetaAcc_deg(void);
float GetThetaAcc_rad(void);
float GetPhiAcc_rad(void); 
float GetPhiAcc_deg(void);
float GetRollTurnRateGyro(void);
float GetThetaGyro_rad(void);
void Timer3InterruptInit(float sampling_period);
void Timer3InterruptEnable(void);
void Timer3InterruptDisable(void);
void TIM3_IRQHandler( void );
float TiltCompensatedThetaEstimation(void);
float TiltCompensatedThetaEstimation2(void);
