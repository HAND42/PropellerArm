/*LICENSE: 
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + Copyright (c) Holger Buss, Ingo Busker
// + Nur für den privaten Gebrauch / NON-COMMERCIAL USE ONLY
// + www.MikroKopter.com
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + Es gilt für das gesamte Projekt (Hardware, Software, Binärfiles, Sourcecode und Dokumentation),
// + dass eine Nutzung (auch auszugsweise) nur für den privaten und nicht-kommerziellen Gebrauch zulässig ist.
// + Sollten direkte oder indirekte kommerzielle Absichten verfolgt werden, ist mit uns (info@mikrokopter.de) Kontakt
// + bzgl. der Nutzungsbedingungen aufzunehmen.
// + Eine kommerzielle Nutzung ist z.B.Verkauf von MikroKoptern, Bestückung und Verkauf von Platinen oder Bausätzen,
// + Verkauf von Luftbildaufnahmen, usw.
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + Werden Teile des Quellcodes (mit oder ohne Modifikation) weiterverwendet oder veröffentlicht,
// + unterliegen sie auch diesen Nutzungsbedingungen und diese Nutzungsbedingungen incl. Copyright müssen dann beiliegen
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + Sollte die Software (auch auszugesweise) oder sonstige Informationen des MikroKopter-Projekts
// + auf anderen Webseiten oder Medien veröffentlicht werden, muss unsere Webseite "http://www.mikrokopter.de"
// + eindeutig als Ursprung verlinkt und genannt werden
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + Keine Gewähr auf Fehlerfreiheit, Vollständigkeit oder Funktion
// + Benutzung auf eigene Gefahr
// + Wir übernehmen keinerlei Haftung für direkte oder indirekte Personen- oder Sachschäden
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + Die Portierung oder Nutzung der Software (oder Teile davon) auf andere Systeme (ausser der Hardware von www.mikrokopter.de) ist nur
// + mit unserer Zustimmung zulässig
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
// :license*/
#include "main.h"

unsigned char EEPromArray[E2END+1] EEMEM; //EEMEM -> #define...
unsigned char PlatinenVersion = 20;//mwm: for I3S hexa, this is true //hardcoded everywhere. The variable is necessary as it is called apparently by libfc
unsigned char SendVersionToNavi = 1;
unsigned char BattLowVoltageWarning = 94;
unsigned int FlugMinuten = 0, FlugMinutenGesamt = 0, FlugSekunden = 0;
pVoidFnct_pVoidFnctChar_const_fmt _printf_P;

char markDebugString[MARK_DEBUG_STRING_LENGTH] = "";

// -- Parametersatz aus EEPROM lesen ---
// number [1..5]
void ReadParameterSet(unsigned char number, unsigned char *buffer, unsigned char length){
   if((number > 5)||(number < 1)) 
     number = 3;
   eeprom_read_block(buffer, &EEPromArray[EEPROM_ADR_PARAM_BEGIN + length * (number - 1)], length);
   LED_Init();
}

// -- Parametersatz ins EEPROM schreiben ---
// number [1..5]
void WriteParameterSet(unsigned char number, unsigned char *buffer, unsigned char length) {
  if(number > 5) 
    number = 5;
  if(number < 1) 
    return;
  eeprom_write_block(buffer, &EEPromArray[EEPROM_ADR_PARAM_BEGIN + length * (number - 1)], length);
  eeprom_write_byte(&EEPromArray[EEPROM_ADR_PARAM_LENGTH], length); // Länge der Datensätze merken
  eeprom_write_block(buffer, &EEPromArray[EEPROM_ADR_CHANNELS], 12); // 12 Kanäle merken
  SetActiveParamSetNumber(number);
  LED_Init();
}

unsigned char GetActiveParamSetNumber(void){
  unsigned char set;
  set = eeprom_read_byte(&EEPromArray[EEPROM_ADR_ACTIVE_SET]);
  if((set > 5) || (set < 1)){
    set = 3;
    SetActiveParamSetNumber(set);               // diesen Parametersatz als aktuell merken
  }
  return(set);
}

void SetActiveParamSetNumber(unsigned char number){
  if(number > 5) 
    number = 5;
  if(number < 1) 
    return;
  eeprom_write_byte(&EEPromArray[EEPROM_ADR_ACTIVE_SET], number);// diesen Parametersatz als aktuell merken
}

void CalMk3Mag(void){
  static unsigned char stick = 1;

  if(PPM_in[EE_Parameter.Kanalbelegung[CH_PITCH]] > -20)
    stick = 0;
  if((PPM_in[EE_Parameter.Kanalbelegung[CH_PITCH]] < -70) && !stick){
    stick = 1;
    WinkelOut.CalcState++;
    if(WinkelOut.CalcState > 4){
      beeptime = 1000;
    }
    else{
      Piep(WinkelOut.CalcState,150);
    }
  }
  DebugOut.Analog[19] = WinkelOut.CalcState;
}

void LipoDetection(unsigned char print){
  unsigned int timer;
  if(print) 
    printf("\n\rBatt:");
  if(EE_Parameter.UnterspannungsWarnung < 50){// automatische Zellenerkennung
    timer = SetDelay(500);
    if(print){
      while (!CheckDelay(timer)){
        //do nothing...
      }
    }
    if(UBat < 130){
      BattLowVoltageWarning = 3 * EE_Parameter.UnterspannungsWarnung;
      if(print){
        Piep(3,200);
        printf(" 3 Cells  ");
      }
    }
    else{
      BattLowVoltageWarning = 4 * EE_Parameter.UnterspannungsWarnung;
      if(print){
        Piep(4,200);
        printf(" 4 Cells  ");
      }
    }
  }
  else{
    BattLowVoltageWarning = EE_Parameter.UnterspannungsWarnung;
  }
  if(print) 
    printf(" Low warning level: %d.%d",BattLowVoltageWarning/10,BattLowVoltageWarning%10);
}

void diagnosticBeep(unsigned char numBeeps, unsigned int duration){
  while(numBeeps--){
    beeptime = duration;
    while(beeptime){
      //do nothing...
    }
    Delay_ms(duration * 2);
  }

}

int main (void){
  unsigned int timer,i,timer2 = 0, timerPolling;
  unsigned char RequiredMotors = 0;

  DDRB  = 0x00;
  PORTB = 0x00;
  for(timer = 0; timer < 1000; timer++){
    //do nothing..
  }

  DDRC  = 0x81; // SCL
  DDRC  |=0x40; // HEF4017 Reset
  PORTC = 0xff; // Pullup SDA
  DDRB  = 0x1B; // LEDs und Druckoffset
  PORTB = 0x01; // LED_Rot
  DDRD  = 0x3E; // Speaker & TXD & J3 J4 J5
  PORTD = 0x47; // LED
  HEF4017R_ON;
  MCUSR &=~(1<<WDRF);
  WDTCSR |= (1<<WDCE)|(1<<WDE);
  WDTCSR = 0;

  beeptime = 2000;
  PPM_in[CH_PITCH] = 0;

  ROT_OFF;

  Timer_Init();
  TIMER2_Init();
  UART_Init();
  rc_sum_init();
  ADC_Init();
  i2c_init();
  SPI_MasterInit();
  Capacity_Init();
  LIBFC_Init();
  GRN_ON;
  sei();

  ReadParameterSet(3, (unsigned char *) &EE_Parameter.Kanalbelegung[0], 13); // read only the first bytes

  if((eeprom_read_byte(&EEPromArray[EEPROM_ADR_MIXER_TABLE]) == MIXER_REVISION) && // Check Revision in the first Byte
     (eeprom_read_byte(&EEPromArray[EEPROM_ADR_VALID]) != 0xff))                   // Settings reset via Koptertool
  {
    eeprom_read_block(&Mixer, &EEPromArray[EEPROM_ADR_MIXER_TABLE], sizeof(Mixer));
    for(unsigned char i=0; i<16;i++){ 
      if(Mixer.Motor[i][MIXER_GAS] > 0){
        RequiredMotors++;
      }
    }
  }
  else{// default
    //mwm: replaced default, so that we have the haxa table:
    printf("\n\rGenerating default Mixer Table");
    for(unsigned char i=0; i<16;i++){//set all motors to disabled
      Mixer.Motor[i][MIXER_GAS]   = 0;
      Mixer.Motor[i][MIXER_PITCH] = 0;
      Mixer.Motor[i][MIXER_ROLL]  = 0;
      Mixer.Motor[i][MIXER_YAW]   = 0;
    }
    // default to hexa (copied from MKTools) 
    //        GAS             NICK                    ROLL                        YAW
    Mixer.Motor[0][MIXER_GAS] = +64; Mixer.Motor[0][MIXER_PITCH] = +64; Mixer.Motor[0][MIXER_ROLL] =   0; Mixer.Motor[0][MIXER_YAW] = +64;
    Mixer.Motor[1][MIXER_GAS] = +64; Mixer.Motor[1][MIXER_PITCH] = +64; Mixer.Motor[1][MIXER_ROLL] = -64; Mixer.Motor[1][MIXER_YAW] = -64;
    Mixer.Motor[2][MIXER_GAS] = +64; Mixer.Motor[2][MIXER_PITCH] = -64; Mixer.Motor[2][MIXER_ROLL] = -64; Mixer.Motor[2][MIXER_YAW] = +64;
    Mixer.Motor[3][MIXER_GAS] = +64; Mixer.Motor[3][MIXER_PITCH] = -64; Mixer.Motor[3][MIXER_ROLL] =   0; Mixer.Motor[3][MIXER_YAW] = -64;
    Mixer.Motor[4][MIXER_GAS] = +64; Mixer.Motor[4][MIXER_PITCH] = -64; Mixer.Motor[4][MIXER_ROLL] = +64; Mixer.Motor[4][MIXER_YAW] = +64;
    Mixer.Motor[5][MIXER_GAS] = +64; Mixer.Motor[5][MIXER_PITCH] = +64; Mixer.Motor[5][MIXER_ROLL] = +64; Mixer.Motor[5][MIXER_YAW] = -64;
    Mixer.Revision = MIXER_REVISION;
    memcpy(Mixer.Name, "Hexa\0", 11);
    eeprom_write_block(&Mixer, &EEPromArray[EEPROM_ADR_MIXER_TABLE], sizeof(Mixer));
  }
  printf("\n\rMixer-Config: '%s' (%u Motors)",Mixer.Name,RequiredMotors);

  // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  // + Check connected BL-Ctrls
  // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  motorread = 0;   
  UpdateMotor = 0;
  sendMotorData(); 
  while(!UpdateMotor){
    //do nothing...
  }
  motorread = 0;  // read the first I2C-Data
  printf("\n\rFound BL-Ctrl: ");
  timer = SetDelay(4000);

  for(i=0; i < MAX_MOTORS; i++){
    UpdateMotor = 0;
    sendMotorData();
    while(!UpdateMotor){
      //do nothing...
    }

    if(Mixer.Motor[i][MIXER_GAS] > 0){//this motor is active
      // wait max 4 sec for the BL-Ctrls to wake up
      while(!CheckDelay(timer) && !(Motor[i].State & MOTOR_STATE_PRESENT_MASK) ){
        UpdateMotor = 0; 
        sendMotorData(); 
        while(!UpdateMotor){
          //do nothing...
        }
      }
    }
    if(Motor[i].State & MOTOR_STATE_PRESENT_MASK){
      printf("%d ",i+1);
    }
  }

  for(i=0; i < MAX_MOTORS; i++){
    if(!(Motor[i].State & MOTOR_STATE_PRESENT_MASK) && Mixer.Motor[i][MIXER_GAS] > 0){
      printf("\n\r\n\r!! MISSING BL-CTRL: %d !!",i+1);
      ServoActive = 1; // just in case the FC would be used as camera-stabilizer
    }
    Motor[i].State &= ~MOTOR_STATE_ERROR_MASK; // clear error counter
  }
  printf("\n\r===================================");
  sendMotorData();
  //printf("\n size: %u",STRUCT_PARAM_LAENGE);

  // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  // + Check Settings
  // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  if(eeprom_read_byte(&EEPromArray[EEPROM_ADR_VALID]) != EE_DATENREVISION){
    DefaultKonstanten1();
    printf("\n\rInit. EEPROM");
    for (unsigned char i=1;i<6;i++){
      if(i==2) 
        DefaultKonstanten2(); // Kamera
      if(i==3) 
        DefaultKonstanten3(); // Beginner
      if(i>3)  
        DefaultKonstanten2(); // Kamera

      EE_Parameter.Gyro_D = 5;
      EE_Parameter.Driftkomp = 0;
      EE_Parameter.GyroAccFaktor = 27;
      EE_Parameter.WinkelUmschlagNick = 78;
      EE_Parameter.WinkelUmschlagRoll = 78;

      // valid Stick-Settings?
      if(eeprom_read_byte(&EEPromArray[EEPROM_ADR_CHANNELS])   < 12 && 
         eeprom_read_byte(&EEPromArray[EEPROM_ADR_CHANNELS+1]) < 12 && 
         eeprom_read_byte(&EEPromArray[EEPROM_ADR_CHANNELS+2]) < 12 && 
         eeprom_read_byte(&EEPromArray[EEPROM_ADR_CHANNELS+3]) < 12){
        EE_Parameter.Kanalbelegung[0] = eeprom_read_byte(&EEPromArray[EEPROM_ADR_CHANNELS+0]);
        EE_Parameter.Kanalbelegung[1] = eeprom_read_byte(&EEPromArray[EEPROM_ADR_CHANNELS+1]);
        EE_Parameter.Kanalbelegung[2] = eeprom_read_byte(&EEPromArray[EEPROM_ADR_CHANNELS+2]);
        EE_Parameter.Kanalbelegung[3] = eeprom_read_byte(&EEPromArray[EEPROM_ADR_CHANNELS+3]);
        EE_Parameter.Kanalbelegung[4] = eeprom_read_byte(&EEPromArray[EEPROM_ADR_CHANNELS+4]);
        EE_Parameter.Kanalbelegung[5] = eeprom_read_byte(&EEPromArray[EEPROM_ADR_CHANNELS+5]);
        EE_Parameter.Kanalbelegung[6] = eeprom_read_byte(&EEPromArray[EEPROM_ADR_CHANNELS+6]);
        EE_Parameter.Kanalbelegung[7] = eeprom_read_byte(&EEPromArray[EEPROM_ADR_CHANNELS+7]);

        if(eeprom_read_byte(&EEPromArray[EEPROM_ADR_CHANNELS+8]) < 255){
          EE_Parameter.Kanalbelegung[CH_POTI5] = eeprom_read_byte(&EEPromArray[EEPROM_ADR_CHANNELS+8]);
          EE_Parameter.Kanalbelegung[CH_POTI6] = eeprom_read_byte(&EEPromArray[EEPROM_ADR_CHANNELS+9]);
          EE_Parameter.Kanalbelegung[CH_POTI7] = eeprom_read_byte(&EEPromArray[EEPROM_ADR_CHANNELS+10]);
          EE_Parameter.Kanalbelegung[CH_POTI8] = eeprom_read_byte(&EEPromArray[EEPROM_ADR_CHANNELS+11]);
        }
        else{
          EE_Parameter.Kanalbelegung[CH_POTI5] = 9;
          EE_Parameter.Kanalbelegung[CH_POTI6] = 10;
          EE_Parameter.Kanalbelegung[CH_POTI7] = 11;
          EE_Parameter.Kanalbelegung[CH_POTI8] = 12;
        }
        if(i==1){
          printf(": Generating Default-Parameter using old Stick Settings");
        }
      }
      else{
        DefaultStickMapping();
      }
      WriteParameterSet(i, (unsigned char *) &EE_Parameter.Kanalbelegung[0], STRUCT_PARAM_LAENGE);
    }
    SetActiveParamSetNumber(3); // default-Setting
    eeprom_write_byte(&EEPromArray[EEPROM_ADR_VALID], EE_DATENREVISION);
  }

  FlugMinuten = (int)eeprom_read_byte(&EEPromArray[EEPROM_ADR_MINUTES2]) * 256 + (int)eeprom_read_byte(&EEPromArray[EEPROM_ADR_MINUTES2+1]);
  FlugMinutenGesamt = (int)eeprom_read_byte(&EEPromArray[EEPROM_ADR_MINUTES]) * 256 + (int)eeprom_read_byte(&EEPromArray[EEPROM_ADR_MINUTES+1]);
  if(FlugMinutenGesamt == 0xffff || FlugMinuten == 0xffff){//overflow
    FlugMinuten = 0;
    FlugMinutenGesamt = 0;
  }
  printf("\n\rFlight-time %u min  Total:%u min" ,FlugMinuten,FlugMinutenGesamt);

  if(eeprom_read_byte(&EEPromArray[EEPROM_ADR_ACC_NICK]) > 4){
    printf("\n\rACC not calibrated !");
  }

  ReadParameterSet(GetActiveParamSetNumber(), (unsigned char *) &EE_Parameter.Kanalbelegung[0], STRUCT_PARAM_LAENGE);
  printf("\n\rUsing parameterset %d", GetActiveParamSetNumber());

  printf("\n\rCalibrating pressure sensor..");
  timer = SetDelay(1000);
  findAirpressureOffset();

  while(!CheckDelay(timer)){
    //do nothing... 
  }

  printf("OK\n\r");

  SetNeutral();

  ROT_OFF;

  beeptime = 2000;
  ExternControl.Digital[0] = 0x55;


  printf("\n\rControl: ");
  if (EE_Parameter.GlobalConfig & CFG_HEADING_HOLD){
    printf("HeadingHold");
  }
  else{
    printf("Normal (ACC-Mode)");
  }

  LcdClear();
  I2CTimeout = 5000;
  WinkelOut.Orientation = 1;
  LipoDetection(1);

  LIBFC_ReceiverInit();

  printf("\n\r===================================\n\r");
  //SpektrumBinding();
  timer = SetDelay(2000);
  timerPolling = SetDelay(250);

  while(1){//runs very quickly - ~20kHz
    if(CheckDelay(timerPolling)){
      timerPolling = SetDelay(100);
      LIBFC_Polling(); //this function lives inside libfc...
    }

    if(UpdateMotor && AdReady){// ReglerIntervall
      UpdateMotor=0;
      if(WinkelOut.CalcState){
        CalMk3Mag();
      }
      else{
        motorController();
      }

      sendMotorData();
      ROT_OFF;
      if(SenderOkay){
        SenderOkay--;
      }
      else{
        TIMSK1 |= _BV(ICIE1); // enable PPM-Input
      }

      if(!--I2CTimeout || MissingMotor){
        if(!I2CTimeout){
          i2c_reset();
          I2CTimeout = 5;
          DebugOut.Analog[28]++; // I2C-Error
          FCFlags |= FCFLAG_I2CERR;
        }
        if((BeepMuster == 0xffff) && vehicleIsFlying){
          beeptime = 10000;
          BeepMuster = 0x0080;
        }
      }
      else{
        ROT_OFF;
        if(!beeptime){
          FCFlags &= ~FCFLAG_I2CERR;
        }
      }

      if(SIO_DEBUG && (!UpdateMotor || !vehicleIsFlying)){//SIO_DEBUG is hash-defined to 1 ("should debugger be activated?")
        DatenUebertragung();//sends data out
      }
      BearbeiteRxDaten();//check for incoming data

      if(CheckDelay(timer)){//?this runs at 20ms
        static unsigned char second; //this counts the number of seconds passed
        timer += 20; // 20 ms intervalUSE_NAVICTRL
        if(PcZugriff){
          PcZugriff--;
        }
        else{
          ExternControl.Config = 0;
          if(BeepMuster == 0xffff && SenderOkay == 0){
            beeptime = 15000;
            BeepMuster = 0x0c00;
          }
        }
        if(NaviDataOkay){
          NaviDataOkay--;
          FCFlags &= ~FCFLAG_SPI_RX_ERR;
        }
        else{
          GPS_Nick = 0;
          GPS_Roll = 0;
          if(!beeptime){
            FCFlags |= FCFLAG_SPI_RX_ERR;
          }
        }

        if(UBat < BattLowVoltageWarning){
          FCFlags |= FCFLAG_LOWBAT;
          if(BeepMuster == 0xffff){
            beeptime = 6000;
            BeepMuster = 0x0300;
          }
        }else if(!beeptime){
          FCFlags &= ~FCFLAG_LOWBAT;
        }

        SPI_StartTransmitPacket();
        SendSPI = 4;
        if(!vehicleIsFlying){
          timer2 = 1450; // 0,5 Minuten aufrunden
        }
        else if(++second == 49){
          second = 0;
          FlugSekunden++;
          sprintf(markDebugString,"Time passed = %dsec",FlugSekunden);
        }

        if(++timer2 == 2930){// one minute
          timer2 = 0;
          FlugMinuten++;
          FlugMinutenGesamt++;
          eeprom_write_byte(&EEPromArray[EEPROM_ADR_MINUTES2],   FlugMinuten / 256);
          eeprom_write_byte(&EEPromArray[EEPROM_ADR_MINUTES2+1], FlugMinuten % 256);
          eeprom_write_byte(&EEPromArray[EEPROM_ADR_MINUTES],    FlugMinutenGesamt / 256);
          eeprom_write_byte(&EEPromArray[EEPROM_ADR_MINUTES+1],  FlugMinutenGesamt % 256);
          timer = SetDelay(20); // falls "timer += 20;" mal nicht geht
        }
      }

      LED_Update();
      Capacity_Update();
    }

    if(!SendSPI){ 
      SPI_TransmitByte(); 
    }
  }
  return (1);
}
