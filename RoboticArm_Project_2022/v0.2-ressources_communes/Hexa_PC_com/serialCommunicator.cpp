/* mwm 11.05.2011
 *     01.06.2011: moved thread fns to "threadFunctions.cpp"
 */
// GD last update: 01 /2016

#include "serialCommunicator.h"

/************************************************************************************
 * Class member function definitions                                                *
 ************************************************************************************/
SerialCommunicator::SerialCommunicator(void) {//the constructor
    //port = portFunctions::open_port(USE_BAUD_RATE); //USE_BAUD_RATE is defined in the firmware headers...
    this->message_error = new char[100];
    this->my_port_usb = new port_usb;
    this->my_port_com = new port_com;
    this->port_selected = PORT_COM;
}

SerialCommunicator::~SerialCommunicator(void) {//the destructor
    if(this->port_selected == PORT_USB)
        this->my_port_usb->close();
    else
        this->my_port_com->close();

    this->my_port_usb->~port_usb();
    this->my_port_com->~port_com();

    delete this->message_error;
    delete this->my_port_com;
    delete this->my_port_usb;
}

void SerialCommunicator::port_select(int new_port_selected)
{
    switch(new_port_selected)
    {
        case PORT_USB:
            this->port_selected = PORT_USB;
            break;
        case PORT_COM:
            this->port_selected = PORT_COM;
            break;
        default:
            break;
    }
}

void SerialCommunicator::open_port(int nPort, int nBaud)
{
    if(this->port_selected == PORT_USB)
    {
        this->my_port_usb->open_port(USE_BAUD_RATE);
        if(this->my_port_usb->is_open())
        {
            this->start_communication();
            strcpy(this->message_error, "Port USB: open.\n");
        }
        else
            strcpy(this->message_error, "Port USB: not open.\n");
    }
    else // PORT_COM
    {
        assert(nPort > 0);
        this->my_port_com->open_port(nPort, nBaud);
        if(this->my_port_com->is_open())
        {
            this->start_communication();
            strcpy(this->message_error, "Port COM: open.\n");
        }
        else
            strcpy(this->message_error, "Port COM: not open.\n");
    }
}

void SerialCommunicator::start_communication(void)
{
    receivedPacket.size = 0;
    receiverLocked = transmitterLocked = 0;
    debugDataOuputFile = debugLabelsOuputFile = 0;

    for(int i=0; i<serialComms::NUMBER_DEBUG_DATA; i++)
    {//empty the labels
        debugLabels[i].label[0] = 0;
        //   debugLabels[i].hardware = serialComms::ADDRESS_UNKNOWN;
    }

    // lastReceivedHardwareVersion = serialComms::ADDRESS_UNKNOWN;
    vehicleHILSEnabled = HILS_UNKNOWN_MODE;
    lastReceivedErrorString[0] = 0;
    lastDebugData.timeReceivedMS = 0;
    startTimeTicks = GetTickCount();


    HILSSensorData.sensorData.ArmProp.theta = 0;
    HILSSensorData.sensorData.ArmProp.theta_dot =0;
    HILSSensorData.sensorData.ArmProp.ADC_value =0; //GD 17.01.2016
    HILSSensorData.haveNewData=0;

    /*  HILSSensorData.sensorData.gyroscopes.yaw   = 0;
    HILSSensorData.sensorData.gyroscopes.pitch = 0;
    HILSSensorData.sensorData.gyroscopes.roll  = 0;
    HILSSensorData.sensorData.accelerometer.x = 0;
    HILSSensorData.sensorData.accelerometer.y = 0;
    HILSSensorData.sensorData.accelerometer.z = 0;
    HILSSensorData.sensorData.airPressureSensor = 0;
    HILSSensorData.haveNewData = 0;

    A2DSensorMeanExpected.gyroscopes.yaw    = serialComms::A2D_ZERO_VALUE_GYROSCOPES;
    A2DSensorMeanExpected.gyroscopes.pitch  = serialComms::A2D_ZERO_VALUE_GYROSCOPES;
    A2DSensorMeanExpected.gyroscopes.roll   = serialComms::A2D_ZERO_VALUE_GYROSCOPES;
    A2DSensorMeanExpected.accelerometer.x   = serialComms::A2D_ZERO_VALUE_ACCELEROMETERS;
    A2DSensorMeanExpected.accelerometer.y   = serialComms::A2D_ZERO_VALUE_ACCELEROMETERS;
    A2DSensorMeanExpected.accelerometer.z   = serialComms::A2D_ZERO_VALUE_ACCELEROMETERS;
    A2DSensorMeanExpected.airPressureSensor = serialComms::A2D_ZERO_VALUE_AIRPRESSURE;*/

    //----------------------------------------------------------
    // Motor commands //GD 17.01.2016
    lastHILSMotorCommands.commands.setpoint = 0; //GD 17.01.2016
    lastHILSMotorCommands.haveNewData = 0;

              //initalise the waiting times:
              receiverThreadSleepTime_ms    = serialComms::DEFAULT_RECEIVER_THREAD_SLEEP_TIME_MS;
              transmitterThreadSleepTime_ms = serialComms::DEFAULT_TRANSMITTER_THREAD_SLEEP_TIME_MS;
              resourceLockedSleepTime_ms    = serialComms::DEFAULT_RESOURCE_LOCKED_SLEEP_TIME_MS;
              resourceLockedTimeout_ms          = serialComms::DEFAULT_RESOURCE_LOCKED_TIMEOUT_MS;
              rcCheckSleepTime_ms                = serialComms::DEFAULT_RC_CHECK_SLEEP_TIME_MS;
              debugDataSleepTime_ms         = serialComms::DEFAULT_DEBUG_DATA_SLEEP_TIME_MS;
              HILSTransmitSleepTime_ms      = serialComms::DEFAULT_HILS_TRANSMIT_SLEEP_TIME_MS;
              confirmRequestTimeout_ms      = serialComms::DEFAULT_CONFIRM_REQUEST_TIMEOUT_MS;

    //initialise the counters
    HILSSensorDataCounter = 0;
    HILSMotorCommandsCounter = 0;
    numberOfUnknownPackets = 0;
}

char* SerialCommunicator::getMessage(void)
{
    char* temp = new char[100];
    strcpy(temp, this->message_error);
    return temp;
}

bool SerialCommunicator::portSuccessfullyOpened(void)
{
    if(this->port_selected == PORT_USB)
        return this->my_port_usb->is_open();
    else
        return this->my_port_com->is_open();
}

void SerialCommunicator::close_port(void)
{
    if(this->port_selected == PORT_USB)
        this->my_port_usb->close();
    else
        this->my_port_com->close();
}

bool SerialCommunicator::SerialRx(void) {
  //the function to be called by the receiver thread
  BYTE buf[1024*8];

    unsigned int read;

    static UINT16 crc;
    static UINT8 ptr_rxd_buffer = 0;
    UINT8 crc1, crc2;
    UINT8 c;

    do
    {
        if(this->port_selected == PORT_USB)
            read = this->my_port_usb->ReadAvailable(buf,1000); // catch the received bytes, read= nb of received bytes in th buffer buf
        else
            read = this->my_port_com->ReadAvailable(buf,1000);

        for( unsigned int k =0; k<read; k++)
        {
            c= buf[k];
            if(receiverLocked) return 0; // if rxd buffer is locked immediately return

            // the rxd buffer is unlocked
            if((ptr_rxd_buffer == 0) && (c == '#')) // if rxd buffer is empty and syncronisation character is received
            {
        receivedPacket.data[ptr_rxd_buffer++] = c; // copy 1st byte to buffer
                crc = c; // init crc
            }else if (ptr_rxd_buffer < serialComms::RXD_BUFFER_LEN) // collect incomming bytes
            {
                if(c != '\r') // no termination character
                {
                    receivedPacket.data[ptr_rxd_buffer++] = c; // copy byte to rxd buffer
                    crc += c; // update crc
                }else // termination character was received
                {
                    // the last 2 bytes are not subject for checksum calculation
                    // they are the checksum itself
                    crc -= receivedPacket.data[ptr_rxd_buffer-2];
                    crc -= receivedPacket.data[ptr_rxd_buffer-1];
                    // calculate checksum from transmitted data
                    crc %= 4096;
                    crc1 = (UINT8) ('=' + crc / 64);
                    crc2 = (UINT8) '=' + crc % 64;
                    // compare checksum to transmitted checksum bytes
                    if((crc1 == receivedPacket.data[ptr_rxd_buffer-2]) && (crc2 == receivedPacket.data[ptr_rxd_buffer-1]))
                    {   // checksum valid
                        receivedPacket.data[ptr_rxd_buffer] = '\r'; // set termination character
                        receivedPacket.size = ptr_rxd_buffer + 1;// store number of received bytes
                        receiverLocked = true;          // lock the rxd buffer

                        USART_ProcessRxData();
                    }else
                    {    // checksum invalid
                        receiverLocked = false; // unlock rxd buffer
                    }
                    ptr_rxd_buffer = 0; // reset rxd buffer pointer
                }
            }else // rxd buffer overrun
            {
                ptr_rxd_buffer = 0; // reset rxd buffer
                receiverLocked = false; // unlock rxd buffer
            }
        } // for( unsigned int k =0; k<read; k++)

    }while(read>0); //end of do/while

    return 0;
}

void SerialCommunicator::SendData(UINT8 packet[serialComms::RXD_BUFFER_LEN], UINT8 size) {
  //send a command through the serial port
    UINT8 TX_temp;
    for (unsigned i = 0; i<size; i++)
    {
        TX_temp=packet[i];

        if(this->port_selected == PORT_USB)
            this->my_port_usb->Write(&TX_temp, sizeof(TX_temp));
        else
            this->my_port_com->Write(&TX_temp, sizeof(TX_temp));
    }
}

void SerialCommunicator::addPacket(UINT8 CMD, int Address, UINT8 Data[serialComms::RXD_BUFFER_LEN], UINT8 Length) {
  //add a packet to the queue
  serialComms::TransmissionDataPacket newPacket = makePacket(CMD, Address, Data, Length);

  unsigned int timeWaited = 0;
  while(!tryToLockTransmitter())
  {
    if(timeWaited > resourceLockedTimeout_ms)
      return; //we give up...

    Sleep(resourceLockedSleepTime_ms);
    timeWaited += resourceLockedSleepTime_ms;
  }
  transmissionData.push_back(newPacket);
  setUnlockTransmitter();
}

serialComms::TransmissionDataPacket SerialCommunicator::makePacket(UINT8 CMD, int Address, UINT8 Data[serialComms::RXD_BUFFER_LEN], UINT8 Length) {
  //creates a packet
  serialComms::TransmissionDataPacket newPacket;

    if(CMD != '#')
    {//the packet doesn't contain the leading elements: add them
    UINT8 TX_Data[serialComms::RXD_BUFFER_LEN];
    UINT8 nbEncodedData = ToolBox::Encode64(TX_Data, Data, Length); // result of encoding is contained in TX_Data

        //we add the start, address and ID bytes
    newPacket.data[0] = (UINT8) ('#'); //start
        newPacket.data[1] = (UINT8) ('a'+ Address); //address
        newPacket.data[2] = CMD; //ID

        for (unsigned int i = 0; i<nbEncodedData; i++)
        {
            newPacket.data[3+i] = TX_Data[i];
        }
    ToolBox::AddCRC(newPacket.data, 3 + nbEncodedData);//adds CRC, and '/r' character
    newPacket.size = nbEncodedData + 3 + 3; //the three characters added by the CRC, three leading bytes
    }else
    {//the packet already starts with '#' -- send as-is
    newPacket.size = Length;
    memcpy(newPacket.data,Data,serialComms::RXD_BUFFER_LEN);//copies unenconded data into incomingData
    }

  return newPacket;
}


void SerialCommunicator::requestA2DSensorValues(void) {
    addPacket('<',serialComms::ADDRESS_FC,0,0);
}

void SerialCommunicator::requestDebugData(UINT8 updateRate) {
    UINT8 MyData[1];
    MyData[0]= updateRate; //fast 10; //Sets speed of debug data
    addPacket('d',serialComms::ADDRESS_ALL,MyData,1);
}

void SerialCommunicator::requestDebugLabel(UINT8 labelNumber) {
  //request only one label
    UINT8 MyData[1];
    MyData[0]= labelNumber; //the number of the label we want
    addPacket('a',serialComms::ADDRESS_ALL,MyData,1);
}

void SerialCommunicator::requestDebugLabelsAll(void) {
  //request all labels
  for(UINT8 i=0; i<serialComms::NUMBER_DEBUG_DATA; i++)
  {
    requestDebugLabel(i);
  }
}

//void SerialCommunicator::requestSerialLinkTest(void) {
//  //sends a packet to be echoed by the vehicle
//  UINT8 sendData[2];
//  sendData[0] = serialComms::SERIAL_LINK_TEST_CHAR_0;
//  sendData[1] = serialComms::SERIAL_LINK_TEST_CHAR_1;
//
//  addPacket('z',serialComms::ADDRESS_NC,sendData,2);
//}
//
//void SerialCommunicator::requestEngineTest(UINT8 engineTestSpeed) {
//  //sends a packet to be echoed by the vehicle
//  UINT8 sendData[16];
//    for(int i=0; i<16; i++)
//    {
//        if(i < 6)
//        {//we have 6 motors
//            sendData[i]=engineTestSpeed;
//        }else
//        {
//            sendData[i]=0;
//        }
//    }
//
//  addPacket('t',serialComms::ADDRESS_FC,sendData,16);
//}
//
//void SerialCommunicator::requestChangePrintInfoStrings(bool newValue) {
//  //toggles whether the vehicle should transmit the (self-generated) info strigns.
//  //This sends data to the FC address, containing one bit of data: whether to send the info strings or not.
//  UINT8 sendData[1];
//  sendData[0]=newValue;
//  addPacket('i',serialComms::ADDRESS_FC,sendData,1);
//}
//
//void SerialCommunicator::requestPPMChannels(void) {
//  addPacket('p',serialComms::ADDRESS_FC,0,0);
//}

void SerialCommunicator::requestChangeHILSState(enum TypeHILS newValue) {
  //ask the vehicle to set the HILS state
  UINT8 MyData[1];
  MyData[0] = (unsigned char) newValue;
  addPacket('(',serialComms::ADDRESS_FC,MyData,1);
}

void SerialCommunicator::requestHILSState(void) {
  addPacket('[',serialComms::ADDRESS_FC,0,0);
}

void SerialCommunicator::USART_ProcessRxData(void) {
  if(!receiverLocked)
  {
    return;// if data in the rxd buffer are not locked immediately return
  }

  receivedPacketsCounter++;

    UINT8 dataAddress = receivedPacket.data[1]-'a';

  bool haveInterpretedData = interpretCommonData();

    if(!haveInterpretedData)
    {
        switch(dataAddress)
        {
        case serialComms::ADDRESS_FC:
            //addressString = "FC";
            haveInterpretedData = interpretDataFromFC();
            break;
        case serialComms::ADDRESS_NC:
            //addressString = "NC";
            haveInterpretedData = interpretDataFromNC();
            break;
        case serialComms::ADDRESS_MK3MAG:
            //addressString = "MK3MAG";
            haveInterpretedData = 0;
            break;
        case serialComms::ADDRESS_ALL:
            //addressString = "ALL";
            haveInterpretedData = 0;
            break;
        default:  //i.e. not ADDRESS_FC
            {
                std::string tempStr = "OTHER[X]";
                tempStr[6] = '0'+dataAddress;
                //addressString = tempStr;
            }
            haveInterpretedData = 0;
            break;
        }
  }

  if(!haveInterpretedData)
  {//we don't know what this packet was for...
    numberOfUnknownPackets++;
  }

  receiverLocked = false;
}

bool SerialCommunicator::interpretDataFromFC(void) {
  //we've got a packet with FC address byte

    UINT8 decodedData[serialComms::RXD_BUFFER_LEN];
                 //Decode64(decodedDataOut,encodedDataIn,ReceivedBytes, UINT8 &mysteryOut1, UINT8 &mysteryOut1);
    /*UINT8 numberOfDecodedBytes = */ToolBox::Decode64(decodedData, receivedPacket.data, receivedPacket.size); // decode data block in rxd_buffer
    switch(receivedPacket.data[2])
    {
    case 'k':
        //dataTypeString = "Compass heading";
        //messageString = "";
        break;
    case 'T':
        //dataTypeString = "Engine Test";
        //messageString = "";
        break;
    case 'Q':
        //dataTypeString = "Settings";
        //messageString = "";
        break;
    case 'S':
        //dataTypeString = "Write settings";
        //messageString = "";
        break;
    case 'P':
        //dataTypeString = "Read PPM channels";
        //dataTypeString = "";
        //messageString = "";
        memcpy((unsigned char *)&lastRCState.channels, (unsigned char *)&decodedData, sizeof(lastRCState.channels));
        lastRCState.timeReceivedMS = GetTickCount();
        break;
    case 'C':
        //dataTypeString = "Set 3D data interval";
        //messageString = "";
        break;
    case 'N':
        //dataTypeString = "Mixer request";
        //messageString = "";
        break;
    case 'M':
        //dataTypeString = "Mixer write";
        //messageString = "";
        break;
    case 'U':
        //dataTypeString = "BL Parameter request";
        //messageString = "";
        break;
    case 'W':
        //dataTypeString = "BL Parameter write";
        //messageString = "";
        break;
    default:
        //unhandled data type
        //dataTypeString = "UNKOWN";
        return 0; //unsuccessful decoding
    }
    return 1;
}

bool SerialCommunicator::interpretDataFromNC(void) {
  //received data from Navi Ctrl
    //decode the input data:
    UINT8 decodedData[serialComms::RXD_BUFFER_LEN];
                 //Decode64(decodedDataOut,encodedDataIn,ReceivedBytes, UINT8 &mysteryOut1, UINT8 &mysteryOut1);
    UINT8 numberOfDecodedBytes = ToolBox::Decode64(decodedData, receivedPacket.data, receivedPacket.size); // decode data block in rxd_buffer

    UINT8 idByte = receivedPacket.data[2];
    switch(idByte)
    {
    case 'Z':
        //dataTypeString = "Serial link test";
    if( (decodedData[0] == serialComms::SERIAL_LINK_TEST_CHAR_0) && (decodedData[1] == serialComms::SERIAL_LINK_TEST_CHAR_1))
    {
            serialLinkTestPassed = 1;
    }
        break;

    case 'E':
        //dataTypeString = "Error messageString string";
    {
          int i=0;
          while(decodedData[i])
          {
              if(i>=(numberOfDecodedBytes-1))
                  break; //this prevents access violations, but shouldn't be necessary

              lastReceivedErrorString[i]=decodedData[i];
              i++;
          }
        lastReceivedErrorString[i]=0;
    }
        break;

    case 'W':
        //dataTypeString = "Number of waypoints (W)";
        break;

    case 'X':
        //dataTypeString = "Number of waypoints (X)";
        break;

    case 'C':
        //dataTypeString = "3D data interval";
        break;

    case 'O':
        //dataTypeString = "OSD Data";
        break;

    default://unkown data
        //dataTypeString = idByte;
        return 0;
    }
    return 1;
}

bool SerialCommunicator::interpretCommonData(void) {
  /* decodes data that is not received from a specific address
    * returns 1 if the data is common, zero otherwise;
    */

  UINT8 decodedData[serialComms::RXD_BUFFER_LEN];
         //Decode64(decodedDataOut,encodedDataIn,ReceivedBytes, UINT8 &mysteryOut1, UINT8 &mysteryOut1);
  UINT8 numberOfDecodedBytes = ToolBox::Decode64(decodedData, receivedPacket.data, receivedPacket.size); // decode data block in rxd_buffer

    UINT8 idByte = receivedPacket.data[2];

    switch(idByte)
    {
    case 'A':
      //dataTypeString = "Debug labels";

        //assert(numberOfDecodedBytes == 16); //the labels should be this many bytes long

        //we check that the label number makes sense:
        assert(decodedData[0]>=0);
        assert(decodedData[0]<serialComms::NUMBER_DEBUG_DATA);

        //write it to the relevant debug label
        debugLabels[decodedData[0]].hardware = lastReceivedHardwareVersion;
      for (unsigned int k = 1; k<numberOfDecodedBytes; k++)
      {// must start 1 later =>k=1. the first value is the index of the label
      debugLabels[decodedData[0]].label[k-1] = (char) decodedData[k];
      }
    debugLabels[decodedData[0]].label[serialComms::DEBUG_LABEL_LENGTH-1] = 0;//terminate the string

        if(debugLabelsOuputFile)
        {//Check whether to write to the outfile
            bool haveAllLabels = 1;
            int i = 0;
            while( (i<serialComms::NUMBER_DEBUG_DATA) && (haveAllLabels))
            {
                haveAllLabels = debugLabels[i].hardware == lastReceivedHardwareVersion;
                i++;
            }
            if(haveAllLabels)
            {
                switch(lastReceivedHardwareVersion)
                {
                case serialComms::ADDRESS_FC:
          fprintf(debugLabelsOuputFile,"%s","FC\n");
                    break;
        case serialComms::ADDRESS_MK3MAG:
          fprintf(debugLabelsOuputFile,"%s","MK3MAG\n");
                    break;
                case serialComms::ADDRESS_NC:
          fprintf(debugLabelsOuputFile,"%s","NC\n");
                    break;
                default:
          fprintf(debugLabelsOuputFile,"%s","UNKNOWN\n");
                    break;
                }
                for(i=0; i<serialComms::NUMBER_DEBUG_DATA; i++)
                {
          fprintf(debugLabelsOuputFile,"%s",debugLabels[i].label);
          fprintf(debugLabelsOuputFile,"%s","\n");
                }
                //we're done writing to this file, clear pointer
                debugLabelsOuputFile = 0;
            }
        }
    break;

    case 'B':
        //dataTypeString = "ExternControl";
        //do something with this
        break;

    case 'H':
        //dataTypeString = "Request display (H)";
        //do something with this
        break;

    case 'L':
        //dataTypeString = "Request display (L)";
        //do something with this
        break;

    case 'V':
        //dataTypeString = "Version request";
        //need to copy the string of characters into a version structure:
    switch(receivedPacket.data[1]-'a')
    {//cases come from trial & error
    case 1:
            lastReceivedHardwareVersion = serialComms::ADDRESS_FC;
      break;
    case 2:
            lastReceivedHardwareVersion = serialComms::ADDRESS_NC;
      break;
    case 3:
            lastReceivedHardwareVersion = serialComms::ADDRESS_MK3MAG;
      break;
    default:
            lastReceivedHardwareVersion = serialComms::ADDRESS_UNKNOWN;
    }
        break;

    case 'D': //got debug data
        //dataTypeString = "Debug data";
        //this line from QMK Groundstation, "wgt_Connecgtion.cpp", ln190
    memcpy((unsigned char *)&lastDebugData.data, (unsigned char *)&decodedData, sizeof(lastDebugData));
        lastDebugData.timeReceivedMS = GetTickCount(); //timestamp
        if(debugDataOuputFile)
        {//we should write to the file
      fprintf(debugDataOuputFile,"%f",((float) lastDebugData.timeReceivedMS)/1000.);
      fprintf(debugDataOuputFile,"%s",",");
            for(int i=0; i<serialComms::NUMBER_DEBUG_DATA; i++)
            {
        fprintf(debugDataOuputFile,"%d",lastDebugData.data.Analog[i]);
        fprintf(debugDataOuputFile,"%s",",");
            }
            fprintf(debugDataOuputFile,"\n");
        }
        break;

    case 'G':
        //dataTypeString = "ExternControl struct";
        //do something with this
        break;

  case 'I':
    //this is my own debug string. Just print to the screent:
    {
      char debugStringReceived[80];
          int i=0;
          while(decodedData[i])
          {
              if(i>=(numberOfDecodedBytes-1))
                  break; //this prevents access violations, but shouldn't be necessary

              debugStringReceived[i]=decodedData[i];
              i++;
          }
        debugStringReceived[i]=0;
      printf("Got debug string at t=%.3fsec: \"",(double) (GetTickCount() - startTimeTicks)/1000.);
      printf(debugStringReceived);
      printf("\"\n");
    }
        break;

  case ']':
    //this is the HILS state (enabled/disabled)
    vehicleHILSEnabled = (enum TypeHILS) decodedData[0];
    break;

  case '}':
    //this is the HILS motor commands set
    memcpy((unsigned char *)&lastHILSMotorCommands.commands, (unsigned char *)&decodedData, sizeof(lastHILSMotorCommands.commands));
    lastHILSMotorCommands.haveNewData = 1;
        lastHILSMotorCommands.timeReceivedMS = GetTickCount(); //timestamp
    HILSMotorCommandsCounter++;
    break;

  case '>':
    //We have received A2D data
    memcpy((unsigned char *)&A2DSensorDataIn.sensorData, (unsigned char *)&decodedData, sizeof(A2DSensorDataIn.sensorData));
    A2DSensorDataIn.haveNewData = 1;
    break;

    default:
        //the command is not a "Common command" -- we'll have to decode it in the context of the address
        return 0;
    }
  return 1; //have succeeded in decoding the data
}

volatile bool SerialCommunicator::tryToLockTransmitter(void) {
  if(transmitterLocked)
  {
    return 0;
  }
  transmitterLocked = 1;
  return 1;
}

void SerialCommunicator::sendLastPacket(void) {
  //sends the last packet, and removes it from the queue
  if(!transmissionData.size())
  {//the queue is empty
    return;
  }

  SendData(transmissionData[transmissionData.size()-1]);
  transmissionData.pop_back();
}

void SerialCommunicator::sendRCStateRequestNow(void) {
  //immediately send a request for the PPM channels
  SendData(makePacket('p',serialComms::ADDRESS_FC,0,0));
}

void SerialCommunicator::sendDebugDataRequestNow(UINT8 updateRate) {
  //immediately send a request for the PPM channels
    UINT8 MyData[1];
    MyData[0]= updateRate;
    SendData(makePacket('d',serialComms::ADDRESS_ALL,MyData,1));
}

void SerialCommunicator::sendStopRequestNow() {
  //immediately send a request for the PPM channels
    UINT8 MyData[1];
    MyData[0]= 0;
    SendData(makePacket('x',serialComms::ADDRESS_ALL,MyData,1));
}

void SerialCommunicator::sendHILSDataNow(void) {
  //immediately send the HILS data
    UINT8 MyData[serialComms::RXD_BUFFER_LEN];
  memcpy(MyData, (unsigned char *)&HILSSensorData.sensorData, sizeof(HILSSensorData.sensorData));

  HILSSensorData.haveNewData = 0;
  SendData(makePacket('{', serialComms::ADDRESS_FC, MyData, sizeof(HILSSensorData.sensorData)));
}


bool SerialCommunicator::requestConfirmedDebugLabel(UINT8 labelNumber) {
  //update a specific label
    unsigned int totalTimeWaited = 0;

  //clear the label
  debugLabels[labelNumber].label[0] = 0;

    for(;;)
    {
        if(totalTimeWaited > confirmRequestTimeout_ms)
        {//we've waited to long for this request, call it off
            return 0;
        }

    requestDebugLabel(labelNumber);

    if(debugLabels[labelNumber].label[0])
    {//the label now contains text
      return 1;
    }

        Sleep(transmitterThreadSleepTime_ms);
        totalTimeWaited += transmitterThreadSleepTime_ms;
    }
}


void SerialCommunicator::getDebugLabel(char outLabelText[serialComms::DEBUG_LABEL_LENGTH], int labelNum) {
  assert((labelNum>=0) && (labelNum < serialComms::NUMBER_DEBUG_DATA)); //make sure the label exists
  for(int characterNum=0; characterNum<serialComms::DEBUG_LABEL_LENGTH; characterNum++)
  {
    outLabelText[characterNum] = debugLabels[labelNum].label[characterNum];

  }
}

void SerialCommunicator::getErrorString(char outText[serialComms::RXD_BUFFER_LEN]) {
  for(int characterNum=0; characterNum<serialComms::RXD_BUFFER_LEN; characterNum++)
  {
    outText[characterNum] = lastReceivedErrorString[characterNum];
        if(!lastReceivedErrorString[characterNum])
        {//end of the string
            break;
        }
  }
}



bool SerialCommunicator::requestConfirmedGetHILSState(void) {
  //get the vehicle's onboard HILS state
    unsigned int totalTimeWaited = 0;

  //clear the HILS Status
  vehicleHILSEnabled = HILS_UNKNOWN_MODE;

    for(;;)
    {
        if(totalTimeWaited > confirmRequestTimeout_ms)
        {//we've waited to long for this request, call it off
            return 0;
        }

        requestHILSState();

        if(vehicleHILSEnabled!=HILS_UNKNOWN_MODE)
          return 1; //value has changed

        Sleep(transmitterThreadSleepTime_ms);
        totalTimeWaited += transmitterThreadSleepTime_ms;
    }
}

bool SerialCommunicator::requestConfirmedChangeHILSState(enum TypeHILS newValue) {
  //set the vehicle's onboard HILS state
    unsigned int totalTimeWaited = 0;

  //clear the HILS Status
  vehicleHILSEnabled = HILS_UNKNOWN_MODE;

    for(;;)
    {
        if(totalTimeWaited > confirmRequestTimeout_ms)
        {//we've waited to long for this request, call it off
            return 0;
        }

    requestChangeHILSState(newValue);
    requestHILSState();

    if(vehicleHILSEnabled == newValue)
    {//value has changed to what we wanted
      return 1;
    }

        Sleep(transmitterThreadSleepTime_ms);
        totalTimeWaited += transmitterThreadSleepTime_ms;
    }
}

bool SerialCommunicator::requestConfirmedA2DValues(void) {
  //Read the on-board sensors A2D values
    unsigned int totalTimeWaited = 0;

  //clear the have data status
  A2DSensorDataIn.haveNewData = 0;

    for(;;)
    {
        if(totalTimeWaited > confirmRequestTimeout_ms)
        {//we've waited to long for this request, call it off
            return 0;
        }

    requestA2DSensorValues();

    if(A2DSensorDataIn.haveNewData)
    {//got new sensordata
      return 1;
    }

        Sleep(transmitterThreadSleepTime_ms);
        totalTimeWaited += transmitterThreadSleepTime_ms;
    }
}

COMMS_INT saturate10Bits(int inValue){//saturates a value to the (positive range) of a 10bit int (i.e. what we expect from the A2D)
  if(inValue < 0)
    return 0;

  if(inValue > 1023)
    return 1023;

  return COMMS_INT(inValue); //cast does nothing...
}

/*void SerialCommunicator::setHILSSensorData(int gyroRoll, int gyroPitch, int gyroYaw, int accX, int accY, int accZ, int airPressure) {
  //polulate the HILS sensor info
  HILSSensorData.sensorData.gyroscopes.yaw   = saturate10Bits(gyroYaw   + (A2DSensorMeanMeasured.gyroscopes.yaw   - A2DSensorMeanExpected.gyroscopes.yaw));
  HILSSensorData.sensorData.gyroscopes.pitch = saturate10Bits(gyroPitch + (A2DSensorMeanMeasured.gyroscopes.pitch - A2DSensorMeanExpected.gyroscopes.pitch));
  HILSSensorData.sensorData.gyroscopes.roll  = saturate10Bits(gyroRoll  + (A2DSensorMeanMeasured.gyroscopes.roll  - A2DSensorMeanExpected.gyroscopes.roll));

  HILSSensorData.sensorData.accelerometer.x  = saturate10Bits(accX + (A2DSensorMeanMeasured.accelerometer.x - A2DSensorMeanExpected.accelerometer.x));
  HILSSensorData.sensorData.accelerometer.y  = saturate10Bits(accY + (A2DSensorMeanMeasured.accelerometer.y - A2DSensorMeanExpected.accelerometer.y));
  HILSSensorData.sensorData.accelerometer.z  = saturate10Bits(accZ + (A2DSensorMeanMeasured.accelerometer.z - A2DSensorMeanExpected.accelerometer.z));

  HILSSensorData.sensorData.airPressureSensor = saturate10Bits(airPressure + (A2DSensorMeanMeasured.airPressureSensor - A2DSensorMeanExpected.airPressureSensor));

  HILSSensorDataCounter = (HILSSensorDataCounter+1)%256;
  HILSSensorData.sensorData.id = (COMMS_UNSIGNED_CHAR) HILSSensorDataCounter;//an internal counter

  HILSSensorData.haveNewData = 1;
}*/

// -----------------------------------------------------------------------------------------------
// GD: 04.05. 2013,  specific to arm prop
void SerialCommunicator::setHILSSensorData(int theta, int theta_dot, int ADC_value)
{
  HILSSensorData.sensorData.ArmProp.theta = short(theta);
  HILSSensorData.sensorData.ArmProp.theta_dot = short(theta_dot);
  HILSSensorData.sensorData.ArmProp.ADC_value = short(ADC_value);

  HILSSensorDataCounter = (HILSSensorDataCounter+1)%256;
  HILSSensorData.sensorData.id = COMMS_UNSIGNED_CHAR(HILSSensorDataCounter);//an internal counter

  HILSSensorData.haveNewData = 1; // this flag will be checked DWORD WINAPI communicationThread::HILSSensorDataTransmitter(LPVOID lpParam)
}


bool SerialCommunicator::switchIntoHILSMode(enum TypeHILS mode, bool printDiagnosticInfo)
{
  //switch the vehicle into HILS mode

 /* if(!requestConfirmedChangeHILSState(HILS_REAL_FLIGHT))
  {
    if(printDiagnosticInfo)
      printf("FAILED\n");

    Sleep(2*confirmRequestTimeout_ms);
    if(printDiagnosticInfo)
      printf("Try again... ");
    if(!requestConfirmedChangeHILSState(HILS_REAL_FLIGHT))
    {//give up
      if(printDiagnosticInfo)
        printf("FAILED\n");

      return 1;
    }
    Sleep(2*confirmRequestTimeout_ms);
  }
  if(printDiagnosticInfo)
    printf("OK\n");



  if(printDiagnosticInfo)
    printf(" OK\n");*/

    if(printDiagnosticInfo)
        printf("Enabling HILS mode... ");

    if(!this->requestConfirmedChangeHILSState(mode))
    {//can't enable HILS
        if(printDiagnosticInfo)
            printf("FAILED\n");

        Sleep(3*confirmRequestTimeout_ms);

        if(printDiagnosticInfo)
            printf("Try again... ");

        if(!this->requestConfirmedChangeHILSState(mode))
        {//give up
            if(printDiagnosticInfo)
                printf("FAILED\n");
        }
        else if(printDiagnosticInfo)
            printf("OK\n");
    }
    else if(printDiagnosticInfo)
        printf("OK\n");

    Sleep(2*confirmRequestTimeout_ms);

    if(printDiagnosticInfo)
        printf("Checking HILS mode... ");

    if(!this->requestConfirmedGetHILSState())
    {
        if(printDiagnosticInfo)
            printf("FAILED\n");

        return 1;
    }

    if(printDiagnosticInfo)
        printf("OK\n");

    if(!this->getLastHILSState())
    {
        if(printDiagnosticInfo)
            printf("NOT in HILS mode. \n");

        return 1;
    }

    return 0;
}
