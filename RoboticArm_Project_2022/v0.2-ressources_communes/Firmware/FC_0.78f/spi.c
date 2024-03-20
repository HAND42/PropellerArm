// ######################## SPI - FlightCtrl ###################
#include "main.h"

struct str_ToNaviCtrl   ToNaviCtrl;
struct str_FromNaviCtrl   FromNaviCtrl;
struct str_FromNaviCtrl_Value FromNaviCtrl_Value;
struct str_SPI_VersionInfo NC_Version;
struct str_GPSInfo GPSInfo;

unsigned char SPI_BufferIndex;
unsigned char SPI_RxBufferIndex;

volatile unsigned char SPI_Buffer[sizeof(FromNaviCtrl)];
unsigned char *SPI_TX_Buffer;

unsigned char SPITransferCompleted, SPI_ChkSum;
unsigned char SPI_RxDataValid,NaviDataOkay = 0;

unsigned char SPI_CommandSequence[] = {SPI_FCCMD_STICK, \
                                       SPI_FCCMD_USER, \
                                       SPI_FCCMD_PARAMETER1, \
                                       SPI_FCCMD_STICK, \
                                       SPI_FCCMD_MISC, \
                                       SPI_FCCMD_VERSION, \
                                       SPI_FCCMD_STICK, \
                                       SPI_FCCMD_SERVOS, \
                                       SPI_FCCMD_ACCU};
unsigned char SPI_CommandCounter = 0;

//------------------------------------------------------
void SPI_MasterInit(void){
  DDR_SPI |= (1<<DD_MOSI)|(1<<DD_SCK);    // Set MOSI and SCK output, all others input
  SLAVE_SELECT_DDR_PORT |= (1 << SPI_SLAVE_SELECT);

  SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR1)|(0<<SPR0)|(0<<SPIE);   // Enable SPI, Master, set clock rate fck/64
  SPSR = 0;//(1<<SPI2X);

  SLAVE_SELECT_PORT |=  (1 << SPI_SLAVE_SELECT);
  SPITransferCompleted = 1;

  ToNaviCtrl.Sync1 = 0xAA;
  ToNaviCtrl.Sync2 = 0x83;

  ToNaviCtrl.Command = SPI_FCCMD_USER;
  ToNaviCtrl.IntegralNick = 0;
  ToNaviCtrl.IntegralRoll = 0;
  FromNaviCtrl_Value.SerialDataOkay = 0;
  SPI_RxDataValid = 0;

}

//------------------------------------------------------
void SPI_StartTransmitPacket(void){
   if (!SPITransferCompleted){ 
     return;
   }

   SLAVE_SELECT_PORT &=  ~(1 << SPI_SLAVE_SELECT);  // SelectSlave
   SPI_TX_Buffer = (unsigned char *) &ToNaviCtrl;

   ToNaviCtrl.Command = SPI_CommandSequence[SPI_CommandCounter++];
   if (SPI_CommandCounter >= sizeof(SPI_CommandSequence)){ 
     SPI_CommandCounter = 0;
   }

   SPITransferCompleted = 0;
   UpdateSPI_Buffer();                              // update buffer

   SPI_BufferIndex = 1;
   asm volatile ("nop"); asm volatile ("nop"); asm volatile ("nop"); asm volatile ("nop");     
   asm volatile ("nop"); asm volatile ("nop"); asm volatile ("nop"); asm volatile ("nop");
   asm volatile ("nop"); asm volatile ("nop"); asm volatile ("nop"); asm volatile ("nop");     
   asm volatile ("nop"); asm volatile ("nop"); asm volatile ("nop"); asm volatile ("nop");
   asm volatile ("nop"); asm volatile ("nop"); asm volatile ("nop"); asm volatile ("nop");     
   asm volatile ("nop"); asm volatile ("nop"); asm volatile ("nop"); asm volatile ("nop");
   ToNaviCtrl.Chksum = ToNaviCtrl.Sync1;
   SPDR = ToNaviCtrl.Sync1;                  // Start transmission
}

//------------------------------------------------------
//SIGNAL(SIG_SPI)
void SPI_TransmitByte(void){
  static unsigned char SPI_RXState = 0;
  unsigned char rxdata;
  static unsigned char rxchksum;

  if (SPITransferCompleted){ 
    return;
  }

  if (!(SPSR & (1 << SPIF))){ 
    return;
  }

  SendSPI = 4;

  SLAVE_SELECT_PORT |=  (1 << SPI_SLAVE_SELECT);   // DeselectSlave

  rxdata = SPDR;
  switch ( SPI_RXState)
  {
  case 0:

    SPI_RxBufferIndex = 0;
    rxchksum = rxdata;
    if (rxdata == 0x81 ){ // 1. Syncbyte ok
      SPI_RXState  = 1;  
    }

    break;

  case 1:
    if (rxdata == 0x55){ // 2. Syncbyte ok
      rxchksum += rxdata; SPI_RXState  = 2;  
    }   
    else SPI_RXState  = 0;
    break;

  case 2:
    SPI_Buffer[SPI_RxBufferIndex++]= rxdata;             // get data
    if (SPI_RxBufferIndex >= sizeof(FromNaviCtrl)){
      if (rxdata == rxchksum){
        unsigned char *ptr = (unsigned char *)&FromNaviCtrl;

        memcpy(ptr, (unsigned char *) SPI_Buffer,  sizeof(SPI_Buffer));

        SPI_RxDataValid = 1;
      }
      else{
        SPI_RxDataValid = 0;
      }

      SPI_RXState  = 0;
    }
    else{
      rxchksum += rxdata;
    }
    break;

  }

  if (SPI_BufferIndex < sizeof(ToNaviCtrl)){
    SLAVE_SELECT_PORT &=  ~(1 << SPI_SLAVE_SELECT);  // SelectSlave
    asm volatile ("nop"); asm volatile ("nop"); asm volatile ("nop"); asm volatile ("nop");     
    asm volatile ("nop"); asm volatile ("nop"); asm volatile ("nop"); asm volatile ("nop");
    asm volatile ("nop"); asm volatile ("nop"); asm volatile ("nop"); asm volatile ("nop");     
    asm volatile ("nop"); asm volatile ("nop"); asm volatile ("nop"); asm volatile ("nop");
    asm volatile ("nop"); asm volatile ("nop"); asm volatile ("nop"); asm volatile ("nop");     
    asm volatile ("nop"); asm volatile ("nop"); asm volatile ("nop"); asm volatile ("nop");

    SPDR = SPI_TX_Buffer[SPI_BufferIndex];
    ToNaviCtrl.Chksum += SPI_TX_Buffer[SPI_BufferIndex];
  }
  else{
    SPITransferCompleted = 1;
  }

  SPI_BufferIndex++;
}


//------------------------------------------------------
void UpdateSPI_Buffer(void){
  signed int tmp;
  ToNaviCtrl.IntegralNick = (int) (angleIntegral.pitch / (long)(EE_Parameter.GyroAccFaktor * 4));
  ToNaviCtrl.IntegralRoll = (int) (angleIntegral.roll  / (long)(EE_Parameter.GyroAccFaktor * 4));
  ToNaviCtrl.GyroCompass = (10 * ErsatzKompass) / GIER_GRAD_FAKTOR;
  ToNaviCtrl.AccNick = ((int) ACC_AMPLIFY * (naviAcc.pitch / NaviCntAcc))/4;
  ToNaviCtrl.AccRoll = ((int) ACC_AMPLIFY * (naviAcc.roll  / NaviCntAcc))/4;
  NaviCntAcc = 0; naviAcc.pitch = 0; naviAcc.roll = 0;

  switch(ToNaviCtrl.Command)  //
  {
  case SPI_FCCMD_USER:
    ToNaviCtrl.Param.Byte[0] = Parameter_UserParam1;
    ToNaviCtrl.Param.Byte[1] = Parameter_UserParam2;
    ToNaviCtrl.Param.Byte[2] = Parameter_UserParam3;
    ToNaviCtrl.Param.Byte[3] = Parameter_UserParam4;
    ToNaviCtrl.Param.Byte[4] = Parameter_UserParam5;
    ToNaviCtrl.Param.Byte[5] = Parameter_UserParam6;
    ToNaviCtrl.Param.Byte[6] = Parameter_UserParam7;
    ToNaviCtrl.Param.Byte[7] = Parameter_UserParam8;
    ToNaviCtrl.Param.Byte[8] = (unsigned char) FCFlags;
    FCFlags &= ~(FCFLAG_CALIBRATE | FCFLAG_START);
    ToNaviCtrl.Param.Byte[9] =(unsigned char) eeprom_read_byte(&EEPromArray[EEPROM_ADR_ACTIVE_SET]);
    break;

  case SPI_FCCMD_ACCU:
    ToNaviCtrl.Param.Int[0] = Capacity.ActualCurrent; // 0.1A
    ToNaviCtrl.Param.Int[1] = Capacity.UsedCapacity; // mAh
    ToNaviCtrl.Param.Byte[4] = (unsigned char) UBat; // 0.1V
    ToNaviCtrl.Param.Byte[5] = (unsigned char) BattLowVoltageWarning; //0.1V
    break;

  case SPI_FCCMD_PARAMETER1:
    ToNaviCtrl.Param.Byte[0] = EE_Parameter.NaviGpsModeControl;     // Parameters for the Naviboard
    ToNaviCtrl.Param.Byte[1] = EE_Parameter.NaviGpsGain;
    ToNaviCtrl.Param.Byte[2] = EE_Parameter.NaviGpsP;
    ToNaviCtrl.Param.Byte[3] = EE_Parameter.NaviGpsI;
    ToNaviCtrl.Param.Byte[4] = EE_Parameter.NaviGpsD;
    ToNaviCtrl.Param.Byte[5] = EE_Parameter.NaviGpsACC;
    ToNaviCtrl.Param.Byte[6] = EE_Parameter.NaviGpsMinSat;
    ToNaviCtrl.Param.Byte[7] = EE_Parameter.NaviStickThreshold;
    ToNaviCtrl.Param.Byte[8] = EE_Parameter.NaviOperatingRadius;
    ToNaviCtrl.Param.Byte[9] = EE_Parameter.NaviWindCorrection;
    ToNaviCtrl.Param.Byte[10] = EE_Parameter.NaviSpeedCompensation;
    ToNaviCtrl.Param.Byte[11] = EE_Parameter.NaviAngleLimitation;
    break;

  case SPI_FCCMD_STICK:
    cli();
    tmp = PPM_in[EE_Parameter.Kanalbelegung[CH_GAS]];  if(tmp > 127) tmp = 127; else if(tmp < -127) tmp = -127;
    ToNaviCtrl.Param.Byte[0] = (char) tmp;
    tmp = PPM_in[EE_Parameter.Kanalbelegung[CH_YAW]]; if(tmp > 127) tmp = 127; else if(tmp < -127) tmp = -127;
    ToNaviCtrl.Param.Byte[1] = (char) tmp;
    tmp = PPM_in[EE_Parameter.Kanalbelegung[CH_ROLL]]; if(tmp > 127) tmp = 127; else if(tmp < -127) tmp = -127;
    ToNaviCtrl.Param.Byte[2] = (char) tmp;
    tmp = PPM_in[EE_Parameter.Kanalbelegung[CH_PITCH]]; if(tmp > 127) tmp = 127; else if(tmp < -127) tmp = -127;
    sei();
    ToNaviCtrl.Param.Byte[3] = (char) tmp;
    ToNaviCtrl.Param.Byte[4] = (unsigned char) Poti[0];
    ToNaviCtrl.Param.Byte[5] = (unsigned char) Poti[1];
    ToNaviCtrl.Param.Byte[6] = (unsigned char) Poti[2];
    ToNaviCtrl.Param.Byte[7] = (unsigned char) Poti[3];
    ToNaviCtrl.Param.Byte[8] = (unsigned char) Poti[4];
    ToNaviCtrl.Param.Byte[9] = (unsigned char) Poti[5];
    ToNaviCtrl.Param.Byte[10] = (unsigned char) Poti[6];
    ToNaviCtrl.Param.Byte[11] = (unsigned char) Poti[7];
    break;
  case SPI_FCCMD_MISC:
    if(WinkelOut.CalcState > 5){
      WinkelOut.CalcState = 0;
      ToNaviCtrl.Param.Byte[0] = 5;
    }
    else{
      ToNaviCtrl.Param.Byte[0] = WinkelOut.CalcState;
    }

    ToNaviCtrl.Param.Byte[1] = EE_Parameter.NaviPH_LoginTime;
    ToNaviCtrl.Param.Int[1] = DebugOut.Analog[5];// = HoehenWert/5;
    ToNaviCtrl.Param.Int[2] = (int)(desiredHeight/5);
    ToNaviCtrl.Param.Byte[6] = EE_Parameter.NaviGpsPLimit;
    ToNaviCtrl.Param.Byte[7] = EE_Parameter.NaviGpsILimit;
    ToNaviCtrl.Param.Byte[8] = EE_Parameter.NaviGpsDLimit;
    ToNaviCtrl.Param.Byte[9] = (unsigned char) SenderOkay;
    ToNaviCtrl.Param.Byte[10] = (unsigned char) SenderRSSI;
    ToNaviCtrl.Param.Byte[11] = DebugOut.Analog[7] / 4; //GasMischanteil
    break;
  case SPI_FCCMD_VERSION:
    ToNaviCtrl.Param.Byte[0] = VERSION_MAJOR;
    ToNaviCtrl.Param.Byte[1] = VERSION_MINOR;
    ToNaviCtrl.Param.Byte[2] = VERSION_PATCH;
    ToNaviCtrl.Param.Byte[3] = NC_SPI_COMPATIBLE;
    ToNaviCtrl.Param.Byte[4] = 20; //mwm PlatinenVersion hard-coded as 20
    break;

  case SPI_FCCMD_SERVOS:
    ToNaviCtrl.Param.Byte[0] = EE_Parameter.ServoNickRefresh;     // Parameters for the Servo Control
    ToNaviCtrl.Param.Byte[1] = EE_Parameter.ServoCompInvert;
    ToNaviCtrl.Param.Byte[2] = Parameter_ServoNickControl;
    ToNaviCtrl.Param.Byte[3] = EE_Parameter.ServoNickComp;
    ToNaviCtrl.Param.Byte[4] = EE_Parameter.ServoNickMin;
    ToNaviCtrl.Param.Byte[5] = EE_Parameter.ServoNickMax;
    ToNaviCtrl.Param.Byte[6] = Parameter_ServoRollControl;
    ToNaviCtrl.Param.Byte[7] = EE_Parameter.ServoRollComp;
    ToNaviCtrl.Param.Byte[8] = EE_Parameter.ServoRollMin;
    ToNaviCtrl.Param.Byte[9] = EE_Parameter.ServoRollMax;
    break;
  }

  if(SPI_RxDataValid){
    NaviDataOkay = 250;
    if(abs(FromNaviCtrl.GPS_Nick) < 512 && abs(FromNaviCtrl.GPS_Roll) < 512 && (EE_Parameter.GlobalConfig & CFG_GPS_AKTIV)){
      GPS_Nick = FromNaviCtrl.GPS_Nick;
      GPS_Roll = FromNaviCtrl.GPS_Roll;
    }

    if(FromNaviCtrl.CompassValue <= 360){
      KompassValue = FromNaviCtrl.CompassValue;
    }
    KompassRichtung = ((540 + KompassValue - KompassStartwert) % 360) - 180;

    if(FromNaviCtrl.BeepTime > beeptime && !WinkelOut.CalcState){
      beeptime = FromNaviCtrl.BeepTime;
    }

    switch (FromNaviCtrl.Command){
    case SPI_NCCMD_KALMAN:
      FromNaviCtrl_Value.Kalman_K = FromNaviCtrl.Param.sByte[0];
      FromNaviCtrl_Value.Kalman_MaxFusion = FromNaviCtrl.Param.sByte[1];
      FromNaviCtrl_Value.Kalman_MaxDrift = FromNaviCtrl.Param.sByte[2];
      FromNaviCtrl_Value.SerialDataOkay = FromNaviCtrl.Param.Byte[3];
      FromNaviCtrl_Value.GpsZ = FromNaviCtrl.Param.Byte[4];
      break;

    case SPI_NCCMD_VERSION:
      NC_Version.Major = FromNaviCtrl.Param.Byte[0];
      NC_Version.Minor = FromNaviCtrl.Param.Byte[1];
      NC_Version.Patch = FromNaviCtrl.Param.Byte[2];
      NC_Version.Compatible = FromNaviCtrl.Param.Byte[3];
      NC_Version.Hardware = FromNaviCtrl.Param.Byte[4];
      break;

    case SPI_NCCMD_GPSINFO:
      GPSInfo.Flags = FromNaviCtrl.Param.Byte[0];
      GPSInfo.NumOfSats = FromNaviCtrl.Param.Byte[1];
      GPSInfo.SatFix = FromNaviCtrl.Param.Byte[2];
      GPSInfo.HomeDistance = FromNaviCtrl.Param.Int[2];
      GPSInfo.HomeBearing = FromNaviCtrl.Param.sInt[3];
      break;

    default:
      break;
    }
  }
  else{
    GPS_Nick = 0;
    GPS_Roll = 0;
  }
}
