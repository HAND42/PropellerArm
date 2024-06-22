#pragma once
#include "System_Config.h"

int saturate(int input, int min_val, int max_val);
void MotorPWM_PIN_Init(void);
void MotorPWM_TIMER_Init(void);
void MotorPWM_PWM_Init(void);
void ESC_Calibration(void);

void o_Motor_Initialization_o(void);
void MotorPWM_Set(int g, int d);
void TestingThrustMotor(void);
void TestingStepInputResponse(void);
