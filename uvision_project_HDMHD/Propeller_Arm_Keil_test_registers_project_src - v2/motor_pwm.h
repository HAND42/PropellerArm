#pragma once
#include "System_Config.h"

void MotorPWM_EPort_PinSetup(void);
void MotorPWM_BPort_PinSetup(void);
void MotorPWM_Timer1Setup(void);
void MotorPWM_Timer3Setup(void);
void MotorPWM_TIM1_Start(double dutyCyclePercent);
void MotorPWM_TIM3_Start(double dutyCyclePercent);
void Start_PWM(void);

//This file header is not working for now I'm not able to make a PWM on a LED, you can see my configuration here ad if you find the bug I'm interested.