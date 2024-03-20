/* mwm 11.05.2011
 *
 * A file containing all the hash defines and typedefs for the communication
 * Replaces global.h & typedefs.h
 */

#ifndef COMMUNICATIONDEFINES_H
#define COMMUNICATIONDEFINES_H

//#include <Windows.h>
#include "../Firmware/FC_0.78f/HILSStructures.h"
#include "../Firmware/FC_0.78f/CommonCommsDefines.h" //for addresses

namespace serialComms{
    /************************************************************************************
     * Constants                                                                        *
     ************************************************************************************/
    static const int DEFAULT_RECEIVER_THREAD_SLEEP_TIME_MS    = 1;    //the time the receiver thread sleeps between checking for data
    static const int DEFAULT_TRANSMITTER_THREAD_SLEEP_TIME_MS = 30;   //the time the transmission thread sleeps between checking for data
    static const int DEFAULT_RESOURCE_LOCKED_SLEEP_TIME_MS    = 10;   //the time we sleep between trying to access a locked resource
    static const int DEFAULT_RESOURCE_LOCKED_TIMEOUT_MS       = 2000; //if we've waited this long for a locked resource, we give up
    static const int DEFAULT_RC_CHECK_SLEEP_TIME_MS           = 20;   //How often we ask for the remote state (if we do that)
    static const int DEFAULT_DEBUG_DATA_SLEEP_TIME_MS         = 4000; //How often we ask to renew the debug data subscription (if we do that)
    static const int DEFAULT_HILS_TRANSMIT_SLEEP_TIME_MS      = 10;   //How often we send HILS sensor data
    static const int DEFAULT_CONFIRM_REQUEST_TIMEOUT_MS       = 1000; //How long we try to do a confirmRequest function before we give up
    static const int DEFAULT_DEBUG_DATA_UPDATE_SPEED_10MS     = 2;    //this is the default rate at which the debug data should come back (*10 [ms])

    // Base address of the different Hardware
    static const int ADDRESS_ALL     = 0;
    static const int ADDRESS_FC      = FC_ADDRESS;
    static const int ADDRESS_NC      = NC_ADDRESS;
    static const int ADDRESS_MK3MAG  = MK3MAG_ADDRESS;
    static const int ADDRESS_UNKNOWN =-1;

  //the values we expect the A2D to give, when all sensors are at nominal values
  static const int A2D_ZERO_VALUE_GYROSCOPES     = 511; //TODO: VERIFY!!!
  static const int A2D_ZERO_VALUE_ACCELEROMETERS = 511; //TODO: VERIFY!!! (seems OK -- Z-value reads 225 higher than x,y, which corresponsd to ~ gravity)
  static const int A2D_ZERO_VALUE_AIRPRESSURE    = 511; //TODO: VERIFY!!!

  //how the sensors convert measurements to bits:
  static const double A2D_CONVERSION_FACTOR_GYRO = 0.0142; //[rad/s/bit]
  static const double A2D_CONVERSION_FACTOR_ACC  = 0.0479; //[(m/s^2)/bit]

    //Test the serial communications (chars are arbitrary)
  static const UINT8 SERIAL_LINK_TEST_CHAR_0 = 'B';
  static const UINT8 SERIAL_LINK_TEST_CHAR_1 = '+';

    // Debug data
    static const int NUMBER_DEBUG_DATA = 32; //the number of labels/debug data
    static const int DEBUG_LABEL_LENGTH = 16; //the length of the labels

    //from main.cpp
  static const int RXD_BUFFER_LEN = 150;//the length of the buffer used to store the info received from the vehicle

    /************************************************************************************
     * Structures                                                                       *
     ************************************************************************************/

    //from QMK groundstation: a structure for PPM data
    struct str_PPMData{
    //the values of the RC sender. Integer values in range {-125,..,125}. See also http://www.mikrokopter.de/ucwiki/MK-Parameter/Channels
        int16_t Data[26];
    };

  struct s_MK_Debug{//Debug data structure, from: QMK Groundstation, MK_Datatypes.h ("s_MK_Debug")
   unsigned char Digital[2];
   int16_t Analog[NUMBER_DEBUG_DATA];    // Debugwerte
     //angles are measured in 0.1 degrees (?maybe?) - from inspection of UART.C
  };

  struct DebugData{
        s_MK_Debug data;
        long int timeReceivedMS; //when we received the packet (evaluated with GetTickCount())
  };

  //---------------------------------------------
  // HILS structures
  //---------------------------------------------
  struct HILSMotorCommands{
    struct MotorCommandSet commands;
        long int timeReceivedMS; //when we received the packet (evaluated with GetTickCount())
    volatile bool haveNewData;//we should send at most one set of sensor data per received motor commands packet
  };

  struct HILSSensorPack{
    struct SensorPack sensorData;
    volatile bool haveNewData;//do we have fresh data to send?
  };
  //---------------------------------------------


    struct str_VersionInfo{//from UART.C: the version info
        unsigned char SWMajor;
        unsigned char SWMinor;
        unsigned char ProtoMajor;
        unsigned char ProtoMinor;
        unsigned char SWPatch;
        unsigned char HardwareError[5];
    };

    
    struct set_Analog{//from cSetting.h (probably originally from QMK)
        char Version;
        char Label[NUMBER_DEBUG_DATA][DEBUG_LABEL_LENGTH];
    };

    
    struct sResend{//from cSetting.h (probably originally from QMK)
        uint8_t packet[RXD_BUFFER_LEN];
        uint8_t size;
        bool start;
    };

    struct TransmissionDataPacket{
        uint8_t data[RXD_BUFFER_LEN];
        uint8_t size;
    };

    struct RCState{
        str_PPMData channels;
        long int timeReceivedMS; //when we received the packet (evaluated with GetTickCount())
    };

    struct DebugLabelElement{
        char label[DEBUG_LABEL_LENGTH]; //the text of the label
        int  hardware;   //where the label comes from (e.g. Flight control...)
    };
}//end of namespace "serialComms"

#endif
