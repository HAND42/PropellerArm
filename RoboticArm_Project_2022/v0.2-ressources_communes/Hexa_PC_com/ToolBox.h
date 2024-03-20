/***************************************************************************

 
 ***************************************************************************/
#ifndef TOOLBOX_H
#define TOOLBOX_H

//#include <Windows.h>
#include <assert.h>

#include "communicationDefines.h"

//mwm: Comes from QMK? Missing license...
//a set of tools for dataconversion

namespace ToolBox //made it a namespace, rather than a class
{
  int Data2Int(int *Data , int Start, bool is_signed = true);
  int16_t Data2Int(uint8_t *Data , int Start, bool is_signed = true);
  uint8_t Encode64(uint8_t TX_Data[serialComms::RXD_BUFFER_LEN], uint8_t Data[serialComms::RXD_BUFFER_LEN],unsigned int Length);
  uint8_t  Decode64(uint8_t decodedDataOut[serialComms::RXD_BUFFER_LEN], uint8_t encodedDataIn[serialComms::RXD_BUFFER_LEN], uint8_t ReceivedBytes);
  void AddCRC(uint8_t packet[serialComms::RXD_BUFFER_LEN], unsigned int datalen);
};

#endif // TOOLBOX_H
