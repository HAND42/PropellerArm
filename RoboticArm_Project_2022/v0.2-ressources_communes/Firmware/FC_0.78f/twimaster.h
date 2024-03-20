/*############################################################################
############################################################################*/

#ifndef _I2C_MASTER_H
#define _I2C_MASTER_H

//############################################################################

// I2C Konstanten
#define SCL_CLOCK  200000L
#define I2C_TIMEOUT 30000
#define I2C_START          0x08
#define I2C_REPEATED_START 0x10
#define I2C_TX_SLA_ACK     0x18
#define I2C_TX_DATA_ACK    0x28
#define I2C_RX_SLA_ACK     0x40
#define I2C_RX_DATA_ACK    0x50

//############################################################################

extern volatile unsigned char twi_state;
extern unsigned char motor,MissingMotor;
extern unsigned char motorread;

#define MAX_MOTORS 12
#define MOTOR_STATE_PRESENT_MASK    0x80
#define MOTOR_STATE_ERROR_MASK        0x7F

typedef struct
{
    unsigned char SetPoint; // written by attitude controller
    unsigned char State;    // 7 bit for I2C error counter, highest bit indicates if motor is present
    unsigned char Current;  // in 0.1 A steps, read back from BL
    unsigned char MaxPWM;   // read back from BL is less than 255 if BL is in current limit
} __attribute__((packed)) MotorData_t;

extern MotorData_t Motor[MAX_MOTORS];

void i2c_reset(void);
extern void i2c_init (void); // I2C initialisieren
extern void i2c_start (void); // Start I2C
extern void i2c_stop (void); // Stop I2C
extern void i2c_write_byte (char byte); // 1 Byte schreiben
extern void i2c_reset(void);


extern struct UnsignedCharAngles analogueOffset; //replaces AnalogOffset{Nick,Roll,Gier}

#endif
