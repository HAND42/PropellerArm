#pragma once


#include "StringUtilities.h"
#include "Angles_Estimation.h"
#include "UART.h"
#include "Delay.h"
#include "i2c-lcd.h"
#include <stdio.h>
#include <stdarg.h>
#include "Accelerometer.h"
#include "Gyro.h"

typedef enum {
    DEFAULT = 0,
	 INFO    = 1,
	 WARNING = 2,
	 ERROR   = 3,
	 QT      = 4,
    
} SeverityLevel;

void PrintConsole(SeverityLevel severity, const char *format, ...);
void SendValuesToQT(float theta, float phy, float psi);
void DisplayAxisValues(void);
void DisplayFloatValue(float value);
void DisplayIntegerValue(short value);
