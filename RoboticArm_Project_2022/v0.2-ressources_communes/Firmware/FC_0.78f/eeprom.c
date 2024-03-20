#include "eeprom.h"

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + Konstanten
// + 0-247 -> normale Werte
// + 255 -> Poti1
// + 254 -> Poti2
// + 253 -> Poti3
// + 252 -> Poti4
// + 251 -> Poti5
// + 247 -> Poti6
// + 249 -> Poti7
// + 248 -> Poti8
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void DefaultStickMapping(void){ // mode 2
 EE_Parameter.Kanalbelegung[CH_GAS]   = 1;
 EE_Parameter.Kanalbelegung[CH_ROLL]  = 2;
 EE_Parameter.Kanalbelegung[CH_PITCH] = 3;
 EE_Parameter.Kanalbelegung[CH_YAW]   = 4;
 EE_Parameter.Kanalbelegung[CH_POTI1] = 5;
 EE_Parameter.Kanalbelegung[CH_POTI2] = 6;
 EE_Parameter.Kanalbelegung[CH_POTI3] = 7;
 EE_Parameter.Kanalbelegung[CH_POTI4] = 8;
 EE_Parameter.Kanalbelegung[CH_POTI5] = 9;
 EE_Parameter.Kanalbelegung[CH_POTI6] = 10;
 EE_Parameter.Kanalbelegung[CH_POTI7] = 11;
 EE_Parameter.Kanalbelegung[CH_POTI8] = 12;
}

void DefaultKonstanten1(void){
 EE_Parameter.GlobalConfig = CFG_ACHSENKOPPLUNG_AKTIV | CFG_KOMPASS_AKTIV | CFG_GPS_AKTIV | CFG_HOEHEN_SCHALTER;
 EE_Parameter.ExtraConfig = CFG2_HEIGHT_LIMIT;// | CFG2_VARIO_BEEP | CFG_SENSITIVE_RC
 EE_Parameter.Hoehe_MinGas = 30;
 EE_Parameter.MaxHoehe     = 255;      // Wert : 0-247   255 -> Poti1
 EE_Parameter.Hoehe_P      = 10;       // Wert : 0-32
 EE_Parameter.Luftdruck_D  = 30;       // Wert : 0-247
 EE_Parameter.Hoehe_ACC_Wirkung = 30;  // Wert : 0-247
 EE_Parameter.Hoehe_HoverBand = 5;     // Wert : 0-247
 EE_Parameter.Hoehe_GPS_Z = 64;        // Wert : 0-247
 EE_Parameter.Hoehe_StickNeutralPoint = 0; // Wert : 0-247 (0 = Hover-Estimation)
 EE_Parameter.Hoehe_Verstaerkung = 20; // Wert : 0-50
 EE_Parameter.Stick_P = 14;            // Wert : 1-6
 EE_Parameter.Stick_D = 16;            // Wert : 0-64
 EE_Parameter.Gier_P = 12;             // Wert : 1-20
 EE_Parameter.Gas_Min = 8;             // Wert : 0-32
 EE_Parameter.Gas_Max = 230;           // Wert : 33-247
 EE_Parameter.GyroAccFaktor = 30;      // Wert : 1-64
 EE_Parameter.KompassWirkung = 128;    // Wert : 0-247
 EE_Parameter.Gyro_P = 80;             // Wert : 0-247
 EE_Parameter.Gyro_I = 150;            // Wert : 0-247
 EE_Parameter.Gyro_D = 3;              // Wert : 0-247
 EE_Parameter.Gyro_Gier_P = 80;        // Wert : 0-247
 EE_Parameter.Gyro_Gier_I = 150;       // Wert : 0-247
 EE_Parameter.UnterspannungsWarnung = 33; // Wert : 0-247 ( Automatische Zellenerkennung bei < 50)
 EE_Parameter.NotGas = 35;                // Wert : 0-247     // Gaswert bei Empangsverlust
 EE_Parameter.NotGasZeit = 30;            // Wert : 0-247     // Zeit bis auf NotGas geschaltet wird, wg. Rx-Problemen
 EE_Parameter.I_Faktor = 32;
 EE_Parameter.UserParam1 =   0;           // zur freien Verwendung
 EE_Parameter.UserParam2 =   0;           // zur freien Verwendung
 EE_Parameter.UserParam3 =   0;           // zur freien Verwendung
 EE_Parameter.UserParam4 =   0;           // zur freien Verwendung
 EE_Parameter.UserParam5 =   0;           // zur freien Verwendung
 EE_Parameter.UserParam6 =   0;           // zur freien Verwendung
 EE_Parameter.UserParam7 = 0;             // zur freien Verwendung
 EE_Parameter.UserParam8 = 0;             // zur freien Verwendung
 EE_Parameter.ServoNickControl = 100;     // Wert : 0-247     // Stellung des Servos
 EE_Parameter.ServoNickComp = 40;         // Wert : 0-247     // Einfluss Gyro/Servo
 EE_Parameter.ServoCompInvert = 1;        // Wert : 0-247     // Richtung Einfluss Gyro/Servo
 EE_Parameter.ServoNickMin = 0;           // Wert : 0-247     // Anschlag
 EE_Parameter.ServoNickMax = 247;         // Wert : 0-247     // Anschlag
 EE_Parameter.ServoNickRefresh = 6;
 EE_Parameter.Servo3 = 125;
 EE_Parameter.Servo4 = 125;
 EE_Parameter.Servo5 = 125;
 EE_Parameter.ServoRollControl = 100;     // Wert : 0-247     // Stellung des Servos
 EE_Parameter.ServoRollComp = 40;         // Wert : 0-247     // Einfluss Gyro/Servo
 EE_Parameter.ServoRollMin = 0;           // Wert : 0-247     // Anschlag
 EE_Parameter.ServoRollMax = 247;         // Wert : 0-247     // Anschlag
 EE_Parameter.LoopGasLimit = 50;
 EE_Parameter.LoopThreshold = 90;         // Wert: 0-247  Schwelle f�r Stickausschlag
 EE_Parameter.LoopHysterese = 50;
 EE_Parameter.BitConfig = 0;              // Bitcodiert: 0x01=oben, 0x02=unten, 0x04=links, 0x08=rechts / wird getrennt behandelt
 EE_Parameter.AchsKopplung1 = 90;
 EE_Parameter.AchsKopplung2 = 80;
 EE_Parameter.CouplingYawCorrection = 1;
 EE_Parameter.WinkelUmschlagNick = 85;
 EE_Parameter.WinkelUmschlagRoll = 85;
 EE_Parameter.GyroAccAbgleich = 16;        // 1/k
 EE_Parameter.Driftkomp = 32;
 EE_Parameter.DynamicStability = 100;
 EE_Parameter.J16Bitmask = 95;
 EE_Parameter.J17Bitmask = 243;
 EE_Parameter.WARN_J16_Bitmask = 0xAA;
 EE_Parameter.WARN_J17_Bitmask = 0xAA;
 EE_Parameter.J16Timing = 15;
 EE_Parameter.J17Timing = 15;
 EE_Parameter.NaviGpsModeControl = 254; // 254 -> Poti 2
 EE_Parameter.NaviGpsGain = 100;
 EE_Parameter.NaviGpsP = 90;
 EE_Parameter.NaviGpsI = 90;
 EE_Parameter.NaviGpsD = 90;
 EE_Parameter.NaviGpsPLimit = 75;
 EE_Parameter.NaviGpsILimit = 75;
 EE_Parameter.NaviGpsDLimit = 75;
 EE_Parameter.NaviGpsACC = 0;
 EE_Parameter.NaviGpsMinSat = 6;
 EE_Parameter.NaviStickThreshold = 8;
 EE_Parameter.NaviWindCorrection = 90;
 EE_Parameter.NaviSpeedCompensation = 30;
 EE_Parameter.NaviOperatingRadius = 100;
 EE_Parameter.NaviAngleLimitation = 100;
 EE_Parameter.NaviPH_LoginTime = 4;
 EE_Parameter.Receiver = RECEIVER_SPEKTRUM;
 memcpy(EE_Parameter.Name, "Sport\0", 12);
}

void DefaultKonstanten2(void){
 EE_Parameter.GlobalConfig = CFG_ACHSENKOPPLUNG_AKTIV | CFG_KOMPASS_AKTIV | CFG_GPS_AKTIV | CFG_HOEHEN_SCHALTER;
 EE_Parameter.ExtraConfig = CFG2_HEIGHT_LIMIT;// | CFG2_VARIO_BEEP | CFG_SENSITIVE_RC
 EE_Parameter.Hoehe_MinGas = 30;
 EE_Parameter.MaxHoehe     = 255;         // Wert : 0-247   255 -> Poti1
 EE_Parameter.Hoehe_P      = 10;          // Wert : 0-32
 EE_Parameter.Luftdruck_D  = 30;          // Wert : 0-247
 EE_Parameter.Hoehe_ACC_Wirkung = 30;     // Wert : 0-247
 EE_Parameter.Hoehe_HoverBand = 5;        // Wert : 0-247
 EE_Parameter.Hoehe_GPS_Z = 64;           // Wert : 0-247
 EE_Parameter.Hoehe_StickNeutralPoint = 0;// Wert : 0-247 (0 = Hover-Estimation)
 EE_Parameter.Hoehe_Verstaerkung = 15;    // Wert : 0-50
 EE_Parameter.Stick_P = 10;               // Wert : 1-6
 EE_Parameter.Stick_D = 16;               // Wert : 0-64
 EE_Parameter.Gier_P = 6;                 // Wert : 1-20
 EE_Parameter.Gas_Min = 8;                // Wert : 0-32
 EE_Parameter.Gas_Max = 230;              // Wert : 33-247
 EE_Parameter.GyroAccFaktor = 30;         // Wert : 1-64
 EE_Parameter.KompassWirkung = 128;       // Wert : 0-247
 EE_Parameter.Gyro_P = 90;                // Wert : 0-247
 EE_Parameter.Gyro_I = 120;               // Wert : 0-247
 EE_Parameter.Gyro_D = 3;                 // Wert : 0-247
 EE_Parameter.Gyro_Gier_P = 90;           // Wert : 0-247
 EE_Parameter.Gyro_Gier_I = 120;          // Wert : 0-247
 EE_Parameter.UnterspannungsWarnung = 33; // Wert : 0-247 ( Automatische Zellenerkennung bei < 50)
 EE_Parameter.NotGas = 35;                // Wert : 0-247     // Gaswert bei Empangsverlust
 EE_Parameter.NotGasZeit = 30;            // Wert : 0-247     // Zeit bis auf NotGas geschaltet wird, wg. Rx-Problemen
 EE_Parameter.I_Faktor = 32;
 EE_Parameter.UserParam1 =   0;           // zur freien Verwendung
 EE_Parameter.UserParam2 =   0;           // zur freien Verwendung
 EE_Parameter.UserParam3 =   0;           // zur freien Verwendung
 EE_Parameter.UserParam4 =   0;           // zur freien Verwendung
 EE_Parameter.UserParam5 =   0;           // zur freien Verwendung
 EE_Parameter.UserParam6 =   0;           // zur freien Verwendung
 EE_Parameter.UserParam7 = 0;             // zur freien Verwendung
 EE_Parameter.UserParam8 = 0;             // zur freien Verwendung
 EE_Parameter.ServoNickControl = 100;     // Wert : 0-247     // Stellung des Servos
 EE_Parameter.ServoNickComp = 40;         // Wert : 0-247     // Einfluss Gyro/Servo
 EE_Parameter.ServoCompInvert = 1;        // Wert : 0-247     // Richtung Einfluss Gyro/Servo
 EE_Parameter.ServoNickMin = 0;           // Wert : 0-247     // Anschlag
 EE_Parameter.ServoNickMax = 247;         // Wert : 0-247     // Anschlag
 EE_Parameter.ServoNickRefresh = 6;
 EE_Parameter.Servo3 = 125;
 EE_Parameter.Servo4 = 125;
 EE_Parameter.Servo5 = 125;
 EE_Parameter.ServoRollControl = 100;     // Wert : 0-247     // Stellung des Servos
 EE_Parameter.ServoRollComp = 40;         // Wert : 0-247     // Einfluss Gyro/Servo
 EE_Parameter.ServoRollMin = 0;           // Wert : 0-247     // Anschlag
 EE_Parameter.ServoRollMax = 247;         // Wert : 0-247     // Anschlag
 EE_Parameter.LoopGasLimit = 50;
 EE_Parameter.LoopThreshold = 90;         // Wert: 0-247  Schwelle f�r Stickausschlag
 EE_Parameter.LoopHysterese = 50;
 EE_Parameter.BitConfig = 0;              // Bitcodiert: 0x01=oben, 0x02=unten, 0x04=links, 0x08=rechts
 EE_Parameter.AchsKopplung1 = 90;
 EE_Parameter.AchsKopplung2 = 80;
 EE_Parameter.CouplingYawCorrection = 60;
 EE_Parameter.WinkelUmschlagNick = 85;
 EE_Parameter.WinkelUmschlagRoll = 85;
 EE_Parameter.GyroAccAbgleich = 32;        // 1/k
 EE_Parameter.Driftkomp = 32;
 EE_Parameter.DynamicStability = 75;
 EE_Parameter.J16Bitmask = 95;
 EE_Parameter.J17Bitmask = 243;
 EE_Parameter.WARN_J16_Bitmask = 0xAA;
 EE_Parameter.WARN_J17_Bitmask = 0xAA;
 EE_Parameter.J16Timing = 20;
 EE_Parameter.J17Timing = 20;
 EE_Parameter.NaviGpsModeControl = 254; // 254 -> Poti 2
 EE_Parameter.NaviGpsGain = 100;
 EE_Parameter.NaviGpsP = 90;
 EE_Parameter.NaviGpsI = 90;
 EE_Parameter.NaviGpsD = 90;
 EE_Parameter.NaviGpsPLimit = 75;
 EE_Parameter.NaviGpsILimit = 75;
 EE_Parameter.NaviGpsDLimit = 75;
 EE_Parameter.NaviGpsACC = 0;
 EE_Parameter.NaviGpsMinSat = 6;
 EE_Parameter.NaviStickThreshold = 8;
 EE_Parameter.NaviWindCorrection = 90;
 EE_Parameter.NaviSpeedCompensation = 30;
 EE_Parameter.NaviOperatingRadius = 100;
 EE_Parameter.NaviAngleLimitation = 100;
 EE_Parameter.NaviPH_LoginTime = 4;
 EE_Parameter.Receiver = RECEIVER_SPEKTRUM;
 memcpy(EE_Parameter.Name, "Normal\0", 12);
}

void DefaultKonstanten3(void){
 EE_Parameter.GlobalConfig = /*CFG_DREHRATEN_BEGRENZER  |*/ CFG_ACHSENKOPPLUNG_AKTIV | CFG_KOMPASS_AKTIV | CFG_GPS_AKTIV | CFG_HOEHEN_SCHALTER;
 EE_Parameter.ExtraConfig = CFG2_HEIGHT_LIMIT;// | CFG2_VARIO_BEEP | CFG_SENSITIVE_RC
 EE_Parameter.Hoehe_MinGas = 30;
 EE_Parameter.MaxHoehe     = 255;         // Wert : 0-247   255 -> Poti1
 EE_Parameter.Hoehe_P      = 10;          // Wert : 0-32
 EE_Parameter.Luftdruck_D  = 30;          // Wert : 0-247
 EE_Parameter.Hoehe_ACC_Wirkung = 30;     // Wert : 0-247
 EE_Parameter.Hoehe_HoverBand = 5;           // Wert : 0-247
 EE_Parameter.Hoehe_GPS_Z = 64;           // Wert : 0-247
 EE_Parameter.Hoehe_StickNeutralPoint = 0;// Wert : 0-247 (0 = Hover-Estimation)
 EE_Parameter.Hoehe_Verstaerkung = 15;    // Wert : 0-50
 EE_Parameter.Stick_P = 8;                // Wert : 1-6
 EE_Parameter.Stick_D = 16;               // Wert : 0-64
 EE_Parameter.Gier_P  = 6;                // Wert : 1-20
 EE_Parameter.Gas_Min = 8;                // Wert : 0-32
 EE_Parameter.Gas_Max = 230;              // Wert : 33-247
 EE_Parameter.GyroAccFaktor = 30;         // Wert : 1-64
 EE_Parameter.KompassWirkung = 128;       // Wert : 0-247
 EE_Parameter.Gyro_P = 100;               // Wert : 0-247
 EE_Parameter.Gyro_I = 120;               // Wert : 0-247
 EE_Parameter.Gyro_D = 3;                 // Wert : 0-247
 EE_Parameter.Gyro_Gier_P = 100;          // Wert : 0-247
 EE_Parameter.Gyro_Gier_I = 120;          // Wert : 0-247
 EE_Parameter.UnterspannungsWarnung = 33; // Wert : 0-247 ( Automatische Zellenerkennung bei < 50)
 EE_Parameter.NotGas = 35;                // Wert : 0-247     // Gaswert bei Empangsverlust
 EE_Parameter.NotGasZeit = 20;            // Wert : 0-247     // Zeit bis auf NotGas geschaltet wird, wg. Rx-Problemen
 EE_Parameter.I_Faktor = 16;
 EE_Parameter.UserParam1 =   0;           // zur freien Verwendung
 EE_Parameter.UserParam2 =   0;           // zur freien Verwendung
 EE_Parameter.UserParam3 =   0;           // zur freien Verwendung
 EE_Parameter.UserParam4 =   0;           // zur freien Verwendung
 EE_Parameter.UserParam5 =   0;           // zur freien Verwendung
 EE_Parameter.UserParam6 =   0;           // zur freien Verwendung
 EE_Parameter.UserParam7 = 0;             // zur freien Verwendung
 EE_Parameter.UserParam8 = 0;             // zur freien Verwendung
 EE_Parameter.ServoNickControl = 100;     // Wert : 0-247     // Stellung des Servos
 EE_Parameter.ServoNickComp = 40;         // Wert : 0-247     // Einfluss Gyro/Servo
 EE_Parameter.ServoCompInvert = 1;        // Wert : 0-247     // Richtung Einfluss Gyro/Servo
 EE_Parameter.ServoNickMin = 0;           // Wert : 0-247     // Anschlag
 EE_Parameter.ServoNickMax = 247;         // Wert : 0-247     // Anschlag
 EE_Parameter.ServoNickRefresh = 6;
 EE_Parameter.Servo3 = 125;
 EE_Parameter.Servo4 = 125;
 EE_Parameter.Servo5 = 125;
 EE_Parameter.ServoRollControl = 100;     // Wert : 0-247     // Stellung des Servos
 EE_Parameter.ServoRollComp = 40;         // Wert : 0-247     // Einfluss Gyro/Servo
 EE_Parameter.ServoRollMin = 0;           // Wert : 0-247     // Anschlag
 EE_Parameter.ServoRollMax = 247;         // Wert : 0-247     // Anschlag
 EE_Parameter.LoopGasLimit = 50;
 EE_Parameter.LoopThreshold = 90;         // Wert: 0-247  Schwelle f�r Stickausschlag
 EE_Parameter.LoopHysterese = 50;
 EE_Parameter.BitConfig = 0;               // Bitcodiert: 0x01=oben, 0x02=unten, 0x04=links, 0x08=rechts
 EE_Parameter.AchsKopplung1 = 90;
 EE_Parameter.AchsKopplung2 = 80;
 EE_Parameter.CouplingYawCorrection = 70;
 EE_Parameter.WinkelUmschlagNick = 85;
 EE_Parameter.WinkelUmschlagRoll = 85;
 EE_Parameter.GyroAccAbgleich = 32;        // 1/k
 EE_Parameter.Driftkomp = 32;
 EE_Parameter.DynamicStability = 50;
 EE_Parameter.J16Bitmask = 95;
 EE_Parameter.J17Bitmask = 243;
 EE_Parameter.WARN_J16_Bitmask = 0xAA;
 EE_Parameter.WARN_J17_Bitmask = 0xAA;
 EE_Parameter.J16Timing = 30;
 EE_Parameter.J17Timing = 30;
 EE_Parameter.NaviGpsModeControl = 254; // 254 -> Poti 2
 EE_Parameter.NaviGpsGain = 100;
 EE_Parameter.NaviGpsP = 90;
 EE_Parameter.NaviGpsI = 90;
 EE_Parameter.NaviGpsD = 90;
 EE_Parameter.NaviGpsPLimit = 75;
 EE_Parameter.NaviGpsILimit = 75;
 EE_Parameter.NaviGpsDLimit = 75;
 EE_Parameter.NaviGpsACC = 0;
 EE_Parameter.NaviGpsMinSat = 6;
 EE_Parameter.NaviStickThreshold = 8;
 EE_Parameter.NaviWindCorrection = 90;
 EE_Parameter.NaviSpeedCompensation = 30;
 EE_Parameter.NaviOperatingRadius = 100;
 EE_Parameter.NaviAngleLimitation = 100;
 EE_Parameter.NaviPH_LoginTime = 4;
 EE_Parameter.Receiver = RECEIVER_SPEKTRUM;
 memcpy(EE_Parameter.Name, "Beginner\0", 12);
}
