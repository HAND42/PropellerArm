#ifndef _UART_H
#define _UART_H

#include "constants.h"
#include "HILSStructures.h"
#include "CommonCommsDefines.h"

#define printf_P(format, args...)   _printf_P(&uart_putchar, format , ## args)
#define printf(format, args...)     _printf_P(&uart_putchar, PSTR(format) , ## args)

void BearbeiteRxDaten(void);

extern unsigned char DebugGetAnforderung;

extern unsigned volatile char UebertragungAbgeschlossen;
extern unsigned volatile char PC_DebugTimeout;
extern unsigned volatile char NeueKoordinateEmpfangen;
extern unsigned volatile char PC_MotortestActive;
extern unsigned char MeineSlaveAdresse;
extern unsigned char PcZugriff;
extern unsigned char RemotePollDisplayLine;
extern int Debug_Timer,Kompass_Timer;
extern void UART_Init (void);
extern void uart_putchar (char c);
extern void boot_program_page (uint32_t page, uint8_t *buf);
extern void DatenUebertragung(void);
extern void Uart1Init(void);
extern void BearbeiteRxDaten(void);
//extern unsigned char MotorTest[16]; //not extern, defined in uart.c
extern char vehicleIsFlying;//from fc.c
extern volatile enum TypeHILS enableHILS;
extern volatile struct MotorCommandsHILS motorCommandsHILS;//the motor commands we will use for HILS

extern volatile unsigned long controllerCycleNumber;

struct str_DebugOut
{
  unsigned char Digital[2];
  signed int Analog[32];//Debugwerte
};

extern struct str_DebugOut    DebugOut;

struct str_WinkelOut
{
  signed int    Winkel[2];
  unsigned char UserParameter[2];
  unsigned char CalcState;
  unsigned char Orientation;
};
extern struct str_WinkelOut  WinkelOut;

struct str_Data3D
{
  signed int  Winkel[3]; // nick, roll, compass in 0,1°
  signed char reserve[8];
};
extern struct str_Data3D Data3D;

struct str_ExternControl
{
  unsigned char Digital[2];
  unsigned char RemoteTasten;
  signed char   Nick;
  signed char   Roll;
  signed char   Gier;
  unsigned char Gas;
  signed char   Hight;
  unsigned char free;
  unsigned char Frame;
  unsigned char Config;
};
extern struct str_ExternControl   ExternControl;

struct str_VersionInfo
{
  unsigned char SWMajor;
  unsigned char SWMinor;
  unsigned char ProtoMajor;
  unsigned char ProtoMinor;
  unsigned char SWPatch;
  unsigned char Reserved[5];
};
extern struct str_VersionInfo VersionInfo;

extern struct LongAngles angleIntegral;
extern struct LongAngles wrapAround180; //from fc.c
extern struct SensorPack A2DValues; //from analog.c

extern unsigned char Parameter_UserParam1,Parameter_UserParam2,Parameter_UserParam3,Parameter_UserParam4,Parameter_UserParam5,Parameter_UserParam6,Parameter_UserParam7,Parameter_UserParam8;//from fc.c

#endif //_UART_H
