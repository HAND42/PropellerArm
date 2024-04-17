#pragma once
#include "System_Config.h"
#include "Delay.h"

void MotorPWM_PIN_Init(void);
void MotorPWM_TIMER_Init(void);
void MotorPWM_PWM_Init(void);

void o_MotorPWM_Initialization_o(void);

int saturate(int input, int min_val, int max_val);
void MotorPWM_Set(int g, int d);
void MotorPWM_ESC_calibration(int min_val, int max_val);
