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

  CreateThread(0,0,communicationThread::receiver,communicator,0,0); //create the receiver thread
  CreateThread(0,0,communicationThread::generalTransmitter,communicator,0,0); //create the transmitter thread
  CreateThread(0,0,communicationThread::rcRemoteRequestTransmitter,communicator,0,0); //create the RC transmitter thread

  serialComms::RCState rcState;

  printf("Switching to FC... ");
  //switch to FC:
  communicator->requestConfirmedRedirectUART_FC();
  printf("OK\n");

  for(;;)
  {//just print the RC state:
    rcState = communicator->getLastRCState();
    int age = GetTickCount() - rcState.timeReceivedMS;
    printf("%.3fs:\t",((double)age)*0.001);
    for(int i=0; i<8; i++)
    {
      printf("%d\t",rcState.channels.Data[i]);
    };
    printf("\n");
    Sleep(10);
  }
}
