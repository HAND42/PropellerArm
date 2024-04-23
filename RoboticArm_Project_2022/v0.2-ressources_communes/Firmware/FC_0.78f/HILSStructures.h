/* mwm: 31.05.2011
 *
 * Structures to be used by Hardware In the Loop Sim
 * Used by FC and by the thing reading the info
 * 
 * mwm: 13.07.2011
 * Added "extended" sensor structure, containing data for 
 * magnetometer.
 *
 * 19.07.2011 
 * Now supercedes the version of "HILSStructures.h" in 
 * $SVN/Firmware/FC_0.78f/.

* 2013: GD heavily modified by GD for the needs of the balancing arm project

 */
#ifndef HILSSTRUCTURES_H
#define HILSSTRUCTURES_H

#include "constants.h"//for NUMBER_OF_MOTORS

/*-NOTE:----------------------------------------------------------------------------------------------------
 Because the on-board code is compiled by the GCC compiler, and the off-board code (i.e. communicator)
 is compiled by MS Visual C++ compiler, they are not completely equivalent. E.g. the integers have 
 different sizes (in gcc, sizeof(int)=2, and in MSVC sizeof(int)=4). For this reason we define 
 custom COMMS_* types, which we *know* are compiler independent. 

 Now we know that COMMS_INT will have the same size on-board and off-board, and we can easily communicate 
 (more to the point, we know that the structures MotorCommandsHILS and SensorPack will have the same size, 
 and we can communicate them via serial in a straight-forward manner).

 Packing has to do with how structures are physically stored in memory, and here the syntax is different for 
 the different compilers. Refer to http://stackoverflow.com/questions/1537964/visual-c-equivalent-of-gccs-attribute-packed
----------------------------------------------------------------------------------------------------------*/

#ifdef __GNUC__ //for ggc (i.e. for on-board fc code)
#define PACK( __Declaration__ ) __Declaration__ __attribute__((__packed__))
#include <stdint.h>
typedef int16_t  COMMS_INT;
typedef int32_t  COMMS_LONG_INT;
typedef uint8_t  COMMS_UNSIGNED_CHAR;
typedef uint16_t COMMS_UINT;
typedef uint32_t COMMS_LONG_UINT;
//typedef uint64_t COMMS_UNSIGNED_INT64;
#else //for ms vc++ (i.e. for off-board SerialCommunicator object)
#define PACK( __Declaration__ ) __pragma( pack(push, 1) ) __Declaration__ __pragma( pack(pop) )
#include <windows.h>
typedef INT16  COMMS_INT;
typedef INT32  COMMS_LONG_INT;
typedef UINT8  COMMS_UNSIGNED_CHAR;
typedef UINT16 COMMS_UINT;
typedef UINT32 COMMS_LONG_UINT;
//typedef UINT64 COMMS_UNSIGNED_INT64;
#endif

// Structures used in communications:
PACK(struct MotorCommandSet{
  //COMMS_UNSIGNED_CHAR setpoint;//the motor setpoint
  //COMMS_UNSIGNED_CHAR sensorDataID; //the ID number of the sensor data used to generate these commands    
    COMMS_UINT setpoint;//the motor setpoint
    COMMS_UINT sensorDataID; //the ID number of the sensor data used to generate these commands    
});

PACK(struct SensorsA2D{//values from the A2D sensors
  //all A2D values are supposed to be 10 bit, i.e. between 0 and 1023
  //NB: Coordinates are body frame, z UP!
  //struct CommsIntAngles gyroscopes;
  //struct CommsIntXYZ accelerometer;
  //COMMS_INT airPressureSensor;
  
  COMMS_INT theta;
  COMMS_INT theta_dot;

});

struct SensorsArmProp{//sensors of the Propeller Arm system
    COMMS_INT theta; 
    COMMS_INT theta_dot; //angle and angle speed of the arm
    COMMS_INT ADC_value;
};

PACK(struct SensorPack{   
    struct SensorsArmProp ArmProp;    // 04/05/2013 adapted from mwm for the armprop project
    COMMS_UNSIGNED_CHAR id; //an internal identification number
});

//---------------------------------------------------------------------------------------------
// The two most important struct for HILS used with the FC, GD's comment 04.05.2013
//---------------------------------------------------------------------------------------------
struct MotorCommandsHILS{
  struct MotorCommandSet commands;
  unsigned char haveDataToSend; //do we have something new to send (number of motor data in)
};

struct SimulatedSensorData{//used in FC 
  struct SensorPack sensors;
  long controllerCycleWhenReceived;
};

////---------------------------------------------------------------------------------------------
//// The two most important struct for HILS used with the FC, GD's comment 04.05.2013
////---------------------------------------------------------------------------------------------
//struct MotorCommandsHILS{
//  COMMS_UNSIGNED_CHAR setpoint;
//  unsigned char haveDataToSend; //do we have something new to send (number of motor data in)
//};
//
//struct SimulatedSensorData{ // data received from PC
//  struct SensorPack sensors;
//  long controllerCycleWhenReceived;
//};
//
//struct RealSensorData{
//    COMMS_INT theta; //COMMS_INT is used because this is a 10-bit variable
//    COMMS_INT theta_dot; //COMMS_INT is used because this is a 10-bit variable
//    unsigned char haveDataToSend;
//}

//PACK(struct SensorPackExtended{
//  
//    struct SensorsArmProp ArmProp;    // 04/05/2013 adapted from mwm for the armprop project    
//    //This adds the magnetometer unit vector measurement
//    //struct SensorsA2D A2D;
//  //struct SensorsNavi navi; 
//  //struct SensorsMagnetometer mag;
//  COMMS_UNSIGNED_CHAR id; //an internal identification number
//});
//Building block structures
/*struct CommsIntAngles{
  COMMS_INT roll;
  COMMS_INT pitch;
  COMMS_INT yaw;
};

struct CommsIntXYZ{
  COMMS_INT x;
  COMMS_INT y;
  COMMS_INT z;
};*/

/*
PACK(struct SensorsNavi{//sensors from the NaviCtrl
  COMMS_INT compassValue; //angle from north, in degrees, in {0, 1, ..., 360}
});

PACK(struct SensorsMagnetometer{
  struct CommsIntXYZ magVec;
});*/

/*
PACK(struct QuaternionSet{ // used to send back to PC the estimated quaternion
 COMMS_UNSIGNED_CHAR quat[4];     
});*/

#endif
