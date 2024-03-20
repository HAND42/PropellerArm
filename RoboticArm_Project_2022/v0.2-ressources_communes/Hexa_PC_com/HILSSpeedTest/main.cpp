//**********************************************************************************
// Dr. Guillaume Ducard
// Guillaume_FTDI_driver.cpp : Defines the entry point for the console application.
// March 2011
// 
// mwm: 04.05.2011 Removing all non-essential files, simplifying
// mwm: 16.05.2011 Moved communication to a separate project, which is to be included
//                 if one wants to communicate. This main file is an example of how 
//                 to use the communicator.
// mwm: 31.05.2011 Added HILS support to read the vehicle's motor commands.
// mwm: 03.06.2011 Created a separate project to test HILS comms speed/latencies.
//**********************************************************************************

#include <stdio.h>
#include <windows.h> //needed for GetTickCount()

#include "../serialCommunicator.h"

bool reportSuccess(bool in)
{
  if(in)
  {  
    printf("OK\n");
  }else
  {
    printf("FAILED\n");
  }
  return in;
}

int main(void)
{
  /////////////////////////////////////////////////////////////////////////
  //Set up serial port
  /////////////////////////////////////////////////////////////////////////
  SerialCommunicator* communicator = new SerialCommunicator();
  if(!communicator->portSuccessfullyOpened())
  {
    printf("\n\nSerial communicator failed to open serial port. Exiting...\n");
    return 1;
  }

  printf("-----------------------------------------------------\n");
  printf(" Serial comms speed test \n");
  printf("-----------------------------------------------------\n\n");

  CreateThread(0,0,communicationThread::receiver,                  communicator,0,0); //create the receiver thread
  CreateThread(0,0,communicationThread::generalTransmitter,        communicator,0,0); //create the transmitter thread
  CreateThread(0,0,communicationThread::HILSSensorDataTransmitter, communicator,0,0); //create the HILS transmitter thread

  communicator->setConfirmRequestTimeout_ms(1000);
  communicator->setTransmitterThreadSleepTime_ms(100);

  printf("Switching into HILS mode:\n");
  if(communicator->switchIntoHILSMode(1))
  {
    printf("Failed to switch into HILS mode - exiting...\n");
    return 1;
  }
  printf("Successfully swtiched into HILS mode.\n");
  Sleep(1000);

  const unsigned long timeout = 1000;//ms

  unsigned long totalTime=0;
  unsigned int numberOfPacketsCameBack=0;
  unsigned int numberPacketsAbandoned=0;
  unsigned int counter=0;

  unsigned long startTime = GetTickCount();//when we started
  communicator->resetNumberOfHILSMotorCommandsReceived();//set the counter to zero
  communicator->resetTotalNumberOfPacketReceived();

  //make sure the delays aren't because we're waiting for info
  communicator->setReceiverThreadSleepTime_ms(1);
  communicator->setHILSTransmitSleepTime_ms(1);

  for(;;)
  {
    communicator->setHILSSensorData(0,0,0,0,0,0,0);

    unsigned long stopTime;
    unsigned long sendTime = GetTickCount();

    communicator->getLastHILSMotorCommands(); //reset the havedata flag

    bool gotPacket;
    //while((communicator->getLastHILSSensorPacketNumber()) != communicator->getMotorCommandsSensorPacketNumber())
    for(;;)
    {//Wait for the right packet to come back
      if((communicator->getLastHILSSensorPacketNumber()) == communicator->getMotorCommandsSensorPacketNumber())
      {
        gotPacket = 1;
        break;
      }

      if((GetTickCount() - sendTime) > timeout)
      {//we've waited too long
        gotPacket = 0;
        break;
      }
    }

    printf("%d\t",counter++);

    if(gotPacket)
    {//we got the right packet
      stopTime = GetTickCount();
      totalTime += stopTime - sendTime;
      numberOfPacketsCameBack++;
  
      printf("%dms\t",stopTime - sendTime);
    }else
    {//we timed out
      stopTime = GetTickCount();
      numberPacketsAbandoned++;
      printf(">[%d]ms\t",stopTime - sendTime);
    }

    if(numberOfPacketsCameBack)
    {
      printf("avg=%.0fms, \t%d lost",((double) totalTime)/((double) numberOfPacketsCameBack),numberPacketsAbandoned);
    }

    printf("\tRx @%.0fcmds/s",((double) (1000*communicator->getNumberOfHILSMotorCommandsReceived())/ (double) (GetTickCount()-startTime))); 
    printf("(%.0f)",((double) (1000*communicator->getTotalNumberOfPacketsReceived())/ (double) (GetTickCount()-startTime))); 
    printf("\n");
    startTime = GetTickCount();
    communicator->resetNumberOfHILSMotorCommandsReceived();
    communicator->resetTotalNumberOfPacketReceived();
  }
}
