
#include "Angles_Estimation.h"

double bruit=0.0;
double teta = 0.0;
double biais = 0.0;
double TS = 0.002;		// 0.002
const double Kteta = 1.5;	// 1.5
const double Ki =((Kteta * Kteta) /4.0 );


float TiltCompensateThetaEstimation(float* accX, float* gyroY){
	
}

float GetThetaAcc(float* accX){
	return asin(*accX/G)*180/M_PI;
}

float GetPhiAcc(float* accY,float* accZ){
	return atan (-(*accY)/(*accZ))*180/M_PI;
}


void GetAccelerometerValuesInMS2(float* accX, float* accY, float* accZ){
	short accelX, accelY, accelZ;
	GetAccelerometerValues(&accelX, &accelY, &accelZ);
	
	//The output of the accelerometer is mG/LSB = milli-G's / Least Significant bit
	//The LSM303DLHC outputs a 16 bit value for the accelerometer readings
	//16 bits equals 65,535 and -2 to +2, this would be a total of 4g
	
	*accX = accelX*0.061;    //4,000 MilliGs / 65,535 because FS = +- 2g
	*accX = *accX/1000;     //Convert the MilliG in G.
	*accX = *accX*G;			//Convert G in M/S2
	
	*accY = accelY*0.061;   //4,000 MilliGs / 65,535 because FS = +- 2g
	*accY = *accY/1000;		//Convert the MilliG in G.
	*accY = *accY*G;			//Convert G in M/S2
	
	*accZ = accelZ*0.061;   //4,000 MilliGs / 65,535 because FS = +- 2g
	*accZ = *accZ/1000;		//Convert the MilliG in G.
	*accZ = *accZ*G;			//Convert G in M/S2
	
}



