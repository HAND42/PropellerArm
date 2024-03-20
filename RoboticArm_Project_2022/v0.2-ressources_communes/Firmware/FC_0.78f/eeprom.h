/* mwm 30.05.2011
 * 
 * Hopefully this'll sort out the linking errors...(MK just included eeprom.c)
 */

#ifndef EEPROM_H
#define EEPROM_H

#include "constants.h"
#include "structures.h"
#include "string.h"//needed for memcpy

void DefaultStickMapping(void);

void DefaultKonstanten1(void);
void DefaultKonstanten2(void);
void DefaultKonstanten3(void);

extern struct mk_param_struct EE_Parameter;

#endif
