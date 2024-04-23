/*#######################################################################################
Flight Control
#######################################################################################
//LICENSE:
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + Copyright (c) Holger Buss, Ingo Busker
// + Nur f�r den privaten Gebrauch / NON-COMMERCIAL USE ONLY
// + www.MikroKopter.com
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + Es gilt f�r das gesamte Projekt (Hardware, Software, Bin�rfiles, Sourcecode und Dokumentation),
// + dass eine Nutzung (auch auszugsweise) nur f�r den privaten (nicht-kommerziellen) Gebrauch zul�ssig ist.
// + Sollten direkte oder indirekte kommerzielle Absichten verfolgt werden, ist mit uns (info@mikrokopter.de) Kontakt
// + bzgl. der Nutzungsbedingungen aufzunehmen.
// + Eine kommerzielle Nutzung ist z.B.Verkauf von MikroKoptern, Best�ckung und Verkauf von Platinen oder Baus�tzen,
// + Verkauf von Luftbildaufnahmen, usw.
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + Werden Teile des Quellcodes (mit oder ohne Modifikation) weiterverwendet oder ver�ffentlicht,
// + unterliegen sie auch diesen Nutzungsbedingungen und diese Nutzungsbedingungen incl. Copyright m�ssen dann beiliegen
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + Sollte die Software (auch auszugesweise) oder sonstige Informationen des MikroKopter-Projekts
// + auf anderen Webseiten oder sonstigen Medien ver�ffentlicht werden, muss unsere Webseite "http://www.mikrokopter.de"
// + eindeutig als Ursprung verlinkt werden
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + Keine Gew�hr auf Fehlerfreiheit, Vollst�ndigkeit oder Funktion
// + Benutzung auf eigene Gefahr
// + Wir �bernehmen keinerlei Haftung f�r direkte oder indirekte Personen- oder Sachsch�den
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + Die Portierung oder Nutzung der Software (oder Teile davon) auf andere Systeme (ausser der Hardware von www.mikrokopter.de) ist nur
// + mit unserer Zustimmung zul�ssig
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + Die Funktion printf_P() unterliegt ihrer eigenen Lizenz und ist hiervon nicht betroffen
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + Redistributions of source code (with or without modifications) must retain the above copyright notice,
// + this list of conditions and the following disclaimer.
// +   * Neither the name of the copyright holders nor the names of contributors may be used to endorse or promote products derived
// +     from this software without specific prior written permission.
// +   * The use of this project (hardware, software, binary files, sources and documentation) is only permittet
// +     for non-commercial use (directly or indirectly)
// +     Commercial use (for excample: selling of MikroKopters, selling of PCBs, assembly, ...) is only permitted
// +     with our written permission
// +   * If sources or documentations are redistributet on other webpages, out webpage (http://www.MikroKopter.de) must be
// +     clearly linked as origin
// +   * porting the sources to other systems or using the software on other systems (except hardware from www.mikrokopter.de) is not allowed
// +  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// +  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// +  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// +  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// +  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// +  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// +  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// +  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// +  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// +  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// +  POSSIBILITY OF SUCH DAMAGE.
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// :license */

#include "fc.h"

struct Measurements measurement; //this is where we want to store all the measurement values
struct IntAngles A2DNeutral={0,0,0}; //replaces AdNeutral{Nick,Roll,Gier}
struct MixerStruct Mixer;

unsigned int BaroExpandActive = 0; //used in altitude hold loop
volatile unsigned int I2CTimeout = 100; //used in main.c, twimaster.c
struct IntAnglesPR trim;//replaces Trim{Nick,Roll}
struct IntAnglesPR naviAcc;//replaces NaviAcc{Nick,Roll}
struct IntXYZ meanValueAcc;//replaces Mittlewert_Acc{Nick,Roll,Hoch} //referred to elsewhere...

struct LongAngles measurementAngleIntegral={0,0,0}; //replaces Mess_Integral{Nick,Roll,_Gier}
struct LongAngles measurementAngleIntegral2={0,0,0};//replaces Mess_Integral{Nick,Roll,_Gier}2
struct LongAnglesPR meanAngleIntegral;//replaces MittelIntegral{Nick,Roll}
struct LongAnglesPR meanAngleIntegral2;//replaces MittelIntegral{Nick,Roll}2

struct LongAngles angleIntegral = {0,0,0};    //replaces Integral{Nick,Roll,_Gier}
struct LongAnglesPR angleIntegral2 = {0,0};   //replaces Integral{Nick,Roll}2
struct LongXYZ accelerationIntegral = {0,0,0};//replaces IntegralAcc{Nick,Roll,Z}

struct LongAngles wrapAround180={250000L,250000L};//replaces Umschlag180{Nick,Roll}

struct IntAnglesPR angleDifference; //replaces Diff{Nick,Roll}

struct StickValues stick={0,0,0,0};//replaces Stick{Nick,Roll,Gier,Gas}

struct IntAnglesPR poseCorrection = {0,0};//replaces LageKorrektur{Nick,Roll}

int NaviCntAcc = 0;

int AdNeutralGierBias;

int NeutralAccX=0, NeutralAccY=0;
volatile float NeutralAccZ = 0; //This is a float to allow for a "soft limit" -- integers can't be incremented as smoothly

int  KompassValue = 0;
int  KompassStartwert = 0;
int  KompassRichtung = 0;
unsigned int  KompassSignalSchlecht = 500;
unsigned char maximumGasValue, minimumGasValue;
unsigned char TrichterFlug = 0;
long ErsatzKompass;
int  ErsatzKompassInGrad; // Kompasswert in Grad
int  yawGyroError = 0;
char gyroFactorPR,gyroFactorYaw;
char IntegralFaktor,IntegralFaktorGier;
unsigned char Poti[9] = {0,0,0,0,0,0,0,0};
volatile unsigned char SenderOkay = 0;
volatile unsigned char SenderRSSI = 0;


char vehicleIsFlying = 0;
long currentHeight = 0;
long desiredHeight  = 0; //this isn't used anymore

int Ki = 10300 / 33; //=312

unsigned char Parameter_Luftdruck_D  = 48;      // Wert : 0-250
unsigned char Parameter_MaxHoehe     = 251;     // Wert : 0-250
unsigned char Parameter_Hoehe_P      = 16;      // Wert : 0-32
unsigned char Parameter_Hoehe_ACC_Wirkung = 58; // Wert : 0-250
unsigned char Parameter_KompassWirkung = 64;    // Wert : 0-250
unsigned char Parameter_Hoehe_GPS_Z = 64;       // Wert : 0-250
unsigned char Parameter_Gyro_D = 8;             // Wert : 0-250
unsigned char Parameter_Gyro_P = 150;           // Wert : 10-250
unsigned char Parameter_Gyro_I = 150;           // Wert : 0-250
unsigned char Parameter_Gyro_Gier_P = 150;      // Wert : 10-250
unsigned char Parameter_Gyro_Gier_I = 150;      // Wert : 10-250
unsigned char Parameter_Gier_P = 2;             // Wert : 1-20
unsigned char Parameter_I_Faktor = 10;          // Wert : 1-20
unsigned char Parameter_UserParam1 = 0;
unsigned char Parameter_UserParam2 = 0;
unsigned char Parameter_UserParam3 = 0;
unsigned char Parameter_UserParam4 = 0;
unsigned char Parameter_UserParam5 = 0;
unsigned char Parameter_UserParam6 = 0;
unsigned char Parameter_UserParam7 = 0;
unsigned char Parameter_UserParam8 = 0;
unsigned char Parameter_ServoNickControl = 100;
unsigned char Parameter_ServoRollControl = 100;
unsigned char Parameter_LoopGasLimit = 70;
unsigned char Parameter_AchsKopplung1 = 90;
unsigned char Parameter_AchsKopplung2 = 65;
unsigned char Parameter_CouplingYawCorrection = 64;
unsigned char Parameter_DynamicStability = 100;
unsigned char Parameter_J16Bitmask;             // for the J16 Output
unsigned char Parameter_J16Timing;              // for the J16 Output
unsigned char Parameter_J17Bitmask;             // for the J17 Output
unsigned char Parameter_J17Timing;              // for the J17 Output
unsigned char Parameter_NaviGpsModeControl;     // Parameters for the Naviboard
unsigned char Parameter_NaviGpsGain;
unsigned char Parameter_NaviGpsP;
unsigned char Parameter_NaviGpsI;
unsigned char Parameter_NaviGpsD;
unsigned char Parameter_NaviGpsACC;
unsigned char Parameter_NaviOperatingRadius;
unsigned char Parameter_NaviWindCorrection;
unsigned char Parameter_NaviSpeedCompensation;
unsigned char Parameter_ExternalControl;
unsigned char Parameter_Servo3,Parameter_Servo4,Parameter_Servo5;

struct mk_param_struct EE_Parameter;

unsigned int  vehicleFlyingCounter = 0; //we use this to determine whether the vehicle is flying.
volatile unsigned char FCFlags = 0;
signed int KopplungsteilNickRoll,KopplungsteilRollNick;
char VarioCharacter = ' ';//also used by jetimenu.c

struct IntAnglesPR maxStickAngles={0,0};//replaces MaxStick{Nick,Roll}

const long GIER_GRAD_FAKTOR = 1160L;

//variables for Hardware In the Loop Simulation
volatile enum TypeHILS enableHILS=HILS_REAL_FLIGHT;//are we doing HILS?

volatile struct MotorCommandsHILS motorCommandsHILS;//the motor commands we will use for HILS

volatile unsigned long controllerCycleNumber = 0;//counter, also used in uart.c

int motorSmoothing(int new, int old){//something almost like a low-pass (but not really)
  /*NOTE:
     This isn't really smoothing. When the speed is to increase
     (i.e. new > old) it acts as a low pass filter. If the speed 
     is to decrease, the returned value does *not* lie between the 
     old and new values:
        new < old
     => new - old < 0
     => returnValue = 2*new - old < new < old
    
     TODO: WHY is this implemented like this?! (?Braking?)
   */
  if(new > old){
    return ((old + new) / 2);
  }

  return (2*new - old);
}

void Piep(unsigned char amount, unsigned int duration){
  if(vehicleIsFlying){//never run this in filght: the beeps also cause the fc to stop responding...
    return;
  }

  while(amount--){
    beeptime = duration;
    while(beeptime){//do nothing...
      //Beeptime is a volatile variable, which gets decremented in 
      //"timer0.c" :: ISR(.)
      //This function is labelled as 9.7kHz
    }
    Delay_ms(duration * 2);
  }
}

void SetNeutral(void){//Set zero values
  struct IntAngles neutral={0,0,0};//replaces unsigned int gier_neutral=0, nick_neutral=0, roll_neutral=0;
  HEF4017R_ON;

  NeutralAccX = NeutralAccY = NeutralAccZ = 0;

  A2DNeutral.pitch = A2DNeutral.roll = A2DNeutral.yaw = 0;

  AdNeutralGierBias = 0;
  Parameter_AchsKopplung1 = 0;
  Parameter_AchsKopplung2 = 0;

  calibrateProcessMeasurement();
  Delay_ms_Mess(100);
  calibrateProcessMeasurement();

  if(EE_Parameter.GlobalConfig & CFG_HOEHENREGELUNG){// H�henregelung aktiviert?
    if((airPressureMeasurement > 950) || (airPressureMeasurement < 750)){
      findAirpressureOffset();
    }
  }

  const int NEUTRAL_FILTER=32;
  for(unsigned char i=0; i<NEUTRAL_FILTER; i++){
    Delay_ms_Mess(10);
    neutral.yaw   += A2DValue.yaw;
    neutral.pitch += A2DValue.pitch;
    neutral.roll  += A2DValue.roll;
  }
  A2DNeutral.pitch = (neutral.pitch + NEUTRAL_FILTER/2)/(NEUTRAL_FILTER/8);
  A2DNeutral.roll  = (neutral.roll  + NEUTRAL_FILTER/2)/(NEUTRAL_FILTER/8);
  A2DNeutral.yaw   = (neutral.yaw   + NEUTRAL_FILTER/2)/(NEUTRAL_FILTER);
  AdNeutralGierBias = A2DNeutral.yaw;

  if(eeprom_read_byte(&EEPromArray[EEPROM_ADR_ACC_NICK]) > 4){
    NeutralAccY = abs(meanValueAcc.y) / (2*ACC_AMPLIFY);
    NeutralAccX = abs(meanValueAcc.x) / (2*ACC_AMPLIFY);
    NeutralAccZ = currentAcc.z;
  }
  else{
    NeutralAccX = (int)eeprom_read_byte(&EEPromArray[EEPROM_ADR_ACC_NICK]) * 256 + (int)eeprom_read_byte(&EEPromArray[EEPROM_ADR_ACC_NICK+1]);
    NeutralAccY = (int)eeprom_read_byte(&EEPromArray[EEPROM_ADR_ACC_ROLL]) * 256 + (int)eeprom_read_byte(&EEPromArray[EEPROM_ADR_ACC_ROLL+1]);
    NeutralAccZ = (int)eeprom_read_byte(&EEPromArray[EEPROM_ADR_ACC_Z])    * 256 + (int)eeprom_read_byte(&EEPromArray[EEPROM_ADR_ACC_Z+1]);
  }

  measurement.processed.pitch = 0;
  measurement.processed.roll  = 0;
  measurement.processed.yaw   = 0;
  Delay_ms_Mess(100);
  meanValueAcc.x = ACC_AMPLIFY * (long)A2DValueAcc.x;
  meanValueAcc.y = ACC_AMPLIFY * (long)A2DValueAcc.y;

  angleIntegral.pitch = EE_Parameter.GyroAccFaktor * (long)meanValueAcc.x;
  angleIntegral.roll  = EE_Parameter.GyroAccFaktor * (long)meanValueAcc.y;

  measurementAngleIntegral2.pitch = angleIntegral.pitch;
  measurementAngleIntegral2.roll  = angleIntegral.roll;
  measurementAngleIntegral.yaw = 0;

  airPressureStart = airPressure;
  VarioMeter = 0;
  KompassStartwert = KompassValue;
  GPS_Neutral();
  beeptime = 50;
  wrapAround180.pitch = ((long) EE_Parameter.WinkelUmschlagNick * 2500L) + 15000L;
  wrapAround180.roll  = ((long) EE_Parameter.WinkelUmschlagRoll * 2500L) + 15000L;
  ErsatzKompass = KompassValue * GIER_GRAD_FAKTOR;
  yawGyroError = 0;
  SendVersionToNavi = 1;
  LED_Init();
  FCFlags |= FCFLAG_CALIBRATE;
  FromNaviCtrl_Value.Kalman_K = -1;
  FromNaviCtrl_Value.Kalman_MaxDrift = 0;
  FromNaviCtrl_Value.Kalman_MaxFusion = 32;

  for(unsigned char i=0;i<8;i++){
    Poti[i] =    PPM_in[EE_Parameter.Kanalbelegung[CH_POTI1 + i]] + 110;
  }

  SenderOkay = 100;
  if(ServoActive){
    HEF4017R_ON;
    DDRD  |=0x80; // enable J7 -> Servo signal
  }
}

void processMeasurement(void){//Process measurement values (executed once per controller call)
  static signed long tmpl,tmpl2,tmpl3,tmpl4;
  static signed int oldNick, oldRoll, d2Roll, d2Nick;
  signed long winkel_nick, winkel_roll;
  unsigned char i;

  //these are the raw values that we measured
  measurement.raw.yaw   = (signed int) A2DNeutral.yaw - A2DValue.yaw;
  measurement.raw.pitch = (signed int) A2DFilter.pitch / 8;
  measurement.raw.roll  = (signed int) A2DFilter.roll / 8;

  measurement.processed.yaw   = measurement.raw.yaw;
  measurement.processed.pitch = measurement.raw.pitch;
  measurement.processed.roll  = measurement.raw.roll;

  // Beschleunigungssensor  ++++++++++++++++++++++++++++++++++++++++++++++++
  meanValueAcc.x = ((long)meanValueAcc.x * 3 + ((ACC_AMPLIFY * (long)A2DValueAcc.x))) / 4L;
  meanValueAcc.y = ((long)meanValueAcc.y * 3 + ((ACC_AMPLIFY * (long)A2DValueAcc.y))) / 4L;
  meanValueAcc.z = ((long)meanValueAcc.z * 3 + ((long)A2DValueAcc.z)) / 4L;

  accelerationIntegral.x += ACC_AMPLIFY * A2DValueAcc.x;
  accelerationIntegral.y += ACC_AMPLIFY * A2DValueAcc.y;
  accelerationIntegral.z += currentAcc.z - NeutralAccZ;

  naviAcc.pitch += A2DValueAcc.x;
  naviAcc.roll  += A2DValueAcc.y;

  NaviCntAcc += 1;

  //++++++++++++++++++++++++++++++++++++++++++++++++
  // ADC einschalten
  ANALOG_ON;
  AdReady = 0;
  //++++++++++++++++++++++++++++++++++++++++++++++++

  winkel_roll = measurementAngleIntegral.roll;
  LIMIT_SYMMETRIC(winkel_roll,93000L);

  winkel_nick = measurementAngleIntegral.pitch;
  LIMIT_SYMMETRIC(winkel_roll,93000L);

  // Gier  ++++++++++++++++++++++++++++++++++++++++++++++++
  measurementAngleIntegral.yaw += measurement.processed.yaw;
  ErsatzKompass += measurement.processed.yaw;
  // Kopplungsanteil  +++++++++++++++++++++++++++++++++++++
  if(EE_Parameter.GlobalConfig & CFG_ACHSENKOPPLUNG_AKTIV){
    tmpl3 = (measurement.processed.roll * winkel_nick) / 2048L;
    tmpl3 *= Parameter_AchsKopplung2; //65
    tmpl3 /= 4096L;
    tmpl4 = (measurement.processed.pitch * winkel_roll) / 2048L;
    tmpl4 *= Parameter_AchsKopplung2; //65
    tmpl4 /= 4096L;
    KopplungsteilNickRoll = tmpl3;
    KopplungsteilRollNick = tmpl4;
    tmpl4 -= tmpl3;
    ErsatzKompass += tmpl4;
    if(!Parameter_CouplingYawCorrection){
      measurementAngleIntegral.yaw -= tmpl4/2; // Gier nachhelfen
    }

    tmpl = ((measurement.processed.yaw + tmpl4) * winkel_nick) / 2048L;
    tmpl *= Parameter_AchsKopplung1;  // 90
    tmpl /= 4096L;
    tmpl2 = ((measurement.processed.yaw + tmpl4) * winkel_roll) / 2048L;
    tmpl2 *= Parameter_AchsKopplung1;
    tmpl2 /= 4096L;
    if(abs(measurement.processed.yaw) > 64){
      if(labs(tmpl) > 128 || labs(tmpl2) > 128){
        TrichterFlug = 1;
      }
    }
  }
  else{
    tmpl = tmpl2 = KopplungsteilNickRoll = KopplungsteilRollNick = 0;
  }

  //mwm: TODO: Does this make sense? Order of calculation, etc???
  trim.roll  =  tmpl - tmpl2 / 100L;
  trim.pitch = -tmpl2 + tmpl / 100L;

  // Kompasswert begrenzen  ++++++++++++++++++++++++++++++++++++++++++++++++
  if(ErsatzKompass >= (360L * GIER_GRAD_FAKTOR)){
    ErsatzKompass -= 360L * GIER_GRAD_FAKTOR;  // 360� Umschlag
  }
  if(ErsatzKompass < 0){
    ErsatzKompass += 360L * GIER_GRAD_FAKTOR;
  }
  // Roll  ++++++++++++++++++++++++++++++++++++++++++++++++
  measurementAngleIntegral2.roll += measurement.processed.roll + trim.roll;
  measurementAngleIntegral.roll  +=  measurement.processed.roll + trim.roll - poseCorrection.roll;
  if(measurementAngleIntegral.roll > wrapAround180.roll){
    measurementAngleIntegral.roll  = -(wrapAround180.roll - 25000L);
    measurementAngleIntegral2.roll = measurementAngleIntegral.roll;
  }
  if(measurementAngleIntegral.roll <-wrapAround180.roll){
    measurementAngleIntegral.roll =  (wrapAround180.roll - 25000L);
    measurementAngleIntegral2.roll = measurementAngleIntegral.roll;
  }
  // Nick  ++++++++++++++++++++++++++++++++++++++++++++++++
  measurementAngleIntegral2.pitch += measurement.processed.pitch + trim.pitch;
  measurementAngleIntegral.pitch  += measurement.processed.pitch + trim.pitch - poseCorrection.pitch;
  if(measurementAngleIntegral.pitch > wrapAround180.pitch){
    measurementAngleIntegral.pitch = -(wrapAround180.pitch - 25000L);
    measurementAngleIntegral2.pitch = measurementAngleIntegral.pitch;
  }
  if(measurementAngleIntegral.pitch <-wrapAround180.pitch){
    measurementAngleIntegral.pitch =  (wrapAround180.pitch - 25000L);
    measurementAngleIntegral2.pitch = measurementAngleIntegral.pitch;
  }

  angleIntegral.yaw    = measurementAngleIntegral.yaw;
  angleIntegral.pitch  = measurementAngleIntegral.pitch;
  angleIntegral.roll   = measurementAngleIntegral.roll;
  angleIntegral2.pitch = measurementAngleIntegral2.pitch;
  angleIntegral2.roll  = measurementAngleIntegral2.roll;

  measurement.processed.pitch = hiRes.pitch / 8;
  measurement.processed.roll  = hiRes.roll  / 8;

  //The below cases violate the ADC ranges (re: Konrad's files)
  if(A2DValue.pitch < 15)   measurement.processed.pitch = -1000;  
  if(A2DValue.pitch <  7)   measurement.processed.pitch = -2000;
  if(A2DValue.pitch > 2000) measurement.processed.pitch = +1000;  
  if(A2DValue.pitch > 2015) measurement.processed.pitch = +2000; 
  if(A2DValue.roll  < 15)   measurement.processed.roll  = -1000; 
  if(A2DValue.roll  <  7)   measurement.processed.roll  = -2000;
  if(A2DValue.roll  > 2000) measurement.processed.roll  = +1000;  
  if(A2DValue.roll  > 2015) measurement.processed.roll  = +2000;  

  if(Parameter_Gyro_D){
    const int D_LIMIT=128;

    d2Nick = hiRes.pitch - oldNick;
    oldNick = (oldNick + hiRes.pitch)/2;
    LIMIT_SYMMETRIC(d2Nick,D_LIMIT);
    measurement.processed.pitch += (d2Nick * (signed int) Parameter_Gyro_D) / 16;
    d2Roll = hiRes.roll - oldRoll;
    oldRoll = (oldRoll + hiRes.roll)/2;
    LIMIT_SYMMETRIC(d2Roll,D_LIMIT);
    measurement.processed.roll += (d2Roll * (signed int) Parameter_Gyro_D) / 16;
    hiRes.pitch += (d2Nick * (signed int) Parameter_Gyro_D);
    hiRes.roll  += (d2Roll * (signed int) Parameter_Gyro_D);
  }

  if(measurement.raw.roll > 0){
    trim.roll  += ((long) abs(KopplungsteilNickRoll) * Parameter_CouplingYawCorrection) / 64L;
  }
  else{
    trim.roll  -= ((long) abs(KopplungsteilNickRoll) * Parameter_CouplingYawCorrection) / 64L;
  }

  if(measurement.raw.pitch > 0){
    trim.pitch += ((long) abs(KopplungsteilRollNick) * Parameter_CouplingYawCorrection) / 64L;
  }
  else{
    trim.pitch -= ((long) abs(KopplungsteilRollNick) * Parameter_CouplingYawCorrection) / 64L;
  }

  if(EE_Parameter.GlobalConfig & CFG_DREHRATEN_BEGRENZER){
    if(measurement.raw.pitch > 256){
      measurement.processed.pitch += (measurement.raw.pitch - 256);
    }
    else if(measurement.raw.pitch < -256){
      measurement.processed.pitch += (measurement.raw.pitch + 256);
    }
    if(measurement.raw.roll > 256){
      measurement.processed.roll  += (measurement.raw.roll - 256);
    }
    else if(measurement.raw.roll < -256){
      measurement.processed.roll  += (measurement.raw.roll + 256);
    }
  }

  for(i=0;i<8;i++){
    int tmp;
    tmp = PPM_in[EE_Parameter.Kanalbelegung[CH_POTI1 + i]] + 110;
    LIMIT_MIN_MAX(tmp,0,255);
    if(tmp != Poti[i]){
      Poti[i] += (tmp - Poti[i]) / 8;
      if(Poti[i] > tmp){
        Poti[i]--;
      }
      else{
        Poti[i]++;
      }
    }
  }
}

void calibrateProcessMeasurement(void){//Measurement values when calculating the zero position
  ANALOG_OFF;// Disable ADC, so the values don't change during calculation
  measurement.processed.pitch = A2DValue.pitch;
  measurement.processed.roll  = A2DValue.roll;
  measurement.processed.yaw   = A2DValue.yaw;
  meanValueAcc.x = ACC_AMPLIFY * (long)A2DValueAcc.x;
  meanValueAcc.y = ACC_AMPLIFY * (long)A2DValueAcc.y;
  meanValueAcc.z = (long)A2DValueAcc.z;
  ANALOG_ON;// Re-enable ADC
  for(unsigned char i=0;i<8;i++){
    int tmp = PPM_in[EE_Parameter.Kanalbelegung[CH_POTI1 + i]] + 110;
    LIMIT_MIN_MAX(tmp, 0, 255);
    if(Poti[i] > tmp){
      Poti[i]--;  
    }
    else if(Poti[i] < tmp){
      Poti[i]++;
    }
  }
  wrapAround180.pitch = (long) EE_Parameter.WinkelUmschlagNick * 2500L;
  wrapAround180.roll  = (long) EE_Parameter.WinkelUmschlagRoll * 2500L;
}

void sendMotorData(void){//Send motor commands via I2C bus
  unsigned char i;
  if(!vehicleIsFlying){
    //vehicle isn't flying
    FCFlags &= ~(FCFLAG_MOTOR_RUN | FCFLAG_FLY);
    for(i=0;i<MAX_MOTORS;i++){
      if(!PC_MotortestActive){
        MotorTest[i] = 0;
      }
      Motor[i].SetPoint = MotorTest[i];
    }
    if(PC_MotortestActive){
      PC_MotortestActive--;
    }
  }
  else {//vehicle is flying
    FCFlags |= FCFLAG_MOTOR_RUN;
  }

  //write motor commands to the debug packet
  DebugOut.Analog[12] = Motor[0].SetPoint;
  DebugOut.Analog[13] = Motor[1].SetPoint;
  DebugOut.Analog[14] = Motor[2].SetPoint;
  DebugOut.Analog[15] = Motor[3].SetPoint;
  DebugOut.Analog[16] = Motor[4].SetPoint;
  DebugOut.Analog[17] = Motor[5].SetPoint;

  //Start I2C Interrupt Mode
  twi_state = 0;
  motor = 0;
  i2c_start();
}

void assignParameters(void){//Enters Poti values as parameters, as needed
#define CHK_POTI(b,a) {if(a < 248) b = a; else b = Poti[255 - a];}
#define CHK_POTI_MIN(b,a,min) {CHK_POTI(b,a); LIMIT_MIN(b, min);}
#define CHK_POTI_MAX(b,a,max) {CHK_POTI(b,a); LIMIT_MAX(b, max);}

  CHK_POTI_MAX(Parameter_Luftdruck_D,EE_Parameter.Luftdruck_D,100);
  CHK_POTI_MAX(Parameter_Hoehe_P,EE_Parameter.Hoehe_P,100);
  CHK_POTI_MIN(Parameter_Gyro_P,EE_Parameter.Gyro_P,10);
  CHK_POTI_MIN(Parameter_J16Timing,EE_Parameter.J16Timing,1);
  CHK_POTI_MIN(Parameter_J17Timing,EE_Parameter.J17Timing,1);
  CHK_POTI(Parameter_Servo3,EE_Parameter.Servo3);
  CHK_POTI(Parameter_Servo4,EE_Parameter.Servo4);
  CHK_POTI(Parameter_Servo5,EE_Parameter.Servo5);
  CHK_POTI(Parameter_MaxHoehe,EE_Parameter.MaxHoehe);
  CHK_POTI(Parameter_MaxHoehe,EE_Parameter.MaxHoehe);
  CHK_POTI(Parameter_Hoehe_ACC_Wirkung,EE_Parameter.Hoehe_ACC_Wirkung);
  CHK_POTI(Parameter_Hoehe_GPS_Z,EE_Parameter.Hoehe_GPS_Z);
  CHK_POTI(Parameter_KompassWirkung,EE_Parameter.KompassWirkung);
  CHK_POTI(Parameter_Gyro_I,EE_Parameter.Gyro_I);
  CHK_POTI(Parameter_Gyro_D,EE_Parameter.Gyro_D);
  CHK_POTI(Parameter_Gyro_Gier_P,EE_Parameter.Gyro_Gier_P);
  CHK_POTI(Parameter_Gyro_Gier_I,EE_Parameter.Gyro_Gier_I);
  CHK_POTI(Parameter_I_Faktor,EE_Parameter.I_Faktor);
  CHK_POTI(Parameter_UserParam1,EE_Parameter.UserParam1);
  CHK_POTI(Parameter_UserParam2,EE_Parameter.UserParam2);
  CHK_POTI(Parameter_UserParam3,EE_Parameter.UserParam3);
  CHK_POTI(Parameter_UserParam4,EE_Parameter.UserParam4);
  CHK_POTI(Parameter_UserParam5,EE_Parameter.UserParam5);
  CHK_POTI(Parameter_UserParam6,EE_Parameter.UserParam6);
  CHK_POTI(Parameter_UserParam7,EE_Parameter.UserParam7);
  CHK_POTI(Parameter_UserParam8,EE_Parameter.UserParam8);
  CHK_POTI(Parameter_ServoNickControl,EE_Parameter.ServoNickControl);
  CHK_POTI(Parameter_ServoRollControl,EE_Parameter.ServoRollControl);
  CHK_POTI(Parameter_LoopGasLimit,EE_Parameter.LoopGasLimit);
  CHK_POTI(Parameter_AchsKopplung1,    EE_Parameter.AchsKopplung1);
  CHK_POTI(Parameter_AchsKopplung2,    EE_Parameter.AchsKopplung2);
  CHK_POTI(Parameter_CouplingYawCorrection,EE_Parameter.CouplingYawCorrection);
  CHK_POTI(Parameter_DynamicStability,EE_Parameter.DynamicStability);
  CHK_POTI(Parameter_ExternalControl,EE_Parameter.ExternalControl);
  Ki = 10300 / (Parameter_I_Faktor + 1);
  maximumGasValue = EE_Parameter.Gas_Max;
  minimumGasValue = EE_Parameter.Gas_Min;
}

void motorController(void){//Run the controller at ~500Hz
  int tmp_int;

  struct MixerValues mixerValues;//replaces pd_ergebnis_{nick,roll}, MixerPart{Yaw,Gas}

  struct IntAnglesPR integralMultiplicationFactor;//replaces Integral{Nick,Roll}MalFaktor

  unsigned char i;

  static struct LongAnglesPR sum = {0,0};//replaces Summe{Nick,Roll}
  static long desiredYaw = 0; //probably need not be static, nor long: could just be int
  static long tmp_long, tmp_long2;
  static unsigned int RcLostTimer;
  static unsigned char delayNeutral = 0;
  static unsigned char delaySwitchOn = 0, delaySwitchOff = 0;
  static char timerDebugDataOut = 0;
  static char NeueKompassRichtungMerken = 0;
  static struct LongAnglesPR integralError = {0,0};//replaces IntegralFehler{Nick,Roll}
  static struct LongAnglesPR balancing; //replaces ausgleich{Nick,Roll}

  processMeasurement();//get the A2D measurements
  GRN_ON;
  //--Get gas value----------
  mixerValues.gas = stick.gas;
  LIMIT_MIN(mixerValues.gas, (minimumGasValue + 10));

  controllerCycleNumber++;

  if(haveDisabledHILS){//we should reset the coutners, avoids unexpected starting of the motors
    haveDisabledHILS = 0;
    vehicleIsFlying = 0;
    SenderOkay = 100;
  }

  //--Check reception--------
  if(SenderOkay < 100){//reception is bad...
    if(RcLostTimer){
      RcLostTimer--;
    }
    else{
      vehicleIsFlying = 0;
      FCFlags &= ~FCFLAG_EMERGENCY_LANDING;
    }

    ROT_ON;

    if(vehicleFlyingCounter > 1000){//we've lost reception while in flight
      mixerValues.gas = EE_Parameter.NotGas;
      FCFlags |= FCFLAG_EMERGENCY_LANDING;
      PPM_diff[EE_Parameter.Kanalbelegung[CH_PITCH]] = 0;
      PPM_diff[EE_Parameter.Kanalbelegung[CH_ROLL]] = 0;
      PPM_in[EE_Parameter.Kanalbelegung[CH_PITCH]] = 0;
      PPM_in[EE_Parameter.Kanalbelegung[CH_ROLL ]] = 0;
      PPM_in[EE_Parameter.Kanalbelegung[CH_YAW  ]] = 0;
    }
    else{//vehicle is not flying
      vehicleIsFlying = 0;
    }
  }
  else if(SenderOkay > 140){//reception is good
    FCFlags &= ~FCFLAG_EMERGENCY_LANDING;
    RcLostTimer = EE_Parameter.NotGasZeit * 50;
    if(mixerValues.gas > 40 && vehicleIsFlying){
      if(vehicleFlyingCounter < 0xffff){//0xffff = 65535 ?=max int value
        vehicleFlyingCounter++;
      }
    }

    if(vehicleFlyingCounter < 256){//not flying yet
      sum.pitch = 0;
      sum.roll = 0;
      if(vehicleFlyingCounter == 250){
        NeueKompassRichtungMerken = 1;
        desiredYaw = 0;
        measurementAngleIntegral.yaw = 0;
      }
    }
    else{//flying
      FCFlags |= FCFLAG_FLY;
    }

    if((PPM_in[EE_Parameter.Kanalbelegung[CH_GAS]] > 80) && vehicleIsFlying == 0){
      //--Calibrate to zero values-----
      if(PPM_in[EE_Parameter.Kanalbelegung[CH_YAW]] > 75){// Neutralwerte
        if(++delayNeutral > 200){// nicht sofort
          GRN_OFF;
          vehicleIsFlying = 0;
          delayNeutral = 0;
          vehicleFlyingCounter = 0;
          if(PPM_in[EE_Parameter.Kanalbelegung[CH_PITCH]] > 70 || abs(PPM_in[EE_Parameter.Kanalbelegung[CH_ROLL]]) > 70){
            unsigned char setting=1;
            if(PPM_in[EE_Parameter.Kanalbelegung[CH_ROLL]] > 70 && PPM_in[EE_Parameter.Kanalbelegung[CH_PITCH]] < 70){//right stick @ ?
              setting = 1;
            }
            if(PPM_in[EE_Parameter.Kanalbelegung[CH_ROLL]] > 70 && PPM_in[EE_Parameter.Kanalbelegung[CH_PITCH]] > 70){//right stick @ ?
              setting = 2;
            }
            if(PPM_in[EE_Parameter.Kanalbelegung[CH_ROLL]] < 70 && PPM_in[EE_Parameter.Kanalbelegung[CH_PITCH]] > 70){//right stick @ ?
              setting = 3;
            }
            if(PPM_in[EE_Parameter.Kanalbelegung[CH_ROLL]] <-70 && PPM_in[EE_Parameter.Kanalbelegung[CH_PITCH]] > 70){//right stick @ ?
              setting = 4;
            }
            if(PPM_in[EE_Parameter.Kanalbelegung[CH_ROLL]] <-70 && PPM_in[EE_Parameter.Kanalbelegung[CH_PITCH]] < 70){//right stick @ ?
              setting = 5;
            }
            SetActiveParamSetNumber(setting);  // aktiven Datensatz merken
          }

          if(abs(PPM_in[EE_Parameter.Kanalbelegung[CH_ROLL]]) < 30 && PPM_in[EE_Parameter.Kanalbelegung[CH_PITCH]] < -70){
            WinkelOut.CalcState = 1;
            beeptime = 1000;
          }
          else{
            ReadParameterSet(GetActiveParamSetNumber(), (unsigned char *) &EE_Parameter.Kanalbelegung[0], STRUCT_PARAM_LAENGE);
            LipoDetection(0);
            LIBFC_ReceiverInit();
            if(EE_Parameter.GlobalConfig & CFG_HOEHENREGELUNG){// H�henregelung aktiviert?
             if((airPressureMeasurement > 950) || (airPressureMeasurement < 750)){
               findAirpressureOffset();
             }
            }
            ServoActive = 0;
            SetNeutral();
            ServoActive = 1;
            DDRD  |=0x80; // enable J7 -> Servo signal
            Piep(GetActiveParamSetNumber(),120);
          }
        }
      }
      else if(PPM_in[EE_Parameter.Kanalbelegung[CH_YAW]] < -75){// ACC Neutralwerte speichern
        if(++delayNeutral > 200){// nicht sofort
          GRN_OFF;
          eeprom_write_byte(&EEPromArray[EEPROM_ADR_ACC_NICK],0xff); // Werte l�schen
          vehicleIsFlying = 0;
          delayNeutral = 0;
          vehicleFlyingCounter = 0;
          SetNeutral();
          eeprom_write_byte(&EEPromArray[EEPROM_ADR_ACC_NICK],NeutralAccX / 256); // ACC-NeutralWerte speichern
          eeprom_write_byte(&EEPromArray[EEPROM_ADR_ACC_NICK+1],NeutralAccX % 256); // ACC-NeutralWerte speichern
          eeprom_write_byte(&EEPromArray[EEPROM_ADR_ACC_ROLL],NeutralAccY / 256);
          eeprom_write_byte(&EEPromArray[EEPROM_ADR_ACC_ROLL+1],NeutralAccY % 256);
          eeprom_write_byte(&EEPromArray[EEPROM_ADR_ACC_Z],(int)NeutralAccZ / 256);
          eeprom_write_byte(&EEPromArray[EEPROM_ADR_ACC_Z+1],(int)NeutralAccZ % 256);
          Piep(GetActiveParamSetNumber(),120);
        }
      }
      else{
        delayNeutral = 0;
      }
    }
    //--Gas is at minimum------------
    if(PPM_in[EE_Parameter.Kanalbelegung[CH_GAS]] < (35-120)){
      // Motoren Starten
      if(!vehicleIsFlying){
        if(PPM_in[EE_Parameter.Kanalbelegung[CH_YAW]] < -75){//--Switch on--------------------
          if(++delaySwitchOn > 200){
            delaySwitchOn = 0;
            vehicleFlyingCounter = 1;
            vehicleIsFlying = 1;
            desiredYaw = 0;
            measurementAngleIntegral.yaw = 0;
            measurementAngleIntegral2.yaw = 0;
            measurementAngleIntegral.pitch = EE_Parameter.GyroAccFaktor * (long)meanValueAcc.x;
            measurementAngleIntegral.roll  = EE_Parameter.GyroAccFaktor * (long)meanValueAcc.y;
            measurementAngleIntegral2.pitch = angleIntegral.pitch;
            measurementAngleIntegral2.roll = angleIntegral.roll;
            sum.pitch = 0;
            sum.roll = 0;
            FCFlags |= FCFLAG_START;

            sprintf(markDebugString,"Motors started");
          }
        }
        else{
          delaySwitchOn = 0;
        }
      }
      else{// only if motors are running
        //--Switch off-------------------
        if(PPM_in[EE_Parameter.Kanalbelegung[CH_YAW]] > 75){
          if(++delaySwitchOff > 200){// nicht sofort
            vehicleIsFlying = 0;
            delaySwitchOff = 0;
            vehicleFlyingCounter = 0;

            sprintf(markDebugString,"Motors stopped");
          }
        }
        else{
          delaySwitchOff = 0;
        }
      }
    }
  }//end of good reception

  //--New remote values------------
  if(!NewPpmData-- || (FCFlags & FCFLAG_EMERGENCY_LANDING)){
    static int stick_nick,stick_roll;
    assignParameters();
    stick_nick  =  (stick_nick * 3 + PPM_in[EE_Parameter.Kanalbelegung[CH_PITCH]] * EE_Parameter.Stick_P) / 4;
    stick_nick  += PPM_diff[EE_Parameter.Kanalbelegung[CH_PITCH]] * EE_Parameter.Stick_D;
    stick.pitch =  stick_nick - (GPS_Nick + GPS_Nick2);

    stick_roll =  (stick_roll * 3 + PPM_in[EE_Parameter.Kanalbelegung[CH_ROLL]] * EE_Parameter.Stick_P) / 4;
    stick_roll += PPM_diff[EE_Parameter.Kanalbelegung[CH_ROLL]] * EE_Parameter.Stick_D;
    stick.roll =  stick_roll - (GPS_Roll + GPS_Roll2);

    stick.yaw = -PPM_in[EE_Parameter.Kanalbelegung[CH_YAW]];
    if(stick.yaw > 2){
      stick.yaw -= 2;     
    }
    else if(stick.yaw < -2){
      stick.yaw += 2; 
    }
    else{
      stick.yaw = 0;
    }

    stick.gas  = PPM_in[EE_Parameter.Kanalbelegung[CH_GAS]] + 120;

    gyroFactorPR       = (Parameter_Gyro_P + 10.0);
    gyroFactorYaw      = (Parameter_Gyro_Gier_P + 10.0);

    IntegralFaktor     = Parameter_Gyro_I;
    IntegralFaktorGier = Parameter_Gyro_Gier_I;

    //--Analogue control via serial--
    if(ExternControl.Config & 0x01 && Parameter_ExternalControl > 128){
      stick.pitch += (int) ExternControl.Nick * (int) EE_Parameter.Stick_P;
      stick.roll  += (int) ExternControl.Roll * (int) EE_Parameter.Stick_P;
      stick.yaw   += ExternControl.Gier;
      //ExternHoehenValue  = (int) ExternControl.Hight * (int)EE_Parameter.Hoehe_Verstaerkung;
      LIMIT_MAX(stick.gas, ExternControl.Gas);
    }

    LIMIT_MIN(stick.gas, 0);

    if(EE_Parameter.GlobalConfig & CFG_HEADING_HOLD){
      IntegralFaktor =  0;
    }

    if(abs(stick.pitch/STICK_GAIN) > maxStickAngles.pitch){
      maxStickAngles.pitch = abs(stick.pitch)/STICK_GAIN;
      LIMIT_MAX(maxStickAngles.pitch,100);
    }
    else{
      maxStickAngles.pitch--;
    }

    if(abs(stick.roll/STICK_GAIN) > maxStickAngles.roll){
      maxStickAngles.roll = abs(stick.roll)/STICK_GAIN;
      LIMIT_MAX(maxStickAngles.roll,100);
    }
    else{
      maxStickAngles.roll--;
    }

    if(FCFlags & FCFLAG_EMERGENCY_LANDING){
      maxStickAngles.pitch = 0; 
      maxStickAngles.roll = 0;
    }
  } // Ende neue Funken-Werte


  //--Loss of signal during flight-
  if(FCFlags & FCFLAG_EMERGENCY_LANDING){//emergency landing
    stick.yaw      = 0;
    stick.pitch    = 0;
    stick.roll     = 0;
    IntegralFaktor = 120;

    gyroFactorPR       = 90;
    gyroFactorYaw      = 90;
    IntegralFaktorGier = 120;
  }


  //--Balance ACC integrals--------

  meanAngleIntegral.pitch  += angleIntegral.pitch;    // F�r die Mittelwertbildung aufsummieren
  meanAngleIntegral.roll   += angleIntegral.roll;
  meanAngleIntegral2.pitch += angleIntegral2.pitch;
  meanAngleIntegral2.roll  += angleIntegral2.roll;

  //-------------------------------
  if(currentAcc.z > 512 || vehicleIsFlying){//calculate measurementAngleIntegral
    //(currentAcc.z ==512) => free fall (when body-z aligns with grav.)
    // => it should be impossible to achieve currentAcc.z < 512 since thrust is always >=0
    long tmp_long, tmp_long2;
    if(FromNaviCtrl_Value.Kalman_K != -1){
      tmp_long = (long)(angleIntegral.pitch / EE_Parameter.GyroAccFaktor - (long)meanValueAcc.x);
      tmp_long2 = (long)(angleIntegral.roll / EE_Parameter.GyroAccFaktor - (long)meanValueAcc.y);
      tmp_long  = (tmp_long  * FromNaviCtrl_Value.Kalman_K) / (32 * 16);
      tmp_long2 = (tmp_long2 * FromNaviCtrl_Value.Kalman_K) / (32 * 16);
      if((maxStickAngles.pitch > 64) || (maxStickAngles.roll > 64)){
        tmp_long  /= 2;
        tmp_long2 /= 2;
      }

      if(abs(PPM_in[EE_Parameter.Kanalbelegung[CH_YAW]]) > 25){
        tmp_long  /= 3;
        tmp_long2 /= 3;
      }

      LIMIT_SYMMETRIC(tmp_long,  (long)FromNaviCtrl_Value.Kalman_MaxFusion);
      LIMIT_SYMMETRIC(tmp_long2, (long)FromNaviCtrl_Value.Kalman_MaxFusion);
    }
    else{
      tmp_long  = (long)(angleIntegral.pitch / EE_Parameter.GyroAccFaktor - (long)meanValueAcc.x);
      tmp_long2 = (long)(angleIntegral.roll  / EE_Parameter.GyroAccFaktor - (long)meanValueAcc.y);
      tmp_long  /= 16;
      tmp_long2 /= 16;
      if((maxStickAngles.pitch > 64) || (maxStickAngles.roll > 64)){
        tmp_long  /= 3;
        tmp_long2 /= 3;
      }
      if(abs(PPM_in[EE_Parameter.Kanalbelegung[CH_YAW]]) > 25){
        tmp_long  /= 3;
        tmp_long2 /= 3;
      }

      const long AUSGLEICH=32;
      LIMIT_SYMMETRIC(tmp_long, AUSGLEICH);
      LIMIT_SYMMETRIC(tmp_long2,AUSGLEICH);
    }

    measurementAngleIntegral.pitch -= tmp_long;
    measurementAngleIntegral.roll  -= tmp_long2;
  }

  //-------------------------------
  static const long NUMBER_OF_MEASUREMENTS_FOR_BALANCING = 256;
  if(measurementCount >= NUMBER_OF_MEASUREMENTS_FOR_BALANCING){
    static int cnt = 0;
    static char last_n_p,last_n_n,last_r_p,last_r_n;
    static struct LongAnglesPR meanAngleIntegralOld; //replaces MittelIntegral{Nick,Roll}_Alt
    if(!TrichterFlug && EE_Parameter.Driftkomp){
      meanAngleIntegral.pitch  /= NUMBER_OF_MEASUREMENTS_FOR_BALANCING;
      meanAngleIntegral.roll   /= NUMBER_OF_MEASUREMENTS_FOR_BALANCING;
      accelerationIntegral.x = (EE_Parameter.GyroAccFaktor * accelerationIntegral.x) / NUMBER_OF_MEASUREMENTS_FOR_BALANCING;
      accelerationIntegral.y = (EE_Parameter.GyroAccFaktor * accelerationIntegral.y) / NUMBER_OF_MEASUREMENTS_FOR_BALANCING;
      accelerationIntegral.z = accelerationIntegral.z / NUMBER_OF_MEASUREMENTS_FOR_BALANCING;
      //--Pitch------------------------
      integralError.pitch = (long)(meanAngleIntegral.pitch - (long)accelerationIntegral.x);
      balancing.pitch = integralError.pitch / EE_Parameter.GyroAccAbgleich;
      //--Roll-------------------------
      integralError.roll = (long)(meanAngleIntegral.roll - (long)accelerationIntegral.y);
      balancing.roll = integralError.roll / EE_Parameter.GyroAccAbgleich;

      poseCorrection.pitch = balancing.pitch / NUMBER_OF_MEASUREMENTS_FOR_BALANCING;
      poseCorrection.roll  = balancing.roll / NUMBER_OF_MEASUREMENTS_FOR_BALANCING;

      if(((maxStickAngles.pitch > 64) || (maxStickAngles.roll > 64) || (abs(PPM_in[EE_Parameter.Kanalbelegung[CH_YAW]]) > 25)) && (FromNaviCtrl_Value.Kalman_K == -1)){
        poseCorrection.pitch /= 2;
        poseCorrection.roll  /= 2;
      }

      //--Calc gyro drift--------------
      meanAngleIntegral2.pitch /= NUMBER_OF_MEASUREMENTS_FOR_BALANCING;
      meanAngleIntegral2.roll /= NUMBER_OF_MEASUREMENTS_FOR_BALANCING;
      tmp_long  = angleIntegral2.pitch - angleIntegral.pitch;
      tmp_long2 = angleIntegral2.roll - angleIntegral.roll;

      integralError.pitch = tmp_long;
      integralError.roll = tmp_long2;
      measurementAngleIntegral2.pitch -= integralError.pitch;
      measurementAngleIntegral2.roll -= integralError.roll;

      if(EE_Parameter.Driftkomp){
        if(yawGyroError > NUMBER_OF_MEASUREMENTS_FOR_BALANCING/2){ 
          A2DNeutral.yaw++;
          AdNeutralGierBias++; 
        }
        if(yawGyroError <-NUMBER_OF_MEASUREMENTS_FOR_BALANCING/2){ 
          A2DNeutral.yaw--; 
          AdNeutralGierBias--; 
        }
      }
      yawGyroError = 0;


      const int ERROR_LIMIT = NUMBER_OF_MEASUREMENTS_FOR_BALANCING/2;
      const int ERROR_LIMIT1 = NUMBER_OF_MEASUREMENTS_FOR_BALANCING * 2;
      const int ERROR_LIMIT2 = NUMBER_OF_MEASUREMENTS_FOR_BALANCING * 16;
      const int MOVEMENT_LIMIT = 20000;
      //--Pitch------------------------
      cnt = 1;
      if(labs(integralError.pitch) > ERROR_LIMIT1){
        cnt = 4;
      }

      if(labs(meanAngleIntegralOld.pitch - meanAngleIntegral.pitch) < MOVEMENT_LIMIT || (FromNaviCtrl_Value.Kalman_MaxDrift > 3*8)){
        if(integralError.pitch >  ERROR_LIMIT2){
          if(last_n_p){
            cnt += labs(integralError.pitch) / (ERROR_LIMIT2 / 8);
            balancing.pitch = integralError.pitch / 8;
            LIMIT_MAX(balancing.pitch,5000);
            poseCorrection.pitch += balancing.pitch / NUMBER_OF_MEASUREMENTS_FOR_BALANCING;
          }
          else{
            last_n_p = 1;
          }
        }
        else{
          last_n_p = 0;
        }
        if(integralError.pitch < -ERROR_LIMIT2){
          if(last_n_n){
            cnt += labs(integralError.pitch) / (ERROR_LIMIT2 / 8);
            balancing.pitch = integralError.pitch / 8;
            LIMIT_MIN(balancing.pitch, -5000);
            poseCorrection.pitch += balancing.pitch / NUMBER_OF_MEASUREMENTS_FOR_BALANCING;
          }
          else{
            last_n_n = 1;
          }
        }
        else{
          last_n_n = 0;
        }
      }
      else{
        cnt = 0;
        KompassSignalSchlecht = 1000;
      }

      LIMIT_MAX(cnt,EE_Parameter.Driftkomp);
      if(FromNaviCtrl_Value.Kalman_MaxDrift){
        LIMIT_MAX(cnt,FromNaviCtrl_Value.Kalman_MaxDrift);
      }

      if(integralError.pitch >  ERROR_LIMIT){
        A2DNeutral.pitch += cnt;
      }

      if(integralError.pitch < -ERROR_LIMIT){
        A2DNeutral.pitch -= cnt;
      }

      //--Roll-------------------------
      cnt = 1;// + labs(integralError.pitch) / 4096;
      if(labs(integralError.roll) > ERROR_LIMIT1){
        cnt = 4;
      }
      balancing.roll = 0;
      if(labs(meanAngleIntegralOld.roll - meanAngleIntegral.roll) < MOVEMENT_LIMIT || (FromNaviCtrl_Value.Kalman_MaxDrift > 3*8)){
        if(integralError.roll >  ERROR_LIMIT2){
          if(last_r_p){
            cnt += labs(integralError.roll) / (ERROR_LIMIT2 / 8);
            balancing.roll = integralError.roll / 8;
            LIMIT_MAX(balancing.roll, 5000);
            poseCorrection.roll += balancing.roll / NUMBER_OF_MEASUREMENTS_FOR_BALANCING;
          }
          else{
            last_r_p = 1;
          }
        }
        else{
          last_r_p = 0;
        }

        if(integralError.roll < -ERROR_LIMIT2){
          if(last_r_n){
            cnt += labs(integralError.roll) / (ERROR_LIMIT2 / 8);
            balancing.roll = integralError.roll / 8;
            LIMIT_MIN(balancing.roll,-5000);
            poseCorrection.roll += balancing.roll / NUMBER_OF_MEASUREMENTS_FOR_BALANCING;
          }
          else{
            last_r_n = 1;
          }
        }
        else{
          last_r_n = 0;
        }
      }
      else{
        cnt = 0;
        KompassSignalSchlecht = 1000;
      }

      if(cnt > EE_Parameter.Driftkomp){
        cnt = EE_Parameter.Driftkomp;
      }

      if(FromNaviCtrl_Value.Kalman_MaxDrift){
        if(cnt > FromNaviCtrl_Value.Kalman_MaxDrift){
          cnt = FromNaviCtrl_Value.Kalman_MaxDrift;
        }
      }

      if(integralError.roll >  ERROR_LIMIT){
        A2DNeutral.roll += cnt;
      }
      
      if(integralError.roll < -ERROR_LIMIT){
        A2DNeutral.roll -= cnt;
      }
    }
    else{
      poseCorrection.roll  = 0;
      poseCorrection.pitch = 0;
      TrichterFlug = 0;
    }

    if(!IntegralFaktor){ 
      poseCorrection.roll  = 0; 
      poseCorrection.pitch = 0;
    } // z.B. bei HH

    //-------------------------------
    meanAngleIntegralOld.pitch = meanAngleIntegral.pitch;
    meanAngleIntegralOld.roll  = meanAngleIntegral.roll;
    //-------------------------------
    accelerationIntegral.x   = 0;
    accelerationIntegral.y   = 0;
    accelerationIntegral.z   = 0;
    meanAngleIntegral.pitch  = 0;
    meanAngleIntegral.roll   = 0;
    meanAngleIntegral2.pitch = 0;
    meanAngleIntegral2.roll  = 0;
    measurementCount         = 0;
  } //  measurementCount >= NUMBER_OF_MEASUREMENTS_FOR_BALANCING


  //--Yaw--------------------------
  if(abs(stick.yaw) > 15){// war 35
    KompassSignalSchlecht = 1000;
    if(!(EE_Parameter.GlobalConfig & CFG_KOMPASS_FIX)){
      NeueKompassRichtungMerken = 1;
    }
  }
  tmp_int  = (long) EE_Parameter.Gier_P * ((long)stick.yaw * abs(stick.yaw)) / 512L; // expo  y = ax + bx�
  tmp_int += (EE_Parameter.Gier_P * stick.yaw) / 4;
  desiredYaw = tmp_int;
  measurementAngleIntegral.yaw -= tmp_int;
  LIMIT_SYMMETRIC(measurementAngleIntegral.yaw,50000);

  //--Compass----------------------
  if(KompassValue && (EE_Parameter.GlobalConfig & CFG_KOMPASS_AKTIV)){
    int w,v,r,fehler,korrektur;
    w = abs(angleIntegral.pitch /512); // mit zunehmender Neigung den Einfluss drosseln
    v = abs(angleIntegral.roll /512);
    LIMIT_MAX(v,w);// gr�sste Neigung ermitteln

    korrektur = w / 8 + 1;
    fehler = ((540 + KompassValue - (ErsatzKompass/GIER_GRAD_FAKTOR)) % 360) - 180;
    if(abs(measurement.processed.yaw) > 128){
      fehler = 0;
    }

    if(!KompassSignalSchlecht && w < 25){
      yawGyroError += fehler;
      if(NeueKompassRichtungMerken){
        ErsatzKompass = KompassValue * GIER_GRAD_FAKTOR;
        KompassStartwert = (ErsatzKompass/GIER_GRAD_FAKTOR);
        NeueKompassRichtungMerken = 0;
      }
    }
    ErsatzKompass += (fehler * 8) / korrektur;
    w = (w * Parameter_KompassWirkung) / 32;           // auf die Wirkung normieren
    w = Parameter_KompassWirkung - w;                  // Wirkung ggf drosseln
    if(w >= 0){
      if(!KompassSignalSchlecht){
        v = 64 + ((maxStickAngles.pitch + maxStickAngles.roll)) / 8;
        r = ((540 + (ErsatzKompass/GIER_GRAD_FAKTOR) - KompassStartwert) % 360) - 180;

        v = (r * w) / v;  // nach Kompass ausrichten
        w = 3 * Parameter_KompassWirkung;
        LIMIT_SYMMETRIC(v,w);

        measurementAngleIntegral.yaw += v;
      }
      if(KompassSignalSchlecht){
        KompassSignalSchlecht--;
      }
    }
    else{
      KompassSignalSchlecht = 500; // so lange das Signal taub stellen --> ca. 1 sek
    }
  }
  //-------------------------------

  //--Assign debug values----------
  if(!timerDebugDataOut--){
    timerDebugDataOut = 24;//todo: why 24? ->~20.833Hz

    DebugOut.Analog[0] = angleIntegral.pitch / (EE_Parameter.GyroAccFaktor * 4);//"AngleNick       "
    DebugOut.Analog[1] = angleIntegral.roll / (EE_Parameter.GyroAccFaktor * 4);//"AngleRoll       "
    DebugOut.Analog[2] = meanValueAcc.x / 4;//"AccNick         "
    DebugOut.Analog[3] = meanValueAcc.y / 4;//"AccRoll         "
    DebugOut.Analog[4] = measurement.processed.yaw;//"YawGyro         "
    DebugOut.Analog[5] = currentHeight/5;//"Height Value    "
    DebugOut.Analog[6] = currentAcc.z;//"AccZ            "
    //DebugOut.Analog[7] = ?//"Gas             "
    DebugOut.Analog[8] = KompassValue;//"Compass Value   "
    DebugOut.Analog[9] = UBat;//"Voltage [0.1V]  "
    DebugOut.Analog[10] = SenderOkay;//"Receiver Level  "
    DebugOut.Analog[11] = ErsatzKompass / GIER_GRAD_FAKTOR;//"Gyro Compass    "
    /*elsewhere:
    DebugOut.Analog[12] = Motor[0].SetPoint;//"Motor 1         "
    DebugOut.Analog[13] = Motor[1].SetPoint;//"Motor 2         "
    DebugOut.Analog[14] = Motor[2].SetPoint;//"Motor 3         "
    DebugOut.Analog[15] = Motor[3].SetPoint;//"Motor 4         "
    DebugOut.Analog[16] = Motor[4].SetPoint;//"Motor 5         "
    DebugOut.Analog[17] = Motor[5].SetPoint;//"Motor 6         "
    */
    DebugOut.Analog[18] = VarioMeter;//"VarioMeter      "
    DebugOut.Analog[19] = WinkelOut.CalcState;//"MK3Mag CalState "
    DebugOut.Analog[20] = ServoNickValue;//"Servo           "
    //DebugOut.Analog[21] = ?//"Hovergas        "
    DebugOut.Analog[22] = Capacity.ActualCurrent;//"Current [0.1A]  "
    DebugOut.Analog[23] = Capacity.UsedCapacity;//"Capacity [mAh]  "
    //DebugOut.Analog[24] = //"mdebug1         "
    //DebugOut.Analog[25] = //"mdebug2         "
    //DebugOut.Analog[26] = //"mdebug3         "
    //DebugOut.Analog[27] = //"mdebug4         "
    //DebugOut.Analog[28] = ?//"I2C-Error       "
    DebugOut.Analog[29] = FromNaviCtrl_Value.SerialDataOkay;//"                "
    DebugOut.Analog[30] = GPS_Nick;//"GPS_Nick        "
    DebugOut.Analog[31] = GPS_Roll;//"GPS_Roll        "
  }

  //--Calc angular velocity & angles current value
  if(TrichterFlug){ 
    sum.roll  = 0; 
    sum.pitch = 0;
  }

  integralMultiplicationFactor.pitch = (angleIntegral.pitch * IntegralFaktor) /  (44000 / STICK_GAIN); 
  integralMultiplicationFactor.roll  = (angleIntegral.roll  * IntegralFaktor) /  (44000 / STICK_GAIN); 

  const int TRIM_MAX = 200;
  LIMIT_SYMMETRIC(trim.pitch,TRIM_MAX);
  LIMIT_SYMMETRIC(trim.roll, TRIM_MAX);

  measurement.processed.pitch = integralMultiplicationFactor.pitch + \
                                (long)((long)measurement.processed.pitch * gyroFactorPR + (long)trim.pitch * 128L) / (256L / STICK_GAIN);
  measurement.processed.roll  = integralMultiplicationFactor.roll + \
                                (long)((long)measurement.processed.roll  * gyroFactorPR + (long)trim.roll  * 128L) / (256L / STICK_GAIN);
  measurement.processed.yaw = (long)(measurement.processed.yaw * 2 * (long)gyroFactorYaw) / (256L / STICK_GAIN) + \
                              (long)(angleIntegral.yaw * IntegralFaktorGier) / (2 * (44000 / STICK_GAIN));

  // Maximalwerte abfangen
  const int MAX_SENSOR = 4096*4;
  LIMIT_SYMMETRIC(measurement.processed.pitch,MAX_SENSOR);
  LIMIT_SYMMETRIC(measurement.processed.roll, MAX_SENSOR);
  LIMIT_SYMMETRIC(measurement.processed.yaw,  MAX_SENSOR);

  //--Height control---------------
  // Height control can only lower gas value, won't increase it.
  if(UBat > BattLowVoltageWarning){
    mixerValues.gas = ((unsigned int)mixerValues.gas * BattLowVoltageWarning) / UBat; // Gas auf das aktuelle Spannungvieveau beziehen
  }
  mixerValues.gas *= STICK_GAIN;

  // limit gas to parameter setting
  LIMIT_MIN_MAX(mixerValues.gas, (minimumGasValue+10)*STICK_GAIN,(maximumGasValue-20)*STICK_GAIN);

  //--All BL-Ctrl connected?-------
  if(MissingMotor){
    if(vehicleFlyingCounter > 1 && vehicleFlyingCounter < 50 && mixerValues.gas > 0){
      vehicleFlyingCounter = 1;
      mixerValues.gas = minimumGasValue;
    }
  }
  //--Mixer & PI controller--------
  DebugOut.Analog[7] = mixerValues.gas;
  //--Yaw component----------------
  mixerValues.yaw = measurement.processed.yaw - desiredYaw*STICK_GAIN;// Regler f�r Gier

  //If GasMishcanteil is below value this value, we pretend it's higher so that we can still yaw
  const int MIN_GIERGAS = 40*STICK_GAIN;
  LIMIT_SYMMETRIC(mixerValues.yaw, MAX(mixerValues.gas, MIN_GIERGAS) /2);

  LIMIT_SYMMETRIC(mixerValues.yaw,maximumGasValue*STICK_GAIN - mixerValues.gas);

  //--Pitch axis-------------------
  angleDifference.pitch = measurement.processed.pitch - stick.pitch;    // Differenz bestimmen
  if(IntegralFaktor){
    sum.pitch += integralMultiplicationFactor.pitch - stick.pitch; // I-Anteil bei Winkelregelung
  }
  else{
    sum.pitch += angleDifference.pitch; // I-Anteil bei HH
  }

  LIMIT_SYMMETRIC(sum.pitch,STICK_GAIN*16000L);

  mixerValues.pitch = angleDifference.pitch + sum.pitch / Ki; // PI-Regler f�r Nick
  // Motor Vorn
  tmp_int = (long)((long)Parameter_DynamicStability * (long)(mixerValues.gas + abs(mixerValues.yaw)/2)) / 64;
  LIMIT_SYMMETRIC(mixerValues.pitch,tmp_int);

  //--Roll axis--------------------
  angleDifference.roll = measurement.processed.roll - stick.roll;    // Differenz bestimmen
  if(IntegralFaktor){
    sum.roll += integralMultiplicationFactor.roll - stick.roll;// I-Anteil bei Winkelregelung
  }
  else{
    sum.roll += angleDifference.roll;  // I-Anteil bei HH
  }

  LIMIT_SYMMETRIC(sum.roll,STICK_GAIN*16000L);

  mixerValues.roll = angleDifference.roll + sum.roll / Ki;    // PI-Regler f�r Roll
  tmp_int = (long)((long)Parameter_DynamicStability * (long)(mixerValues.gas + abs(mixerValues.yaw)/2)) / 64;
  LIMIT_SYMMETRIC(mixerValues.roll,tmp_int); 

  //--Universal mixer--------------
  static signed int oldMotorCommand[MAX_MOTORS]; //the previous timestep's motor commands
  for(i=0; i<MAX_MOTORS; i++){
    if(Mixer.Motor[i][MIXER_GAS]<=0){//we're not considering this motor
      Motor[i].SetPoint = 0;
      continue;
    }

    signed int newMotorCommand;
    //                                          Mixer.Motor[i][j] \in {-64,0,+64}
    newMotorCommand =  ((long)mixerValues.gas   * Mixer.Motor[i][MIXER_GAS])   / 64L;
    newMotorCommand += ((long)mixerValues.pitch * Mixer.Motor[i][MIXER_PITCH]) / 64L;
    newMotorCommand += ((long)mixerValues.roll  * Mixer.Motor[i][MIXER_ROLL])  / 64L;
    newMotorCommand += ((long)mixerValues.yaw   * Mixer.Motor[i][MIXER_YAW])   / 64L;
    oldMotorCommand[i] = motorSmoothing(newMotorCommand,oldMotorCommand[i]);  // Filter (not really smoothing ... )

    newMotorCommand = oldMotorCommand[i]/STICK_GAIN; //STICK_GAIN = 4
    LIMIT_MIN_MAX(newMotorCommand, minimumGasValue, maximumGasValue);

    switch(enableHILS){
      case HILS_REAL_FLIGHT://we just output the commands as they are
        Motor[i].SetPoint = newMotorCommand;
        break;
      case HILS_PROPER_HILS://we set the motors to zero, output via serial
        Motor[i].SetPoint = 0;
        if(i<NUMBER_OF_MOTORS){//to prevent writing into structures which don't exist...
          motorCommandsHILS.commands.setPoint[i]=newMotorCommand;
        }
        break;
      case HILS_PROPER_HILS_WITH_MOTORS_ON://we output the motor commands via serial, and allow the motors to spin
        if(i<NUMBER_OF_MOTORS){//to prevent writing into structures which don't exist...
          motorCommandsHILS.commands.setPoint[i]=newMotorCommand;
        }
        Motor[i].SetPoint = newMotorCommand;
        break;
      case HILS_EQUAL_MOTOR_COMMANDS://give all motors the mean desired gas value
        //motorCommandsHILS.commands.setPoint[i] = mixerValues.gas;
        motorCommandsHILS.commands.setPoint[i] = (unsigned char) stick.gas;//this is NOT in any way close to reality
        LIMIT_MIN_MAX(motorCommandsHILS.commands.setPoint[i],minimumGasValue,maximumGasValue);
        //TODO!
        break;
      default://this shouldn't happen!
        Motor[i].SetPoint = 0;
    }
  }
  motorCommandsHILS.commands.sensorDataID = simulatedSensorData.sensors.id;//the id-stamp of the corresponding sensor data
  motorCommandsHILS.haveDataToSend =  1;
}
