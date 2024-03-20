/*#######################################################################################
Flight Control
#######################################################################################*/

#ifndef _FC_H
#define _FC_H

#include <avr/boot.h>//for PORTC def
#include <stdlib.h>//for abs(.)

#include <stdio.h> //for sprintf

#include "HILSStructures.h"
#include "structures.h"
#include "constants.h"
#include "twimaster.h"
#include "timer0.h"
#include "analog.h"
#include "gps.h"
#include "led.h"
#include "spi.h"
#include "uart.h"
#include "libfc.h"
#include "capacity.h"
#include "usefulMacros.h"
#include "CommonCommsDefines.h"

extern volatile unsigned int beeptime;//from timer0.c
extern volatile struct IntAngles A2DValue;//from analog.c
extern volatile struct IntXYZ A2DValueAcc;//from analog.c
extern unsigned char EEPromArray[];//not sure from where.
extern unsigned char SendVersionToNavi;//from main.c
extern struct str_FromNaviCtrl_Value FromNaviCtrl_Value; //from spi.c
extern volatile int PPM_in[26];//the RC channels//from rc.c
extern unsigned volatile char PC_MotortestActive; //from uart.c
extern unsigned char MotorTest[16];//from uart.c
extern struct str_DebugOut DebugOut;//from uart.c
extern volatile int PPM_diff[26];  //from rc.c
extern volatile unsigned char NewPpmData;//from rc.c
extern volatile int16_t    ServoNickValue;//from timer0.c
extern unsigned char BattLowVoltageWarning;//from main.c

extern const long GIER_GRAD_FAKTOR;

//functions in main.c
unsigned char GetActiveParamSetNumber(void);
void ReadParameterSet (unsigned char number, unsigned char *buffer, unsigned char length);
void WriteParameterSet(unsigned char number, unsigned char *buffer, unsigned char length);
void SetActiveParamSetNumber(unsigned char number);
void LipoDetection(unsigned char print);

#define STICK_GAIN 4 //Increase the resolution of the desired values

#define FCFLAG_MOTOR_RUN            0x01
#define FCFLAG_FLY                  0x02
#define FCFLAG_CALIBRATE            0x04
#define FCFLAG_START                0x08
#define FCFLAG_EMERGENCY_LANDING     0x10
#define FCFLAG_LOWBAT                  0x20
#define FCFLAG_SPI_RX_ERR            0x40
#define FCFLAG_I2CERR                  0x80

/*these are never used, and would be (at best) only confusing
#define Poti1 Poti[0]
#define Poti2 Poti[1]
#define Poti3 Poti[2]
#define Poti4 Poti[3]
#define Poti5 Poti[4]
#define Poti6 Poti[5]
#define Poti7 Poti[6]
#define Poti8 Poti[7]
*/

void motorController(void);
void sendMotorData(void);
void calibrateProcessMeasurement(void);
void processMeasurement(void);
void SetNeutral(void);
void Piep(unsigned char Anzahl, unsigned int dauer);
extern void DefaultKonstanten1(void);
extern void DefaultKonstanten2(void);
extern void DefaultKonstanten3(void);
extern void DefaultStickMapping(void);

extern volatile unsigned char FCFlags;
extern volatile unsigned int I2CTimeout;
extern unsigned char Poti[9];

//extern volatile long Mess_Integral_Hoch;
extern int  KompassValue;
extern int  KompassStartwert;
extern int  KompassRichtung;
extern long ErsatzKompass;
extern int  ErsatzKompassInGrad; // Kompasswert in Grad

//extern volatile unsigned char Timeout ;
//extern int  DiffNick,DiffRoll;
extern volatile unsigned char SenderOkay;
extern volatile unsigned char SenderRSSI;
//extern int StickNick,StickRoll,StickGier;
//extern char MotorenEin;
extern unsigned char Parameter_Servo3,Parameter_Servo4,Parameter_Servo5;
extern char VarioCharacter;

extern volatile unsigned char shouldPrintInfoStrings; //from uart.c

extern unsigned char Parameter_Luftdruck_D;
extern unsigned char Parameter_MaxHoehe;
extern unsigned char Parameter_Hoehe_P;
extern unsigned char Parameter_Hoehe_ACC_Wirkung;
extern unsigned char Parameter_KompassWirkung;
extern unsigned char Parameter_Gyro_P;
extern unsigned char Parameter_Gyro_I;
extern unsigned char Parameter_Gier_P;
extern unsigned char Parameter_ServoNickControl;
extern unsigned char Parameter_ServoRollControl;
extern unsigned char Parameter_AchsKopplung1;
extern unsigned char Parameter_AchsKopplung2;
extern unsigned char Parameter_J16Bitmask;             // for the J16 Output
extern unsigned char Parameter_J16Timing;              // for the J16 Output
extern unsigned char Parameter_J17Bitmask;             // for the J17 Output
extern unsigned char Parameter_J17Timing;              // for the J17 Output
extern signed char MixerTable[MAX_MOTORS][4];

extern char markDebugString[MARK_DEBUG_STRING_LENGTH];

#define  STRUCT_PARAM_LAENGE  sizeof(EE_Parameter) //mwm: i.e. size of an mk_param_struct

struct MixerStruct{
  char Revision;
  char Name[12];
  signed char Motor[16][4];
  /*From inspection of MKTool:
   * Motor[i][0]: GAS
   * Motor[i][1]: NICK
   * Motor[i][2]: ROLL
   * Motor[i][3]: YAW
   *
   * 64 -> 100%
   *-64 ->-100%
   */
};

//these are used in fc.c, come from elsewhere

extern struct mk_param_struct EE_Parameter;

extern volatile struct IntXYZ currentAcc; //replaces Aktuell_a{x,y,z} //from analog.c
extern volatile struct IntAngles A2DFilter;//the 'raw' measurements from the A2D (NOTE: Yaw isn't currently used)
extern volatile struct IntAnglesPR hiRes;//replaces HiRes{Nick,Roll}, yaw not used
extern volatile unsigned char haveDisabledHILS;

extern volatile long airPressure;//from analog.c
extern volatile int  airPressureStart;//from analog.c
extern volatile unsigned int  airPressureMeasurement;//from analog.c
#endif
