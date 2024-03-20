/* A file where we can keep all the constant values
 *
 * mwm: 30.05.2011
 */
#ifndef CONSTANTS_H
#define CONSTANTS_H

#define NUMBER_OF_MOTORS 6//how many motors we have on the Hexacopter
#define MARK_DEBUG_STRING_LENGTH 80

/***************************************************************************
 * from _Settings.h
 ***************************************************************************/
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Abstimmung
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define ACC_AMPLIFY 6
#define FAKTOR_P    1
#define FAKTOR_I    0.0001

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Debug-Interface
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define SIO_DEBUG               1   // Soll der Debugger aktiviert sein?
#define MIN_DEBUG_INTERVALL   250   // in diesem Intervall werden Degugdaten ohne Aufforderung gesendet

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Sender
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//the ppm channel numbers:
#define CH_PITCH   0//was K_NICK
#define CH_ROLL    1//was K_ROLL
#define CH_GAS     2//was K_GAS
#define CH_YAW     3//was K_YAW
#define CH_POTI1   4//was K_POTI1   
#define CH_POTI2   5//was K_POTI2   
#define CH_POTI3   6//was K_POTI3   
#define CH_POTI4   7//was K_POTI4   
#define CH_POTI5   8//was K_POTI5   
#define CH_POTI6   9//was K_POTI6   
#define CH_POTI7   10//was K_POTI7   
#define CH_POTI8   11//was K_POTI8   
/***************************************************************************
 * from main.h
 ***************************************************************************/
// neue Hardware
////mwm: PlatinenVersion==20 for I3S hexa
#define ROT_OFF   PORTB &=~0x01
#define ROT_ON    PORTB |= 0x01
#define ROT_FLASH PORTB ^= 0x01
#define GRN_OFF   PORTB &=~0x02
#define GRN_ON    PORTB |= 0x02
#define GRN_FLASH PORTB ^= 0x02

#define SYSCLK F_CPU

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define EE_DATENREVISION 82 // Parameter fürs Koptertool; wird angepasst, wenn sich die EEPROM-Daten geändert haben
#define MIXER_REVISION    1 // wird angepasst, wenn sich die Mixer-Daten geändert haben

#define EEPROM_ADR_VALID            1
#define EEPROM_ADR_ACTIVE_SET       2
#define EEPROM_ADR_LAST_OFFSET      3

#define EEPROM_ADR_ACC_NICK         4
#define EEPROM_ADR_ACC_ROLL         6
#define EEPROM_ADR_ACC_Z            8
#define EEPROM_ADR_MINUTES         10
#define EEPROM_ADR_MINUTES2        14

#define EEPROM_ADR_CHANNELS          80

#define EEPROM_ADR_PARAM_LENGTH      98
#define EEPROM_ADR_PARAM_BEGIN      100

#define EEPROM_ADR_MIXER_TABLE     1000 // 1001 - 1100

#define CFG_HOEHENREGELUNG       0x01
#define CFG_HOEHEN_SCHALTER      0x02
#define CFG_HEADING_HOLD         0x04
#define CFG_KOMPASS_AKTIV        0x08
#define CFG_KOMPASS_FIX          0x10
#define CFG_GPS_AKTIV            0x20
#define CFG_ACHSENKOPPLUNG_AKTIV 0x40
#define CFG_DREHRATEN_BEGRENZER  0x80

#define CFG_LOOP_OBEN            0x01
#define CFG_LOOP_UNTEN           0x02
#define CFG_LOOP_LINKS           0x04
#define CFG_LOOP_RECHTS          0x08
#define CFG_MOTOR_BLINK          0x10
#define CFG_MOTOR_OFF_LED1       0x20
#define CFG_MOTOR_OFF_LED2       0x40
#define CFG_RES4                 0x80

#define CFG2_HEIGHT_LIMIT        0x01
#define CFG2_VARIO_BEEP          0x02
#define CFG_SENSITIVE_RC         0x04

#define RECEIVER_PPM              0
#define RECEIVER_SPEKTRUM         1
#define RECEIVER_SPEKTRUM_HI_RES  2
#define RECEIVER_SPEKTRUM_LOW_RES 3
#define RECEIVER_JETI             4
#define RECEIVER_ACT_DSL          5
#define RECEIVER_UNKNOWN          0xFF

#define J3High    PORTD |= 0x20
#define J3Low     PORTD &= ~0x20
#define J4High    PORTD |= 0x10
#define J4Low     PORTD &= ~0x10
#define J5High    PORTD |= 0x08
#define J5Low     PORTD &= ~0x08

//In the mixer table, which row is what:
#define MIXER_GAS   0
#define MIXER_PITCH 1
#define MIXER_ROLL  2
#define MIXER_YAW   3

#endif
