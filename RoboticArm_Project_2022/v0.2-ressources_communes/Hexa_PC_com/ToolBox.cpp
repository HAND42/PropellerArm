#include "ToolBox.h"

// Datensatz nach 16bit Integer
int ToolBox::Data2Int(int *Data , int Start, bool is_signed)//(int *Data , int Start, bool is_signed)
{
    int Out = (Data[Start+1]<<8) | (Data[Start+0]);

    if ((Out > 32767) && (is_signed))
        Out = Out - 65536;

    return Out;
}

INT16 ToolBox::Data2Int(UINT8 *Data , int Start, bool is_signed)
{
    INT16 Out = (Data[Start+1]<<8) | (Data[Start+0]);
    if ((Out > 32767) && (is_signed))
    {
        Out = (INT16) (Out - 65536);
    }

    return Out;

}


//=====================================================================
// 11/02/2011: 
//=====================================================================
UINT8 ToolBox::Encode64(UINT8 TX_Data[serialComms::RXD_BUFFER_LEN], UINT8 Data[serialComms::RXD_BUFFER_LEN],unsigned int Length)
{
    // Data to encode are contained in Data, they have the length: Length
    // Data encoded are contained in TX_Data

    UINT8 pt = 0;
    UINT8 a,b,c; //unsigned char a,b,c;
    UINT8 ptr = 0;//unsigned char ptr = 0;

    while(Length > 0)
    {
        if(Length) { a = Data[ptr++]; Length--;} else a = 0;
        if(Length) { b = Data[ptr++]; Length--;} else b = 0;
        if(Length) { c = Data[ptr++]; Length--;} else c = 0;

        TX_Data[pt++] = '=' + (a >> 2);
        TX_Data[pt++] = '=' + (((a & 0x03) << 4) | ((b & 0xf0) >> 4));
        TX_Data[pt++] = '=' + (((b & 0x0f) << 2) | ((c & 0xc0) >> 6));
        TX_Data[pt++] = '=' + ( c & 0x3f );
    }
    TX_Data[pt] = 0;

    return pt; // returns the number of encoded data
}

//mwm: moved from commsFunctions
UINT8 ToolBox::Decode64(UINT8 decodedDataOut[serialComms::RXD_BUFFER_LEN], UINT8 encodedDataIn[serialComms::RXD_BUFFER_LEN], UINT8 ReceivedBytes)
{
  UINT8 nb_usefulData_decoded_; 

  UINT8 a,b,c,d;
  UINT8 x,y,z;
  UINT8 ptrIn = 3;
  UINT8 ptrOut = 0;
  UINT8 len = ReceivedBytes - 6; 
  //UINT8 len = ReceivedBytes from the USART, need to remove the first 3 and last 3
  // this is why, it starts at the 4th byte and stops three before the end. 

    assert(!(len%4));
  while (len >0 && encodedDataIn[ptrIn+ len-1] == '=')
  {
    len--;
  }

  //the *3/4 has something to do with conversion to 64b data
  //this gives you the number of bytes
  nb_usefulData_decoded_ = (len)*3/4; 
  len = ReceivedBytes - 6; 
  while(len)
  {
    a = encodedDataIn[ptrIn++] - '=';
    b = encodedDataIn[ptrIn++] - '=';
    c = encodedDataIn[ptrIn++] - '=';
    d = encodedDataIn[ptrIn++] - '=';

    //encoding process. refer to QMK tools
    x = (a << 2) | (b >> 4);
    y = ((b & 0x0f) << 4) | (c >> 2);
    z = ((c & 0x03) << 6) | d;

    if(len--) decodedDataOut[ptrOut++] = x; else break;
    if(len--) decodedDataOut[ptrOut++] = y; else break;
    if(len--) decodedDataOut[ptrOut++] = z; else break;
  }
  return nb_usefulData_decoded_; //This is the number of bytes that have been decoded
}


void ToolBox::AddCRC(UINT8 packet[150], unsigned int datalen)
{
    unsigned int tmpCRC = 0,i;
  for(i = 0; i < datalen;i++)
  {
    tmpCRC += packet[i];
  }
  tmpCRC %= 4096;
  packet[i++] = (UINT8) ('=' + tmpCRC / 64);
  packet[i++] = (UINT8) ('=' + tmpCRC % 64);
  packet[i++] = (UINT8) ('\r');
}
