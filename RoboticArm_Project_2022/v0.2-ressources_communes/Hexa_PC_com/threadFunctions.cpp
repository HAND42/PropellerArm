/* mwm: 01.06.2011 moved these fns from serialCommunicator.cpp
 */

#include "serialCommunicator.h"

/************************************************************************************
 * Thread functions                                                                 *
 ************************************************************************************/
DWORD WINAPI communicationThread::receiver(LPVOID lpParam)
{/* A function that we can call to create a thread reading the serial port
  *
  * Continuously polls the serial port, at a frequency determined by 
  * receiverThreadSleepTime_ms
  * If there is information to read, this is automatically interpreted and the 
  * object (serCom) is updated. 
  */
  

  //recast the LPVOID parameter into something useful
  SerialCommunicator* serCom = ((SerialCommunicator*)lpParam); 

  for(;;)
  {
    serCom->SerialRx();
    Sleep(serCom->getReceiverThreadSleepTime_ms());
  }
}

DWORD WINAPI communicationThread::generalTransmitter(LPVOID lpParam)
{/* A function that we can call to create a thread to send the data
  *
  * This function sends any commands in the current command queue, at a 
  * frequency determined by transmitterThreadSleepTime_ms.
  *
  * The command queue is populated using the request functions, e.g.
  *  serCom->requestVersionInfo();
  * but *NOT* when using the requestConfirmed functions.
  */

  //cast the LPVOID parameter into something useful
  SerialCommunicator* serCom = ((SerialCommunicator*)lpParam); 

  for(;;)
  {
    Sleep(serCom->getTransmitterThreadSleepTime_ms());
    if(!serCom->tryToLockTransmitter())
      continue;
    //we have the transmitter locked
    serCom->sendLastPacket();
    serCom->setUnlockTransmitter();
  }
}

//DWORD WINAPI communicationThread::rcRemoteRequestTransmitter(LPVOID lpParam)
//{/* A function that periodically requests the remote state
//  * 
//  * Use this function if you need to read the RC remote values
//  * continuously. It keeps sending requests for RC info at a frequency
//  * determined by rcCheckSleepTime_ms, and these
//  * values are read by the separate communicationThread::receiver thread.
//  */
//
//  //cast the LPVOID parameter into something useful
//  SerialCommunicator* serCom = ((SerialCommunicator*)lpParam); 
//
//  for(;;)
//  {
//    Sleep(serCom->getRcCheckSleepTime_ms());
//    serCom->sendRCStateRequestNow();
//  }
//}

DWORD WINAPI communicationThread::debugDataRequestTransmitter(LPVOID lpParam)
{/* A function that periodically requests debug data
  * 
  * Use this to renew the debug data subscription. Data is received by the
  * separate communicationThread::receiver thread.
  */

  //cast the LPVOID parameter into something useful
  SerialCommunicator* serCom = ((SerialCommunicator*)lpParam); 

  for(;;)
  {
    Sleep(serCom->getDebugDataSleepTime_ms());
    serCom->sendDebugDataRequestNow();
  }
}

DWORD WINAPI communicationThread::HILSSensorDataTransmitter(LPVOID lpParam)
{/* A function that sends the simulator-generated HILS sensor data */

  //cast the LPVOID parameter into something useful
  SerialCommunicator* serCom = ((SerialCommunicator*)lpParam); 

  for(;;)
  {
    Sleep(serCom->getHILSTransmitSleepTime_ms());
    if(serCom->getHaveNewSensorData())
    {
      serCom->sendHILSDataNow();
    }
  }
}
