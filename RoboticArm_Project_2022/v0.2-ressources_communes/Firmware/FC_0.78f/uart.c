/* COPYRIGHT:
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + Copyright (c) Holger Buss, Ingo Busker
// + only for non-profit use
// + www.MikroKopter.com
// + see the File "License.txt" for further Informations
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// :copyright */
#include <stdarg.h>
#include <string.h>
#include <avr/pgmspace.h>
#include "main.h"
#include "uart.h"
#include "libfc.h"

#define ABO_TIMEOUT 4000 // disable debug data subscription after 4 seconds
#define MAX_SENDE_BUFF     160
#define MAX_EMPFANGS_BUFF  160

unsigned char GetExternalControl = 0,DebugDisplayAnforderung1 = 0, DebugDisplayAnforderung = 0,DebugDataAnforderung = 0,GetVersionAnforderung = 0, GetPPMChannelAnforderung = 0;
unsigned char GetHILSStatus = 0;
unsigned char GetA2DValues = 0;
unsigned char DisplayLine = 0;
unsigned volatile char SioTmp = 0;
unsigned volatile char NeuerDatensatzEmpfangen = 0;
unsigned volatile char NeueKoordinateEmpfangen = 0;
unsigned volatile char TransmissionComplete = 1;
unsigned volatile char CntCrcError = 0;
unsigned volatile char AnzahlEmpfangsBytes = 0;
unsigned volatile char TxdBuffer[MAX_SENDE_BUFF];
unsigned volatile char RxdBuffer[MAX_EMPFANGS_BUFF];

unsigned char *pRxData = 0;
unsigned char RxDataLen = 0;
unsigned volatile char PC_DebugTimeout = 0;
unsigned volatile char PC_MotortestActive = 0;

volatile unsigned char shouldPrintInfoStrings = 1; //a boolean which governs whether we send info strings

unsigned char DebugTextAnforderung = 255;
unsigned char PcZugriff = 100;
unsigned char MotorTest[16];
unsigned char MeineSlaveAdresse = 1; // Flight-Ctrl
unsigned char ConfirmFrame;
struct str_DebugOut DebugOut;
struct str_ExternControl ExternControl;
struct str_VersionInfo VersionInfo;
struct str_WinkelOut WinkelOut;
struct str_Data3D Data3D;

int Display_Timer, Debug_Timer,Kompass_Timer,Timer3D;
unsigned int DebugDataIntervall = 0, Intervall3D = 0, Display_Interval = 0;
unsigned int AboTimeOut = 0;

volatile struct SimulatedSensorData simulatedSensorData;

volatile unsigned char haveDisabledHILS = 0;

const unsigned char ANALOG_TEXT[32][16] PROGMEM = {
//"0123456789012345"
  "AngleNick       ", //0
  "AngleRoll       ",
  "AccNick         ",
  "AccRoll         ",
  "YawGyro         ",
  "Height Value    ", //5
  "AccZ            ",
  "Gas             ",
  "Compass Value   ",
  "Voltage [0.1V]  ",
  "Receiver Level  ", //10
  "Gyro Compass    ",
  "Motor 1         ",
  "Motor 2         ",
  "Motor 3         ",
  "Motor 4         ", //15
  "Motor 5         ",
  "Motor 6         ",
  "VarioMeter      ",
  "MK3Mag CalState ",
  "Servo           ", //20
  "Hovergas        ",
  "Current [0.1A]  ",
  "Capacity [mAh]  ",
  "mdebug1         ",
  "mdebug2         ", //25
  "mdebug3         ",
  "mdebug4         ",
  "I2C-Error       ",
  "                ", //    "Navi Serial Data",
  "GPS_Nick        ", //30
  "GPS_Roll        "
};

ISR(USART0_TX_vect){//Data transmission
  static unsigned int ptr = 0;
  unsigned char tmp_tx;
  if(!TransmissionComplete){
    ptr++;// the [0] has already been sent
    tmp_tx = TxdBuffer[ptr];
    if((tmp_tx == '\r') || (ptr == MAX_SENDE_BUFF)){
      ptr = 0;
      TransmissionComplete = 1;
    }
    UDR0 = tmp_tx;
  }
  else{
    ptr = 0;
  }
}

ISR(USART0_RX_vect){//Data reception, incl. CRC check
  static unsigned int crc;
  static unsigned char crc1,crc2,buf_ptr;
  static unsigned char UartState = 0;
  unsigned char CrcOkay = 0;

  SioTmp = UDR0;
  if(buf_ptr >= MAX_SENDE_BUFF){
    UartState = 0;
  }

  if(SioTmp == '\r' && UartState == 2){
    //check CRC:
    UartState = 0;
    crc -= RxdBuffer[buf_ptr-2];
    crc -= RxdBuffer[buf_ptr-1];
    crc %= 4096;
    crc1 = '=' + crc / 64;
    crc2 = '=' + crc % 64;
    CrcOkay = 0;
    if((crc1 == RxdBuffer[buf_ptr-2]) && (crc2 == RxdBuffer[buf_ptr-1])){
      CrcOkay = 1; 
    }
    else{
      CrcOkay = 0; 
      CntCrcError++;
    }

    if(!NeuerDatensatzEmpfangen && CrcOkay){
      //Data has been processed
      NeuerDatensatzEmpfangen = 1;
      AnzahlEmpfangsBytes = buf_ptr + 1;
      RxdBuffer[buf_ptr] = '\r';
      if(RxdBuffer[2] == 'R'){
        LcdClear();
        wdt_enable(WDTO_250MS); // Reset-Commando
        ServoActive = 0;
      }
    }
  }
  else{
    switch(UartState){
    case 0:
      if(SioTmp == '#' && !NeuerDatensatzEmpfangen){
        UartState = 1;  // Startzeichen und Daten schon verarbeitet
      }
      buf_ptr = 0;
      RxdBuffer[buf_ptr++] = SioTmp;
      crc = SioTmp;
      break;

    case 1: // Adresse auswerten
      UartState++;
      RxdBuffer[buf_ptr++] = SioTmp;
      crc += SioTmp;
      break;

    case 2: //  Eingangsdaten sammeln
      RxdBuffer[buf_ptr] = SioTmp;
      if(buf_ptr < MAX_EMPFANGS_BUFF){
        buf_ptr++;
      }
      else{
        UartState = 0;
      }
      crc += SioTmp;
      break;

    default:
      UartState = 0;
      break;
    }
  }
}

void AddCRC(unsigned int amount){
  unsigned int tmpCRC = 0,i;
  for(i = 0; i<amount; i++){
    tmpCRC += TxdBuffer[i];
  }

  tmpCRC %= 4096;
  TxdBuffer[i++] = '=' + tmpCRC / 64;
  TxdBuffer[i++] = '=' + tmpCRC % 64;
  TxdBuffer[i++] = '\r';
  TransmissionComplete = 0;
  UDR0 = TxdBuffer[0];
}

void SendOutData(unsigned char cmd,unsigned char address, unsigned char BufferAnzahl, ...){
  va_list ap;
  unsigned int pt = 0;
  unsigned char a,b,c;
  unsigned char ptr = 0;

  unsigned char *snd = 0;
  int len = 0;

  TxdBuffer[pt++] = '#';          // Start char
  TxdBuffer[pt++] = 'a' + address;// Address ('a'=0; 'b'=1,...)
  TxdBuffer[pt++] = cmd;          // Command 

  va_start(ap, BufferAnzahl);
  if(BufferAnzahl){
    snd = va_arg(ap, unsigned char*);
    len = va_arg(ap, int);
    ptr = 0;
    BufferAnzahl--;
  }

  while(len){
    if(len){
      a = snd[ptr++];
      len--;
      if((!len) && BufferAnzahl)
      {
        snd = va_arg(ap, unsigned char*);
        len = va_arg(ap, int);
        ptr = 0;
        BufferAnzahl--;
      }
    }
    else{//mwm: this will never be executed...
      a = 0;
    }

    if(len){
      b = snd[ptr++];
      len--;
      if((!len) && BufferAnzahl)
      {
        snd = va_arg(ap, unsigned char*);
        len = va_arg(ap, int);
        ptr = 0;
        BufferAnzahl--;
      }
    }
    else{
      b = 0;
    }

    if(len){
      c = snd[ptr++];
      len--;
      if((!len) && BufferAnzahl){
        snd = va_arg(ap, unsigned char*);
        len = va_arg(ap, int);
        ptr = 0;
        BufferAnzahl--;
      }
    }
    else{
      c = 0;
    }

    TxdBuffer[pt++] = '=' + (a >> 2);
    TxdBuffer[pt++] = '=' + (((a & 0x03) << 4) | ((b & 0xf0) >> 4));
    TxdBuffer[pt++] = '=' + (((b & 0x0f) << 2) | ((c & 0xc0) >> 6));
    TxdBuffer[pt++] = '=' + ( c & 0x3f);
  }
  va_end(ap);
  AddCRC(pt);
}

void Decode64(void){// die daten werden im rx buffer dekodiert, das geht nur, weil aus 4 byte immer 3 gemacht werden.
  unsigned char a,b,c,d;
  unsigned char x,y,z;
  unsigned char ptrIn = 3; // start at begin of data block
  unsigned char ptrOut = 3;
  unsigned char len = AnzahlEmpfangsBytes - 6; // von der Gesamtbytezahl eines Frames gehen 3 Bytes des Headers  ('#',Addr, Cmd) und 3 Bytes des Footers (CRC1, CRC2, '\r') ab.

  while(len){
    a = RxdBuffer[ptrIn++] - '=';
    b = RxdBuffer[ptrIn++] - '=';
    c = RxdBuffer[ptrIn++] - '=';
    d = RxdBuffer[ptrIn++] - '=';

    x = (a << 2) | (b >> 4);
    y = ((b & 0x0f) << 4) | (c >> 2);
    z = ((c & 0x03) << 6) | d;

    if(len--) RxdBuffer[ptrOut++] = x; else break;
    if(len--) RxdBuffer[ptrOut++] = y; else break;
    if(len--) RxdBuffer[ptrOut++] = z; else break;
  }
  pRxData = (unsigned char*)&RxdBuffer[3]; // decodierte Daten beginnen beim 4. Byte
  RxDataLen = ptrOut - 3;  // wie viele Bytes wurden dekodiert?

}

void BearbeiteRxDaten(void){
  if(!NeuerDatensatzEmpfangen){
    return;
  }

  unsigned char tempchar1, tempchar2;
  Decode64(); //decode the data in the receiver block

  if(enableHILS != HILS_REAL_FLIGHT){
    //we're doing HILS -- only allow some comms

    switch(RxdBuffer[2]){
    case '{'://received HILS sensor pack data
      memcpy((unsigned char *)&simulatedSensorData.sensors, (unsigned char *)pRxData, sizeof(simulatedSensorData.sensors));
      simulatedSensorData.controllerCycleWhenReceived = controllerCycleNumber; //timestamp
      break;
    case '('://(de-)activate HILS mode
      enableHILS = (enum TypeHILS) pRxData[0];
      if(enableHILS != HILS_REAL_FLIGHT){//we've deactivated the HILS
        haveDisabledHILS = 1;
      }
      break;
    case '['://received request for HILS enabled state
      GetHILSStatus = 1;
      PcZugriff = 255;
      break;
    }
  }
  else{//allow all communications

    switch(RxdBuffer[1]-'a') // check for Slave Address
    {
    case FC_ADDRESS: // FC special commands
      switch(RxdBuffer[2])
      {
      case 'K':// Kompasswert
        memcpy((unsigned char *)&KompassValue , (unsigned char *)pRxData, sizeof(KompassValue));
        KompassRichtung = ((540 + KompassValue - KompassStartwert) % 360) - 180;
        break;
      case 't':// Motortest
        if(AnzahlEmpfangsBytes > 20)
        {
          memcpy(&MotorTest[0], (unsigned char *)pRxData, sizeof(MotorTest));
        }else 
        {
          memcpy(&MotorTest[0], (unsigned char *)pRxData, 4);
        }
        PC_MotortestActive = 240;
        PcZugriff = 255;
        break;

      case 'n':// Get Mixer
        while(!TransmissionComplete)
        {
          //wait...
        }
        SendOutData('N', FC_ADDRESS, 1, (unsigned char *) &Mixer,sizeof(Mixer));
        break;

      case 'm':// Write Mixer
        while(!TransmissionComplete)
        {
          //wait...
        }
        if(pRxData[0] == MIXER_REVISION)
        {
          memcpy(&Mixer, (unsigned char *)pRxData, sizeof(Mixer));
          eeprom_write_block(&Mixer, &EEPromArray[EEPROM_ADR_MIXER_TABLE], sizeof(Mixer));
          tempchar1 = 1;
        }else  
        {
          tempchar1 = 0;
        }
        SendOutData('M', FC_ADDRESS, 1, &tempchar1, sizeof(tempchar1));
        break;

      case 'p': // get PPM Channels
        GetPPMChannelAnforderung = 1;
        PcZugriff = 255;
        break;

      case 'q':// Get-Anforderung für Settings
        // Bei Get werden die vom PC einstellbaren Werte vom PC zurückgelesen
        if(pRxData[0] == 0xFF)
        {
          pRxData[0] = GetActiveParamSetNumber();
        }
        // limit settings range
        if(pRxData[0] < 1)
        {
          pRxData[0] = 1; // limit to 5
        }else if(pRxData[0] > 5)
        {
          pRxData[0] = 5; // limit to 5
        }
        // load requested parameter set
        ReadParameterSet(pRxData[0], (unsigned char *) &EE_Parameter.Kanalbelegung[0], STRUCT_PARAM_LAENGE);

        while(!TransmissionComplete)
        {
          //wait...
        }
        tempchar1 = pRxData[0];
        tempchar2 = EE_DATENREVISION;
        SendOutData('Q', FC_ADDRESS, 3, &tempchar1, sizeof(tempchar1), &tempchar2, sizeof(tempchar2), (unsigned char *) &EE_Parameter.Kanalbelegung[0], STRUCT_PARAM_LAENGE);
        break;

      case 's': // Parametersatz speichern
        if((1 <= pRxData[0]) && (pRxData[0] <= 5) && (pRxData[1] == EE_DATENREVISION)) // check for setting to be in range
        {
          memcpy((unsigned char *) &EE_Parameter.Kanalbelegung[0], (unsigned char *)&pRxData[2], STRUCT_PARAM_LAENGE);
          WriteParameterSet(pRxData[0], (unsigned char *) &EE_Parameter.Kanalbelegung[0], STRUCT_PARAM_LAENGE);
          wrapAround180.pitch = (long) EE_Parameter.WinkelUmschlagNick * 2500L;
          wrapAround180.roll  = (long) EE_Parameter.WinkelUmschlagRoll * 2500L;
          SetActiveParamSetNumber(pRxData[0]);
          tempchar1 = GetActiveParamSetNumber();
        }else
        {
          tempchar1 = 0; // mark in response an invlid setting
        }

        while(!TransmissionComplete)
        {
          //wait...
        }

        SendOutData('S', FC_ADDRESS, 1, &tempchar1, sizeof(tempchar1));
        if(!vehicleIsFlying)
        {
          Piep(tempchar1,110);
        }
        LipoDetection(0);
        LIBFC_ReceiverInit();
        break;

      case 'f': // auf anderen Parametersatz umschalten
        if((1 <= pRxData[0]) && (pRxData[0] <= 5))
        {
          SetActiveParamSetNumber(pRxData[0]);
        }
        tempchar1 = pRxData[0];
        ReadParameterSet(tempchar1, (unsigned char *) &EE_Parameter.Kanalbelegung[0], STRUCT_PARAM_LAENGE);
        while(!TransmissionComplete)
        {
          //wait...
        }
        SendOutData('F', FC_ADDRESS, 1, &tempchar1, sizeof(tempchar1));
        if(!vehicleIsFlying)
        {
          Piep(tempchar1,110);
        }
        LipoDetection(0);
        LIBFC_ReceiverInit();
        break;

      case 'y':// serial Potis
        PPM_in[13] = (signed char) pRxData[0];  PPM_in[14] = (signed char) pRxData[1]; 
        PPM_in[15] = (signed char) pRxData[2];  PPM_in[16] = (signed char) pRxData[3];
        PPM_in[17] = (signed char) pRxData[4];  PPM_in[18] = (signed char) pRxData[5]; 
        PPM_in[19] = (signed char) pRxData[6];  PPM_in[20] = (signed char) pRxData[7];
        PPM_in[21] = (signed char) pRxData[8];  PPM_in[22] = (signed char) pRxData[9]; 
        PPM_in[23] = (signed char) pRxData[10]; PPM_in[24] = (signed char) pRxData[11];
        break;

      case 'i':// Set whether to send info strings
        shouldPrintInfoStrings = pRxData[0];
        PcZugriff = 255;
        break;

      case '('://activate HILS mode
        enableHILS = (enum TypeHILS) pRxData[0];
        break;

      case '['://received request for HILS enabled state
        GetHILSStatus = 1;
        PcZugriff = 255;
        break;

      case '<'://received request for A2D values
        GetA2DValues = 1;
        PcZugriff = 255;
        break;

      }//case FC_ADDRESS:

    default: // any Slave Address (i.e. not FC_ADDRESS)

      switch(RxdBuffer[2])
      {
      case 't':// Motortest// 't' comand placed here only for compatibility to BL
        if(AnzahlEmpfangsBytes > 20)
        {
          memcpy(&MotorTest[0], (unsigned char *)pRxData, sizeof(MotorTest));
        }else 
        {
          memcpy(&MotorTest[0], (unsigned char *)pRxData, 4);
        }

        while(!TransmissionComplete)
        {
          //wait...
        }
        SendOutData('T', MeineSlaveAdresse, 0);
        PC_MotortestActive = 250;
        PcZugriff = 255;
        break;
        
      case 'K':// Kompasswert// 'K' comand placed here only for compatibility to old MK3MAG software, that does not send the right Slave Address
        memcpy((unsigned char *)&KompassValue , (unsigned char *)pRxData, sizeof(KompassValue));
        KompassRichtung = ((540 + KompassValue - KompassStartwert) % 360) - 180;
        break;

      case 'a':// Texte der Analogwerte
        DebugTextAnforderung = pRxData[0];
        if (DebugTextAnforderung > 31)
        {
          DebugTextAnforderung = 31;
        }
        PcZugriff = 255;
        break;

      case 'b':
        memcpy((unsigned char *)&ExternControl, (unsigned char *)pRxData, sizeof(ExternControl));
        ConfirmFrame = ExternControl.Frame;
        PcZugriff = 255;
        break;

      case 'c': // Poll the 3D-Data
        if(!Intervall3D)
        {
          if(pRxData[0])
          {
            Timer3D = SetDelay(pRxData[0] * 10);
          }
        }
        Intervall3D = pRxData[0] * 10;
        AboTimeOut = SetDelay(ABO_TIMEOUT);
        break;

      case 'd': // Poll the debug data
        PcZugriff = 255;
        DebugDataIntervall = (unsigned int)pRxData[0] * 10;
        if(DebugDataIntervall > 0)
        {
          DebugDataAnforderung = 1;
        }
        AboTimeOut = SetDelay(ABO_TIMEOUT);
        break;

      case 'h':// x-1 Displayzeilen
        PcZugriff = 255;
        if((pRxData[0] & 0x80) == 0x00) // old format
        {
          DisplayLine = 2;
          Display_Interval = 0;
        }else // new format
        {
          RemoteKeys |= ~pRxData[0];
          Display_Interval = (unsigned int)pRxData[1] * 10;
          DisplayLine = 4;
          AboTimeOut = SetDelay(ABO_TIMEOUT);
        }
        DebugDisplayAnforderung = 1;
        break;

      case 'l':// x-1 Displayzeilen
        PcZugriff = 255;
        MenuePunkt = pRxData[0];
        DebugDisplayAnforderung1 = 1;
        break;

      case 'v': // Version-Anforderung und Ausbaustufe
        GetVersionAnforderung = 1;
        break;

      case 'g'://
        GetExternalControl = 1;
        break;
      }
      break; // default:
    }
  }
  NeuerDatensatzEmpfangen = 0;
  pRxData = 0;
  RxDataLen = 0;
}

void uart_putchar (char c){//Routine for serial output
  //Warten solange bis Zeichen gesendet wurde
  loop_until_bit_is_set(UCSR0A, UDRE0);
  //Ausgabe des Zeichens
  UDR0 = c;
}

void UART_Init (void){//Installation of the serial inteface
  unsigned int ubrr = (unsigned int) ((unsigned long) F_CPU/(8 * USE_BAUD_RATE) - 1);

  //Enable TXEN im Register UCR TX-Data Enable & RX Enable
  UCSR0B = (1 << TXEN0) | (1 << RXEN0);
  // UART Double Speed (U2X)
  UCSR0A |= (1 << U2X0);
  // RX-Interrupt Freigabe
  UCSR0B |= (1 << RXCIE0);
  // TX-Interrupt Freigabe
  UCSR0B |= (1 << TXCIE0);
  // USART0 Baud Rate Register
  // set clock divider
  UBRR0H = (uint8_t)(ubrr >> 8);
  UBRR0L = (uint8_t)ubrr;

  Debug_Timer = SetDelay(DebugDataIntervall);
  Kompass_Timer = SetDelay(220);

  VersionInfo.SWMajor     = VERSION_MAJOR;
  VersionInfo.SWMinor     = VERSION_MINOR;
  VersionInfo.SWPatch     = VERSION_PATCH;
  VersionInfo.ProtoMajor  = VERSION_SERIAL_MAJOR;
  VersionInfo.ProtoMinor  = VERSION_SERIAL_MINOR;

  pRxData = 0;
  RxDataLen = 0;
}

void DatenUebertragung(void){
  static unsigned char lastSensorIdDataSent = 123;

  if(!TransmissionComplete)
    return;

  //-----------------Things which we do for both HILS and real flight-------------------------------
  if(GetHILSStatus && TransmissionComplete){
    char enableHILS_char = (unsigned char) enableHILS;
    SendOutData(']', FC_ADDRESS, 1, (unsigned char *) &enableHILS_char, sizeof(enableHILS_char));
    GetHILSStatus = 0;
  }
  
  if(markDebugString[0] && shouldPrintInfoStrings && TransmissionComplete){//send a debug string..
    SendOutData('I', FC_ADDRESS, 1, markDebugString, MARK_DEBUG_STRING_LENGTH);
    markDebugString[0] = 0;//empty the string
  }
  //-----------------Things which we do ONLY for HILS-----------------------------------------------
  if(enableHILS != HILS_REAL_FLIGHT){//we only do HILS-related comms, and return immediately
    if(motorCommandsHILS.haveDataToSend && (lastSensorIdDataSent!=motorCommandsHILS.commands.sensorDataID)){//send out motor commands
      SendOutData('}', FC_ADDRESS, 1, (unsigned char *) &motorCommandsHILS.commands, sizeof(motorCommandsHILS.commands));
      motorCommandsHILS.haveDataToSend=0;
      lastSensorIdDataSent = motorCommandsHILS.commands.sensorDataID;
    }

    return;
  }
  //-----------------Things which we do ONLY for real flight----------------------------------------
  if(CheckDelay(AboTimeOut)){
    Display_Interval = 0;
    DebugDataIntervall = 0;
    Intervall3D = 0;
  }

  if(GetA2DValues && TransmissionComplete){
    SendOutData('>', FC_ADDRESS, 1, (unsigned char *) &A2DValues, sizeof(A2DValues));
    GetA2DValues = 0;
  }

  if(((Display_Interval>0 && CheckDelay(Display_Timer)) || DebugDisplayAnforderung) && TransmissionComplete){
    if(DisplayLine > 3){// new format
      Menu();
      SendOutData('H', FC_ADDRESS, 1, (uint8_t *)DisplayBuff, 80);
    }
    else{// old format
      LCD_printfxy(0,0,"!!! INCOMPATIBLE !!!");
      SendOutData('H', FC_ADDRESS, 2, &DisplayLine, sizeof(DisplayLine), (uint8_t *)DisplayBuff, 20);
      if(DisplayLine++ > 3){
        DisplayLine = 0;
      }
    }
    Display_Timer = SetDelay(Display_Interval);
    DebugDisplayAnforderung = 0;
  }

  if(DebugDisplayAnforderung1 && TransmissionComplete){
    Menu();
    SendOutData('L', FC_ADDRESS, 3, &MenuePunkt, sizeof(MenuePunkt), &MaxMenue, sizeof(MaxMenue), DisplayBuff, sizeof(DisplayBuff));
    DebugDisplayAnforderung1 = 0;
  }

  if(GetVersionAnforderung && TransmissionComplete){
    SendOutData('V', FC_ADDRESS, 1, (unsigned char *) &VersionInfo, sizeof(VersionInfo));
    GetVersionAnforderung = 0;
  }

  if(GetExternalControl && TransmissionComplete){// Bei Get werden die vom PC einstellbaren Werte vom PC zurückgelesen
    SendOutData('G',MeineSlaveAdresse, 1, (unsigned char *) &ExternControl, sizeof(ExternControl));
    GetExternalControl = 0;
  }

  if((CheckDelay(Kompass_Timer)) && TransmissionComplete){
    WinkelOut.Winkel[0] = (int) (angleIntegral.pitch / (EE_Parameter.GyroAccFaktor * 4));  // etwa in 0.1 Grad
    WinkelOut.Winkel[1] = (int) (angleIntegral.roll  / (EE_Parameter.GyroAccFaktor * 4));  // etwa in 0.1 Grad
    WinkelOut.UserParameter[0] = Parameter_UserParam1;
    WinkelOut.UserParameter[1] = Parameter_UserParam2;
    SendOutData('w', MK3MAG_ADDRESS, 1, (unsigned char *) &WinkelOut,sizeof(WinkelOut));
    if(WinkelOut.CalcState > 4){
      WinkelOut.CalcState = 6; // wird dann in SPI auf Null gesetzt
    }
    Kompass_Timer = SetDelay(99);
  }

  if(((DebugDataIntervall>0 && CheckDelay(Debug_Timer)) || DebugDataAnforderung) && TransmissionComplete){
    SendOutData('D', FC_ADDRESS, 1, (unsigned char *) &DebugOut,sizeof(DebugOut));
    DebugDataAnforderung = 0;
    if(DebugDataIntervall>0){
      Debug_Timer = SetDelay(DebugDataIntervall);
    }
  }

  if(Intervall3D > 0 && CheckDelay(Timer3D) && TransmissionComplete){
    Data3D.Winkel[0] = (int) (angleIntegral.pitch / (EE_Parameter.GyroAccFaktor * 4));  // etwa in 0.1 Grad
    Data3D.Winkel[1] = (int) (angleIntegral.roll  / (EE_Parameter.GyroAccFaktor * 4));  // etwa in 0.1 Grad
    Data3D.Winkel[2] = (int) ((10 * ErsatzKompass) / GIER_GRAD_FAKTOR);
    SendOutData('C', FC_ADDRESS, 1, (unsigned char *) &Data3D,sizeof(Data3D));
    Timer3D = SetDelay(Intervall3D);
  }

  if(DebugTextAnforderung != 255){// Texte für die Analogdaten
    unsigned char label[16]; // local sram buffer
    memcpy_P(label, ANALOG_TEXT[DebugTextAnforderung], 16); // read lable from flash to sra
    SendOutData('A', FC_ADDRESS, 2, (unsigned char *)&DebugTextAnforderung, sizeof(DebugTextAnforderung),label, 16);
    DebugTextAnforderung = 255;
  }

  if(ConfirmFrame && TransmissionComplete){// Datensatz bestätigen
    SendOutData('B', FC_ADDRESS, 1, (uint8_t*)&ConfirmFrame, sizeof(ConfirmFrame));
    ConfirmFrame = 0;
  }

  if(GetPPMChannelAnforderung && TransmissionComplete){
    SendOutData('P', FC_ADDRESS, 1, (unsigned char *) &PPM_in, sizeof(PPM_in));
    GetPPMChannelAnforderung = 0;
  }
}
