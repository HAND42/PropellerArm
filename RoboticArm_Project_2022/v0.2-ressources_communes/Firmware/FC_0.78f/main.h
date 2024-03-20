#ifndef _MAIN_H
#define _MAIN_H

#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <avr/boot.h>
#include <avr/wdt.h>

#include <stdio.h>

//not used:
//#include "old_macros.h"
//#include "_Settings.h"

#include "printf_P.h"
#include "timer0.h"
#include "uart.h"
#include "analog.h"
#include "twimaster.h"
#include "menu.h"
#include "rc.h"
#include "fc.h"
#include "gps.h"
#include "spi.h"
#include "led.h"
#include "spektrum.h"
#include "capacity.h"
#include "libfc.h"
#include "constants.h"

#ifndef EEMEM
#define EEMEM __attribute__ ((section (".eeprom")))
#endif

//#define DEBUG_DISPLAY_INTERVALL  123 // in ms


#define DELAY_US(x)    ((unsigned int)( (x) * 1e-6 * F_CPU ))

extern char markDebugString[MARK_DEBUG_STRING_LENGTH];

extern struct IntAngles A2DNeutral; //from fc.c
extern struct MixerStruct Mixer; //from fc.c

extern volatile int16_t    ServoNickValue; //from timer0.h
extern volatile int16_t    ServoRollValue; //from timer0.h

extern volatile unsigned char SenderOkay;
extern unsigned char BattLowVoltageWarning;
extern unsigned char PlatinenVersion;//mwm: PlatinenVersion==20 for I3S hexa
extern unsigned char SendVersionToNavi;
extern unsigned char EEPromArray[];
extern unsigned int FlugMinuten,FlugMinutenGesamt,FlugSekunden;

unsigned char GetActiveParamSetNumber(void);
void ReadParameterSet (unsigned char number, unsigned char *buffer, unsigned char length);
void WriteParameterSet(unsigned char number, unsigned char *buffer, unsigned char length);
void SetActiveParamSetNumber(unsigned char number);
void LipoDetection(unsigned char print);

extern char vehicleIsFlying; //from fc.c

#endif //_MAIN_H
