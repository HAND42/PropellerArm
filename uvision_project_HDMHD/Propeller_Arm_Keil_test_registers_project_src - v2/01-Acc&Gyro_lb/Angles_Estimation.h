#pragma once

#include "Accelerometer.h"
#include "Gyro.h"
#include "math.h"
#include "stm32f411xe.h"

#define G 9.80665
#define M_PI 3.14159265359
#define Ko 10
#define Ki 20

extern float T;

extern float previous_theta;
extern float pre_pre_theta;
extern float theta;

extern float theta_acc;
extern float pre_theta_acc;
extern float pre_pre_theta_acc;

extern float gyro_y_dot;
extern float pre_gyro_y_dot;
extern float pre_pre_gyroy__dot;

void GetAccelerometerValuesInMS2(float* accX, float* accY, float* accZ);
float GetThetaAcc(void);
float GetPhiAcc(void); 
float GetRollTurnRateGyro(void);
void Timer3InterruptInit(float sampling_period);
void Timer3InterruptEnable(void);
void Timer3InterruptDisable(void);
void TIM3_IRQHandler( void );
float TiltCompensatedThetaEstimation(void);
void TiltCompensatedThetaEstimation2(void);
float TiltCompensatedThetaEstimation3(void);
float HomeMadeThetaEstimation(void);

