#pragma once

#include "Accelerometer.h"
#include "Gyro.h"
#include "math.h"

#define G 9.80665
#define M_PI 3.14159265359

float GetThetaAcc(float* accX);
void GetAccelerometerValuesInMS2(float* accX, float* accY, float* accZ);
float GetPhiAcc(float* accY,float* accZ);
float TiltCompensateThetaEstimation(float* accX, float* gyroY);
