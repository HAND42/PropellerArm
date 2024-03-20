#ifndef _ANALOG_H
#define _ANALOG_H

#include "structures.h"
#include "usefulMacros.h"
#include "HILSStructures.h"

extern volatile int UBat;
extern volatile unsigned int measurementCount;
extern volatile int VarioMeter;
extern volatile unsigned char AdReady;

extern int NeutralAccX, NeutralAccY,Mittelwert_AccHoch;//from fc.c
extern volatile float NeutralAccZ;//from fc.c
extern struct IntAngles A2DNeutral; //from fc.c
extern volatile enum TypeHILS enableHILS;//from fc.c
extern unsigned int vehicleFlyingCounter;//from fc.c
extern long currentHeight;//from fc.c
extern volatile struct SimulatedSensorData simulatedSensorData;//from uart.c

enum StateA2D{//replaces #defines
  A2D_yaw,
  A2D_roll,
  A2D_pitch,
  A2D_airPressure,
  A2D_batteryVoltage,
  A2D_acc_z,
  A2D_acc_y,
  A2D_acc_x,
};

unsigned int ReadADC(unsigned char adc_input);
void ADC_Init(void);
void findAirpressureOffset(void);
unsigned int getADCValue(enum StateA2D kanal);

static const int SIMULATED_BATTERY_FULL_A2D = 411;

#define ANALOG_OFF ADCSRA=0

#define ANALOG_ON ADCSRA=(1<<ADEN)|(1<<ADSC)|(0<<ADATE)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0)|(1<<ADIE)
 //Signle trigger Mode, Interrupt on
#endif //_ANALOG_H
