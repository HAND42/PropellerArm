#ifndef _MAIN_H
#define _MAIN_H

#define VERSION_MAJOR    0
#define VERSION_MINOR    18
#define VERSION_PATCH    2

#define VERSION_SERIAL_MAJOR    11
#define VERSION_SERIAL_MINOR    0

#define FC_SPI_COMPATIBLE        12
#define MK3MAG_I2C_COMPATIBLE    3

// FC FLAGS
#define FCFLAG_MOTOR_RUN                  0x01
#define FCFLAG_FLY                        0x02
#define FCFLAG_CALIBRATE                  0x04
#define FCFLAG_START                      0x08
#define FCFLAG_EMERGENCY_LANDING          0x10
#define FCFLAG_LOWBAT                      0x20
#define FCFLAG_SPI_RX_ERR                  0x40
#define FCFLAG_RESERVE1                      0x80

// NC Errors
#define NCERR_FLAG_FC_COMPATIBLE            0x00000001
#define NCERR_FLAG_MK3MAG_COMPATIBLE        0x00000002
#define NCERR_FLAG_FC_COMMUNICATION            0x00000004
#define NCERR_FLAG_MK3MAG_COMMUNICATION        0x00000008
#define NCERR_FLAG_MKGPS_COMMUNICATION        0x00000010
#define NCERR_FLAG_BAD_COMPASS_HEADING        0x00000020
#define NCERR_FLAG_RC_SIGNAL_LOST            0x00000040


#define LIMIT_MIN(value, min) {if(value <= min) value = min;}
#define LIMIT_MAX(value, max) {if(value >= max) value = max;}
#define LIMIT_MIN_MAX(value, min, max) {if(value <= min) value = min; else if(value >= max) value = max;}


extern u16 BeepTime;
extern u8  NCFlags;
extern u8 ClearFCFlags;
void Interrupt_Init(void);
extern s16 GeoMagDec;


typedef struct
{
    u8 ActiveSetting;
    u8 User1;
    u8 User2;
    u8 User3;
    u8 User4;
    u8 User5;
    u8 User6;
    u8 User7;
    u8 User8;
    u8 NaviGpsModeControl;
    u8 NaviGpsGain;
    u8 NaviGpsP;
    u8 NaviGpsI;
    u8 NaviGpsD;
    u8 NaviGpsPLimit;
    u8 NaviGpsILimit;
    u8 NaviGpsDLimit;
    u8 NaviGpsACC;
    u8 NaviGpsMinSat;
    u8 NaviStickThreshold;
    u8 NaviOperatingRadius;
    u8 NaviWindCorrection;
    u8 NaviSpeedCompensation;
    u8 LowVoltageWarning;
    u8 NaviAngleLimitation;
    u8 NaviPH_LoginTime;
} __attribute__((packed)) Param_t;

typedef struct
{
    s8 StickNick;
    s8 StickRoll;
    s8 StickYaw;
    s8 StickGas;
    u8 Poti[8];
    u8 Poti5;
    u8 Poti6;
    u8 Poti7;
    u8 Poti8;
    u8 RC_Quality;
    u8 RC_RSSI;
    u8 BAT_Voltage;
    u16 BAT_Current;
    u16 BAT_UsedCapacity;
    u8 Flags;
} __attribute__((packed)) FC_t;

extern Param_t Parameter;
extern volatile FC_t FC;
extern s8 ErrorMSG[25];
extern u32 ErrorCode; 
extern u8 StopNavigation;
#endif // _MAIN_H
