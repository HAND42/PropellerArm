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

  bool askForInfoStrings = 0;

  /////////////////////////////////////////////////////////////////////////
  //Set up KB handler
  /////////////////////////////////////////////////////////////////////////
  //keyboard handling from http://www.cplusplus.com/forum/windows/6632/
  DWORD        mode;          // Preserved console mode 
  INPUT_RECORD event;         // Input event 
  BOOL         done = FALSE;  // Program termination flag 
  // Get the console input handle 
  HANDLE hstdin = GetStdHandle( STD_INPUT_HANDLE );
  // Preserve the original console mode 
  GetConsoleMode( hstdin, &mode );
  // Set to no line-buffering, no echo, no special-key-processing 
  SetConsoleMode( hstdin, 0 );
  printf("Press <Esc> to quit.\n\n");

  while (!done)
  {
    if (WaitForSingleObject( hstdin, 0 ) == WAIT_OBJECT_0)  // if kb hit 
    {
      DWORD count;  // ignored
      // Get the input event
      ReadConsoleInput( hstdin, &event, 1, &count );
      // Only respond to key release events 
      if ((event.EventType == KEY_EVENT) &&  !event.Event.KeyEvent.bKeyDown)
      {
        switch (event.Event.KeyEvent.wVirtualKeyCode)
        {//key definitions: see http://delphi.about.com/od/objectpascalide/l/blvkc.htm
        case VK_ESCAPE:
          done = 1;
          printf("<Esc> hit - exiting. \n\n");
          break;
        case 0x31:
          printf("<1>: Changing mode to FC: ");
          reportSuccess(communicator->requestConfirmedRedirectUART_FC());
          break;
        case 0x32:
          printf("<2>: Changing mode to MK3MAG: ");
          reportSuccess(communicator->requestConfirmedRedirectUART_MK3MAG());
          break;
        case 0x34:
          printf("<4>: Changing mode to NC: ");
          reportSuccess(communicator->requestConfirmedRedirectUART_NC());
          break;
        case 0x44: 
          printf("<D>: Sending debug data request\n");
          communicator->requestDebugData();
          break;
        case 0x41:
          printf("<A>: Sending debug label request: ");
          reportSuccess(communicator->requestConfirmedDebugLabelsAll());
          for(UINT8 i=0; i<serialComms::NUMBER_DEBUG_DATA; i++)
          {
            char labelText[serialComms::DEBUG_LABEL_LENGTH];
            communicator->getDebugLabel(labelText, i);
            printf("#%d: \t", i);
            printf(labelText);
            printf("\n");
          }
          break;
        case 0x57:
          printf("<W>: Printing last debug values: (%.2fsec old)\n",(double) (GetTickCount() - communicator->getLastDebugDataTimeReceived())/1000.);
          for(UINT8 i=0; i<serialComms::NUMBER_DEBUG_DATA; i++)
          {
            char labelText[serialComms::DEBUG_LABEL_LENGTH];
            communicator->getDebugLabel(labelText, i);
            printf("#%d: \t", i);
            printf(labelText);
            printf("\t = %d", communicator->getLastDebugDataAnalog(i));
            printf("\n");
          }
          break;
        case 0x5A:
          printf("<Z>: Serial link test request: ");
          reportSuccess(communicator->requestConfirmedSerialLinkTest());
          break;
        case 0x56:
          printf("<V>: Requesting version info: ");
          if(communicator->requestConfirmedVersionInfo())
          {
            switch(communicator->getLastVersionInfo())
            {
            case serialComms::ADDRESS_FC:
              printf("FC\n");
              break;
            case serialComms::ADDRESS_NC:
              printf("NC\n");
              break;
            case serialComms::ADDRESS_MK3MAG:
              printf("MK3MAG\n");
              break;
            default:
              printf("UNKNOWN!\n");
            }
          }else
          {
            printf("FAILED\n");
          }
          break;
        case 0x45:
          printf("<E>: Requesting NC error string: ");
          communicator->requestErrorText();
          if(reportSuccess(communicator->requestConfirmedErrorString()))
          {
            printf("Error: `");
            char errorText[serialComms::RXD_BUFFER_LEN];
            communicator->getErrorString(errorText);
            printf(errorText);
            printf("'\n");
          }
          break;
        //case 0x47:
          //printf("<G>: Requesting external control struct\n");
          //communicator->requestExternalControl();
          //break;
        case 0x54: 
          printf("<T>: Engine test\n");
          communicator->requestEngineTest(10);
          break;
        case 0x50: 
          printf("<P>: Request PPM channels\n");
          communicator->requestPPMChannels();
          break;
        case 0x55: 
          printf("<U>: Number of unkown packets received = %d\n",communicator->getNumberOfUnknownPackets());
          break;
        case 0x4D: //M
          {
            serialComms::DebugData lastDebugData = communicator->getLastDebugData();;
            printf("<M>: Print debug data (age = %.2fseconds) \n",lastDebugData.timeReceivedMS*0.001);

            for(UINT8 i=0; i<serialComms::NUMBER_DEBUG_DATA; i++)
            {
              char labelText[serialComms::DEBUG_LABEL_LENGTH];
              communicator->getDebugLabel(labelText, i);
              printf("#%d: \t", i);
              printf(labelText);
              printf(" = %d \n",lastDebugData.data.Analog[i]);
            }

          break;
          }

        case 0x53: 
          printf("<S>: Toggle printing of info strings, setting to: %d\n",askForInfoStrings =!askForInfoStrings);
          communicator->requestChangePrintInfoStrings(askForInfoStrings);
          break;

        case 0x46: 
          printf("<F>: Get current HILS state: ");
          if(reportSuccess(communicator->requestConfirmedGetHILSState()))
          {
            printf("      On-board HILS state = %d\n",communicator->getLastHILSState());
          }
          break;

        case 0x48: 
          printf("<H>: Activate HILS mode (real HILS = %d) ",HILS_PROPER_HILS);
          reportSuccess(communicator->requestConfirmedChangeHILSState(HILS_PROPER_HILS));
          break;

        case 0x4A: 
          printf("<J>: Activate HILS mode (``HILS'' HILS = %d) ",HILS_EQUAL_MOTOR_COMMANDS);//all motor commands equal, ""COMMS IN THE LOOP"" 
          reportSuccess(communicator->requestConfirmedChangeHILSState(HILS_EQUAL_MOTOR_COMMANDS));
          break;

        case 0x4B: 
          printf("<K>: Activate HILS mode (real HILS, with motors = %d) ",HILS_PROPER_HILS_WITH_MOTORS_ON);//all motor commands equal, ""COMMS IN THE LOOP"" 
          reportSuccess(communicator->requestConfirmedChangeHILSState(HILS_PROPER_HILS_WITH_MOTORS_ON));
          break;

        case 0x47: 
          printf("<G>: Deactivate HILS mode (HILS = %d) ",HILS_REAL_FLIGHT);
          reportSuccess(communicator->requestConfirmedChangeHILSState(HILS_REAL_FLIGHT));
          break;

        case 0x58:
          printf("<X>: Get A2D values: ");
          if(reportSuccess(communicator->requestConfirmedA2DValues())){
            struct SensorPack A2DData = communicator->getLastA2DData();
            printf("Gyro x = %d\n",A2DData.gyroscopes.roll);
            printf("     y = %d\n",A2DData.gyroscopes.pitch);
            printf("     z = %d\n",A2DData.gyroscopes.yaw);
            printf("Acc x = %d\n", A2DData.accelerometer.x);
            printf("    y = %d\n", A2DData.accelerometer.y);
            printf("    z = %d\n", A2DData.accelerometer.z);
            printf("Airpressure = %d\n",A2DData.airPressureSensor);
          }
          break;
        }
      }
    }
  }

  SetConsoleMode( hstdin, mode );
  return 0;
}
