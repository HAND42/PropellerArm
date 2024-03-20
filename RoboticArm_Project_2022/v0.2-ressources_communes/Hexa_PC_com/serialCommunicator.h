/* mwm 11.05.2011
 *
 * An object which does serial communication with the hexacopter. The
 * idea being that one can simply put this into other code, and use it
 * there (e.g. in a simulator to read the RC channels, or for HILS).
 *
 * --HOW TO USE:--------------------------------------------------------
 *  > To use this in a separate project, you must include this header file,
 *    i.e.
 *     #include "PATH_TO_FILES/serialCommunicator.h"
 *
 *  > You should include the project "SerialComms.vcproj" in your project
 *
 *  > Set the dependencies: under "Project Dependecies" check "Serial Communication"
 *    for your project.
 *
 *  > In your code, instantiate the object with something like
 *     SerialCommunicator* communicator = new SerialCommunicator();
 *    You can test whether a port was opened using by evaluating
 *     communicator->portSuccessfullyOpened()
 *    which returns zero if it failed.
 *
 *  > The SerialCommunicator runs on multiple threads, which you must create.
 *    This is always done by using the following syntax:
 *     CreateThread(0,0,THREAD_NAME,POINTER_TO_SC,0,0);
 *    where THREAD_NAME is the name of the function to be used, one of the
 *    functions in the namespace "communicationThread". POINTER_TO_SC is the
 *    pointer to the serialCommunicator you have instantiated.
 *    See the function declarations for details on what the threads do.
 *    Note that the threads use various times to Sleep(.), and that the user
 *    can overwrite these files using the appropriate setXX(.) functions. This
 *    must be done *BEFORE* the threads are initialised, otherwise it will have
 *    no effect. The threads are defined in "threadFunction.cpp"
 *
 *  > In your own code, note the difference between "requestXX" functions,
 *    and "requestConfirmedXX": requestXX only adds a data packet to the queue
 *    and then immediately exits. The command is sent by the transmission thread.
 *    RequestConfirmedXX does not exit until a command has been completed, and
 *    reports success (true) or failure (false). It fails at timeout (i.e. if it
 *    doesn't succeed in some time, it returns false).
 *
 *  > The "sendXXRequestNow()" functions are intended for use *only* by
 *    the appropriate threads.
 *
 */
#ifndef SERIALCOMMUNICATOR_H
#define SERIALCOMMUNICATOR_H

#define PORT_USB 0
#define PORT_COM 1

#include "port_usb_functions.h"
#include "port_com_functions.h"
#include "communicationDefines.h"
#include "ToolBox.h"
#include "../Firmware/FC_0.78f/CommonCommsDefines.h" //for USE_BAUD_RATE

/************************************************************************************
 * Thread functions                                                                 *
 ************************************************************************************/
namespace communicationThread {
  DWORD WINAPI receiver(LPVOID lpParam);
  DWORD WINAPI generalTransmitter(LPVOID lpParam);
  DWORD WINAPI rcRemoteRequestTransmitter(LPVOID lpParam);
  DWORD WINAPI debugDataRequestTransmitter(LPVOID lpParam);
  DWORD WINAPI HILSSensorDataTransmitter(LPVOID lpParam);
}

using namespace port_usb_functions;
/************************************************************************************
 * Class definition                                                                 *
 ************************************************************************************/
class SerialCommunicator {
private:
    ////////////Variables:////////////////////////////////
    port_usb* my_port_usb; //the comms port through which we send & receive data
    port_com* my_port_com;
    void start_communication(void);
    int port_selected;
    char* message_error;
    volatile bool receiverLocked;
    serialComms::TransmissionDataPacket receivedPacket; //here we store data we get from the vehicle

    volatile bool transmitterLocked;//the "transmitter" is the vector below:
    std::vector<serialComms::TransmissionDataPacket> transmissionData; //here we store data to be sent to the vehicle

    serialComms::RCState lastRCState; //the last RC structure we received
    serialComms::DebugData lastDebugData; //the last debug data struct we received

    struct serialComms::HILSMotorCommands lastHILSMotorCommands; //the last HILS motor commands set we got
    volatile serialComms::HILSSensorPack HILSSensorData; //the sensor data we will send
    serialComms::HILSSensorPack A2DSensorDataIn; //sensor data we get from the vehicle
    SensorPack A2DSensorMeanMeasured; //the true mean values for the A2D (i.e. measuring zero)
    SensorPack A2DSensorMeanExpected; //the expected mean values for the A2D (i.e. measuring zero)

    volatile serialComms::DebugLabelElement debugLabels[serialComms::NUMBER_DEBUG_DATA]; //the debug labels

    volatile int lastReceivedHardwareVersion; //the last known hardware version
    volatile bool serialLinkTestPassed;
    volatile enum TypeHILS vehicleHILSEnabled; //whether the vehicle is running in HILS mode (-1:unknown, 0:false, 1: true)

    volatile char lastReceivedErrorString[serialComms::RXD_BUFFER_LEN];

    FILE *debugDataOuputFile, *debugLabelsOuputFile; //where we write the debug data and labels

    unsigned long int startTimeTicks; //the ticks when we started the communicator

    //counters:
    unsigned int numberOfUnknownPackets;   //the number of 'mystery' packets we've gotten
    unsigned int HILSSensorDataCounter;    //the number of sensor packets we've tried to send
    unsigned int HILSMotorCommandsCounter; //the number of motor commands we've received
    unsigned int receivedPacketsCounter;   //the total number of packets received

    //waiting times:
    unsigned int receiverThreadSleepTime_ms;    //the time the receiver thread sleeps between checking for data
    unsigned int transmitterThreadSleepTime_ms; //the time the transmission thread sleeps between checking for data
    unsigned int resourceLockedSleepTime_ms;    //the time we sleep between trying to access a locked resource
    unsigned int resourceLockedTimeout_ms;      //if we've waited this long for a locked resource, we give up
    unsigned int rcCheckSleepTime_ms;           //How often we ask for the remote state (if we do that)
    unsigned int debugDataSleepTime_ms;         //How often we ask to renew the debug data subscription (if we do that)
    unsigned int HILSTransmitSleepTime_ms;      //how often we send HILS sensor data out (if we do that)
    unsigned int confirmRequestTimeout_ms;      //How long we try to do a confirmRequest function before we give up

    ////////////Functions:////////////////////////////////
    void SendData(UINT8 packet[serialComms::RXD_BUFFER_LEN], UINT8 size);
    void SendData(serialComms::TransmissionDataPacket dataIn){SendData(dataIn.data,dataIn.size);}

    void addPacket(UINT8 CMD, int Address, UINT8 Data[serialComms::RXD_BUFFER_LEN], UINT8 Length);
    serialComms::TransmissionDataPacket makePacket(UINT8 CMD, int Address, UINT8 Data[serialComms::RXD_BUFFER_LEN], UINT8 Length);
    void USART_ProcessRxData(void);

    bool interpretCommonData(void); //returns true if the data was interpreted
    bool interpretDataFromFC(void); //returns true if the data was interpreted
    bool interpretDataFromNC(void); //returns true if the data was interpreted

public:
    SerialCommunicator(void);
    ~SerialCommunicator(void);

    void port_select(int);
    void open_port(int, int);
    void close_port(void);
    bool portSuccessfullyOpened(void);//{return port!=0;};
    char* getMessage(void);

    bool SerialRx(void); //the function called by the receiver thread

    //switch the system into HILS mode
    bool switchIntoHILSMode(enum TypeHILS mode, bool printDiagnosticInfo=1);

    //Comm's requests: (adds a request to the sending queue)
    // void requestRedirectUART_FC(void);
    // void requestRedirectUART_MK3MAG(void);
    // void requestRedirectUART_MKGPS(void);
    // void requestRedirectUART_NC(void);
    // void requestVersionInfo(void);
    void requestDebugData(UINT8 updateRate=serialComms::DEFAULT_DEBUG_DATA_UPDATE_SPEED_10MS);
    void requestDebugLabel(UINT8 labelNumber);
    void requestDebugLabelsAll(void);
    //  void requestSerialLinkTest(void);
    //  void requestErrorText(void);
    void requestExternalControl(void);
    void requestPPMChannels(void);
    void requestEngineTest(UINT8 engineTestSpeed);
    void requestChangePrintInfoStrings(bool newValue);
    void requestHILSState(void);
    void requestChangeHILSState(enum TypeHILS newValue);
    void requestA2DSensorValues(void);

    //Comm's confirming requests: (a function that sends a request and waits until it succeeds)
    // bool requestConfirmedRedirectUART(int newAddress);
    // bool requestConfirmedRedirectUART_FC(){return requestConfirmedRedirectUART(serialComms::ADDRESS_FC);};
    //  bool requestConfirmedRedirectUART_NC(){return requestConfirmedRedirectUART(serialComms::ADDRESS_NC);};//
    //  bool requestConfirmedRedirectUART_MK3MAG(){return requestConfirmedRedirectUART(serialComms::ADDRESS_MK3MAG);};
    bool requestConfirmedDebugLabel(UINT8 labelNumber);
    //  bool requestConfirmedDebugLabelsAll(void);
    //  bool requestConfirmedVersionInfo(void);
    //  bool requestConfirmedSerialLinkTest(void);
    //  bool requestConfirmedErrorString(void);
    bool requestConfirmedGetHILSState(void);
    bool requestConfirmedChangeHILSState(enum TypeHILS newValue);
    bool requestConfirmedA2DValues(void);

    //Comm's sending: (a function that skips the queue, sends a command directly)
    // You probably shouldn't use these yourself, but let the threads handle them
    void sendRCStateRequestNow(void);
    void sendDebugDataRequestNow(UINT8 updateRate = serialComms::DEFAULT_DEBUG_DATA_UPDATE_SPEED_10MS);
    void sendStopRequestNow();
    void sendHILSDataNow(void);

    volatile bool tryToLockTransmitter(void);
    void setUnlockTransmitter(void){transmitterLocked = 0;}

    void sendLastPacket(void);

    /////Get functions////////////////////////////
//    serialComms::RCState   getLastRCState(void){return lastRCState;};
    serialComms::DebugData getLastDebugData(void){return lastDebugData;}
    int                    getLastDebugDataAnalog(int number){return lastDebugData.data.Analog[number];}
    long int               getLastDebugDataTimeReceived(void){return lastDebugData.timeReceivedMS;}
    volatile int           getLastVersionInfo(void){return lastReceivedHardwareVersion;}
    volatile enum TypeHILS getLastHILSState(void){return vehicleHILSEnabled;}

    serialComms::HILSMotorCommands getLastHILSMotorCommands(void){lastHILSMotorCommands.haveNewData=0; return lastHILSMotorCommands;}
    struct SensorPack getLastA2DData(void){A2DSensorDataIn.haveNewData=0; return A2DSensorDataIn.sensorData;}

    void getDebugLabel(char outLabelText[serialComms::DEBUG_LABEL_LENGTH], int labelNum);
    void getErrorString(char outText[serialComms::RXD_BUFFER_LEN]);

    int getNumberOfUnknownPackets(void){return int(numberOfUnknownPackets);}
    int getReceiverThreadSleepTime_ms(void){return int(receiverThreadSleepTime_ms);}
    int getTransmitterThreadSleepTime_ms(void){return int(transmitterThreadSleepTime_ms);}
    int getResourceLockedSleepTime_ms(void){return int(resourceLockedSleepTime_ms);}
    int getResourceLockedTimeout_ms(void){return int(resourceLockedTimeout_ms);}
    int getRcCheckSleepTime_ms(void){return int(rcCheckSleepTime_ms);}
    int getDebugDataSleepTime_ms(void){return int(debugDataSleepTime_ms);}
    int getHILSTransmitSleepTime_ms(void){return int(HILSTransmitSleepTime_ms);}
    int getConfirmRequestTimeout_ms(void){return int(confirmRequestTimeout_ms);}

    unsigned int getNumberOfHILSMotorCommandsReceived(void){return HILSMotorCommandsCounter;}
    unsigned int getTotalNumberOfPacketsReceived(void){return receivedPacketsCounter;}

    volatile unsigned int getLastHILSSensorPacketNumber(void){return HILSSensorDataCounter;}
    volatile unsigned int getMotorCommandsSensorPacketNumber(void){return lastHILSMotorCommands.commands.sensorDataID;}

    volatile bool getHaveNewSensorData(void){return HILSSensorData.haveNewData;}
    volatile bool getHaveNewMotorData(void){return lastHILSMotorCommands.haveNewData;}

    /////Set functions////////////////////////////
    void setOutputFiles(FILE *debugDataFile, FILE* debugLabelsFile){debugDataOuputFile = debugDataFile; debugLabelsOuputFile = debugLabelsFile;}

    void setReceiverThreadSleepTime_ms(int inVal){receiverThreadSleepTime_ms = unsigned(inVal);}     //the time the receiver thread sleeps between checking for data
    void setTransmitterThreadSleepTime_ms(int inVal){transmitterThreadSleepTime_ms = unsigned(inVal);}//the time the transmission thread sleeps between checking for data
    void setResourceLockedSleepTime_ms(int inVal){resourceLockedSleepTime_ms = unsigned(inVal);}      //the time we sleep between trying to access a locked resource
    void setResourceLockedTimeout_ms(int inVal){resourceLockedTimeout_ms = unsigned(inVal);}          //if we've waited this long for a locked resource, we give up
    void setRcCheckSleepTime_ms(int inVal){rcCheckSleepTime_ms = unsigned(inVal);}                    //How often we ask for the remote state (if we do that)
    void setDebugDataSleepTime_ms(int inVal){debugDataSleepTime_ms = unsigned(inVal);}                //How often we ask to renew the debug data subscription (if we do that)
    void setHILSTransmitSleepTime_ms(int inVal){HILSTransmitSleepTime_ms = unsigned(inVal);}          //How often we send HILS sensor data
    void setConfirmRequestTimeout_ms(int inVal){confirmRequestTimeout_ms = unsigned(inVal);}          //How long we try to do a confirmRequest function before we give up

    // void setHILSSensorData(int gyroRoll, int gyroPitch, int gyroYaw, int accX, int accY, int accZ, int airPressure);//populate the HILS sensor info
    void setHILSSensorData(int theta, int theta_dot, int ADC_value); // GD: 04.05.2013 for arm prop project specific

    /////Reset Functions//////////////////////////
    void resetNumberOfHILSMotorCommandsReceived(void){HILSMotorCommandsCounter=0;}
    void resetTotalNumberOfPacketReceived(void){receivedPacketsCounter=0;}
};

#endif
