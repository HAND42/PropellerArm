/* COPYRIGHT:
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + Copyright (c) Holger Buss, Ingo Busker
// + only for non-profit use
// + www.MikroKopter.com
// + porting the sources to other systems or using the software on other systems (except hardware from www.mikrokopter.de) is not allowed
// + see the File "License.txt" for further Informations
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
:copyright*/

#include "main.h"
#include "analog.h"

volatile struct IntXYZ currentAcc; //current measurement of acceleration? //replaces Aktuell_a{x,y,z}
volatile struct IntAngles A2DFilter={0,0,0};  //the 'raw' measurements from the A2D (NOTE: Yaw isn't currently used) //replaces AdWert{Nick,Roll,Gier}Filter 
volatile struct IntAngles A2DValue={0,0,0};   //replaces AdWert{Nick,Roll,Gier}
volatile struct IntXYZ A2DValueAcc={0,0,0};//replaces AdWertAcc{Nick,Roll,Hoch} :note: changed into xyz from angles nick = x, roll = y, hoch = z
volatile struct IntAnglesPR hiRes = {2500,2500};//replaces HiRes{Nick,Roll}, yaw not used
struct UnsignedCharAngles analogueOffset={115,115,115}; //replaces AnalogOffset{Nick,Roll,Gier} //This doesn't seem to be used

volatile int  UBat = 100;

volatile long airPressure = 32000;
volatile int  airPressureStart;
volatile unsigned int  airPressureMeasurement = 1023;
volatile unsigned int measurementCount = 0;
volatile unsigned char AdReady = 1;

//these two don't seem useful at the moment (sumHeight is used by Variometer, which is never used - except as debug). Leave in case of future need
volatile int VarioMeter = 0;
volatile long sumHeight = 0;

struct SensorPack A2DValues;

void ADC_Init(void){
  ADMUX = 0;//Referenz ist extern
  ANALOG_ON;
}

void findAirpressureOffset(void){
  unsigned int off;
  off = eeprom_read_byte(&EEPromArray[EEPROM_ADR_LAST_OFFSET]);
  if(off > 20){
    off -= 10;
  }

  static const int DESIRED_H_ADC = 800;

  OCR0A = off;
  Delay_ms_Mess(100);
  if(airPressureMeasurement < DESIRED_H_ADC){
    off = 0;
  }

  for(; off < 250;off++){
    OCR0A = off;
    Delay_ms_Mess(50);
    printf(".");
    if(airPressureMeasurement < DESIRED_H_ADC)
      break;
  }

  eeprom_write_byte(&EEPromArray[EEPROM_ADR_LAST_OFFSET], off);
  //DruckOffsetSetting = off;
  Delay_ms_Mess(300);
}

/*
0  n
1  r
2     g
3     y
4     x
5  n
6  r
7     u
8     z
9     L
10 n
11 r
12    g
13    y
14    x
15 n
16 r
17    L
*/

ISR(ADC_vect){
  static enum StateA2D A2DChannel = A2D_yaw;
  static unsigned char state = 0;//not sure what this is...
  static signed int yaw1, roll1, pitch1, pitch_filter, roll_filter;
  static signed int accy, accx;

  static long tmpAirPressure = 0;
  static char measurementCountPressure = 0;

  switch(state++){
  case 0:
    pitch1 = getADCValue(A2DChannel);
    A2DChannel = A2D_roll;
    break;
  case 1:
    roll1 = getADCValue(A2DChannel);
    A2DChannel = A2D_yaw;
    break;
  case 2:
    yaw1 = getADCValue(A2DChannel);
    A2DChannel = A2D_acc_y;
    break;
  case 3:
    currentAcc.y = NeutralAccY - getADCValue(A2DChannel);
    accy = currentAcc.y;
    A2DChannel = A2D_acc_x;
    break;
  case 4:
    currentAcc.x = getADCValue(A2DChannel) - NeutralAccX;
    accx =  currentAcc.x;
    A2DChannel = A2D_pitch;
    break;
  case 5:
    pitch1 += getADCValue(A2DChannel);
    A2DChannel = A2D_roll;
    break;
  case 6:
    roll1 += getADCValue(A2DChannel);
    A2DChannel = A2D_batteryVoltage;
    break;
  case 7:
    UBat = (3 * UBat + getADCValue(A2DChannel) / 3) / 4;
    A2DChannel = A2D_acc_z;
    break;
  case 8:
    A2DValueAcc.z =  (signed int) getADCValue(A2DChannel) - NeutralAccZ;
    if(A2DValueAcc.z > 1){
      if(NeutralAccZ < 750){//this is like a soft limit
        if(vehicleFlyingCounter < 500){//vehicle is NOT flying
          NeutralAccZ += 0.12;//=0.1 + 0.02
        }
        else{//vehicle is flying
          NeutralAccZ += 0.02;
        }
      }
    }
    else if(A2DValueAcc.z < -1){
      if(NeutralAccZ > 550){//this is like a soft limit
        if(vehicleFlyingCounter < 500){//not flying
          NeutralAccZ -= 0.12;//=0.1 + 0.02
        }
        else{//flying
          NeutralAccZ -= 0.02;
        }
      }
    }
    currentAcc.z = getADCValue(A2DChannel);
    //messanzahl_AccHoch = 1;
    //Mess_Integral_Hoch += A2DValueAcc.z;//Integrate
    //Mess_Integral_Hoch -= Mess_Integral_Hoch / 1024; //Damping (?)
    A2DChannel = A2D_airPressure;
    break;
  case 10:
    pitch1 += getADCValue(A2DChannel);
    A2DChannel = A2D_roll;
    break;
  case 11:
    roll1 += getADCValue(A2DChannel);
    A2DChannel = A2D_yaw;
    break;
  case 12:
    A2DValue.yaw   = 2047 - (getADCValue(A2DChannel) + yaw1);
    A2DChannel = A2D_acc_y;
    break;
  case 13:
    currentAcc.y = NeutralAccY - getADCValue(A2DChannel);
    A2DValueAcc.y = (currentAcc.y + accy);
    A2DChannel = A2D_acc_x;
    break;
  case 14:
    currentAcc.x = getADCValue(A2DChannel) - NeutralAccX;
    A2DValueAcc.x =  (currentAcc.x + accx);
    A2DChannel = A2D_pitch;
    break;
  case 15:
    pitch1 += getADCValue(A2DChannel);
    pitch1 *= 4;
    A2DValue.pitch = pitch1 / 8;
    pitch_filter = (pitch_filter + pitch1) / 2;
    hiRes.pitch = pitch_filter - A2DNeutral.pitch;
    A2DFilter.pitch = (A2DFilter.pitch + hiRes.pitch) / 2;
    A2DChannel = A2D_roll;
    break;
  case 16:
    roll1 += getADCValue(A2DChannel);
    roll1 *= 4;
    A2DValue.roll = roll1 / 8;
    roll_filter = (roll_filter + roll1) / 2;
    hiRes.roll = roll_filter - A2DNeutral.roll;
    A2DFilter.roll = (A2DFilter.roll + hiRes.roll) / 2;
    A2DChannel = A2D_airPressure;
    break;
  case 17:
    state = 0;
    AdReady = 1;
    measurementCount++;
    // "break" fehlt hier absichtlich
  case 9:
    airPressureMeasurement = getADCValue(A2DChannel);
    tmpAirPressure += airPressureMeasurement;
    if(++measurementCountPressure >= 18){
      airPressure = (7 * airPressure + tmpAirPressure + 4) / 8;  // -523.19 counts per 10 counts offset step
      currentHeight = airPressureStart - airPressure;

      const int SM_FILTER = 16;//removed the #define

      sumHeight -= sumHeight/SM_FILTER; //SM_FILTER = 16
      sumHeight += currentHeight;
      VarioMeter = (15 * VarioMeter + 8 * (int)(currentHeight - sumHeight/SM_FILTER))/16;
      tmpAirPressure /= 2;
      measurementCountPressure = 18/2;
    }
    A2DChannel = A2D_pitch;
    break;
  default:
    A2DChannel = 0; 
    state = 0; 
    A2DChannel = A2D_pitch;
    break;
  }

  ADMUX = A2DChannel;
  if(state != 0){
    ANALOG_ON;
  }
}

unsigned int getADCValue(enum StateA2D A2DChannel){//Fake the sensor data for HILS 
  
  switch(A2DChannel){//Copy the values into our return structure...
  case A2D_yaw:
    A2DValues.gyroscopes.yaw   = ADC;
    break;
  case A2D_roll:
    A2DValues.gyroscopes.roll  = ADC;
    break;
  case A2D_pitch:
    A2DValues.gyroscopes.pitch = ADC;
    break;
  case A2D_airPressure:
    A2DValues.airPressureSensor = ADC;
    break;
  case A2D_acc_z:
    A2DValues.accelerometer.z = ADC;
    break;
  case A2D_acc_y:
    A2DValues.accelerometer.y = ADC;
    break;
  case A2D_acc_x:
    A2DValues.accelerometer.x = ADC;
    break;
  case A2D_batteryVoltage:
    break;
  }

  if(enableHILS == HILS_REAL_FLIGHT){//return the actual ADC value
    return ADC;
  }
  
  switch(A2DChannel){//otherwise, we return the last 'fake' sensor data from HILS simulator
  case A2D_yaw:
    return simulatedSensorData.sensors.gyroscopes.yaw;
  case A2D_roll:
    return simulatedSensorData.sensors.gyroscopes.roll;
  case A2D_pitch:
    return simulatedSensorData.sensors.gyroscopes.pitch;
  case A2D_airPressure:
    return simulatedSensorData.sensors.airPressureSensor;
  case A2D_acc_z:
    return simulatedSensorData.sensors.accelerometer.z;
  case A2D_acc_y:
    return simulatedSensorData.sensors.accelerometer.y;
  case A2D_acc_x:
    return simulatedSensorData.sensors.accelerometer.x;
  case A2D_batteryVoltage:
    return SIMULATED_BATTERY_FULL_A2D;
  }

  return 0;//we should not reach this point...
}

/* this is never used... (only for PlatinenVersion == 13)
void SucheGyroOffset(void){
  unsigned char i, ready = 0;
  int timeout;

  timeout = SetDelay(2000);
  for(i=140; i != 0; i--){
    if(ready == 3 && i > 10){
      i = 9;
    }

    ready = 0;
    if(A2DValue.pitch < 1020) analogueOffset.pitch--; else if(A2DValue.pitch > 1030) analogueOffset.pitch++; else ready++;
    if(A2DValue.roll  < 1020) analogueOffset.roll--;  else if(A2DValue.roll  > 1030) analogueOffset.roll++;  else ready++;
    if(A2DValue.yaw   < 1020) analogueOffset.yaw--;   else if(A2DValue.yaw   > 1030) analogueOffset.yaw++;   else ready++;

    twi_state = 8;
    i2c_start();

    //Check that the gyros are OK
    LIMIT_MIN_MAX(analogueOffset.pitch,10,245);
    LIMIT_MIN_MAX(analogueOffset.roll, 10,245);
    LIMIT_MIN_MAX(analogueOffset.yaw,  10,245);

    while(twi_state){
      if(CheckDelay(timeout)){
        printf("\n\r DAC or I2C ERROR! Check I2C, 3Vref, DAC and BL-Ctrl"); 
        break;
      }
    }

    AdReady = 0;
    ANALOG_ON;
    while(!AdReady){
      //wait...
    }

    if(i<10){
      Delay_ms_Mess(10);
    }
  }
  Delay_ms_Mess(70);
}
*/
