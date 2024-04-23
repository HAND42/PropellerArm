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
//      14.06.2011 Print motor commands
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
  printf(" Printing motor commands \n");
  printf("-----------------------------------------------------\n\n");

  CreateThread(0,0,communicationThread::receiver,communicator,0,0); //create the receiver thread
  CreateThread(0,0,communicationThread::generalTransmitter,communicator,0,0); //create the transmitter thread
  CreateThread(0,0,communicationThread::HILSSensorDataTransmitter,communicator,0,0); //create the HILS transmitter thread

  serialComms::HILSMotorCommands hilsMotorCommands;

  printf("Switching into HILS mode:\n");
  if(communicator->switchIntoHILSMode(1))
  {
    printf("Failed to switch into HILS mode - exiting...\n");
    return 1;
  }
  printf("Successfully swtiched into HILS mode.\n");
  Sleep(1000);

  unsigned long lastCommandsTime = GetTickCount();

  for(;;)
  {//just print the motor speeds:
    communicator->setHILSSensorData(serialComms::A2D_ZERO_VALUE_GYROSCOPES,\
                                    serialComms::A2D_ZERO_VALUE_GYROSCOPES,\
                                    serialComms::A2D_ZERO_VALUE_GYROSCOPES,\
                                    serialComms::A2D_ZERO_VALUE_ACCELEROMETERS,\
                                    serialComms::A2D_ZERO_VALUE_ACCELEROMETERS,\
                                    serialComms::A2D_ZERO_VALUE_ACCELEROMETERS,\
                                    serialComms::A2D_ZERO_VALUE_AIRPRESSURE);

    if(!communicator->getHaveNewMotorData()){
      continue;
    }

    hilsMotorCommands = communicator->getLastHILSMotorCommands();

    int age = GetTickCount() - lastCommandsTime;
    lastCommandsTime = hilsMotorCommands.timeReceivedMS;
    printf("%.3fs:\t",((double)age)*0.001);
    for(int i=0; i<6; i++)
    {
      printf("%d\t",hilsMotorCommands.commands.setPoint[i]);
    };
    printf("\n");
  }
  delete communicator;
  return 0;
}
