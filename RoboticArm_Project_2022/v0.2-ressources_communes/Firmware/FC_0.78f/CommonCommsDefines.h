/* mwm 06.06.2011
 *
 * Defines the baud rate used in communication (used by firmware and 
 * pc-side comms)
 * Modified by GD on 03 June 2013
 */
#ifndef COMMONCOMMSDEFINES_H
#define COMMONCOMMSDEFINES_H

//--define the baud rate---------------------------------------------------
static const unsigned long USE_BAUD_RATE = 19200; // for the microcontroller
//static const unsigned long USE_BAUD_RATE = 57600; //original value (seems to be the only one to work with Mikrokopter FC)
//static const unsigned long USE_BAUD_RATE = 38400;
//static const unsigned long USE_BAUD_RATE = 115200;

//Numbers that make sense:
//300, 600, 1200, 2400, 4800, 9600, 14400, 19200, 38400, 57600, 115200, 230400, 460800, 921600

//--Define the addresses used----------------------------------------------
#define FC_ADDRESS     1
#define NC_ADDRESS     2
#define MK3MAG_ADDRESS 3

enum TypeHILS{//different HILS modes
  /* These are the different options for HILS mode. Note that the integer values 
   * corresponding to each enry must be in {0, 1, .., 255}, since we typecast it
   * into an (unsigned char) for communication via serial. (Because an enum is
   * not guaranteed to have the same size on different implementations.)
   */
  //HILS_REAL_FLIGHT = 0,//real flight (no HILS)

  //HILS_PROPER_HILS = 1,//real HILS, NOT allowing the motors to turn
  //HILS_EQUAL_MOTOR_COMMANDS = 2,//we just send back constant commnads
  //HILS_PROPER_HILS_WITH_MOTORS_ON = 3,//like proper hils, but we allow the motors to turn (might be unsafe)
    
// HILS = 0, In this mode, we are not doing Hardware in the loop, the PC simulator runs on its own
// HILS = 1 if you want to do Hardware in the loop, 
// In this mode, we want that the PC sends the Board sensor data
// The board should reply with motor commands, which treated by the PC simulator and displayed in the 3D animation ...    
// HILS = 2: In this mode, the 3D GUI is only used to display the real angle of the balancing arm
// HILS = 3: In this mode, we want the board to send the true angle values and the PC computes the control law and send to the board the
// force that should be generated to control the arm.
  HILS_MODE_0 = 0,
  HILS_MODE_1 = 1,
  HILS_MODE_2 = 2,
  HILS_MODE_3 = 3,
  HILS_MODE_4 = 4,
  HILS_UNKNOWN_MODE = 255,//used as place-holder value on pc-side when we don't know what the onboard status is (this should never be used on-booard!)
};

#endif
