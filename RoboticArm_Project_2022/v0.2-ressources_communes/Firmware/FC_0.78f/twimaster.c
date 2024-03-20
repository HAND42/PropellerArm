/*############################################################################
############################################################################*/

#include "main.h"

volatile unsigned char twi_state = 0;
unsigned char motor = 0;
unsigned char motorread = 0,MissingMotor = 0;

MotorData_t Motor[MAX_MOTORS];

unsigned int I2CError = 0;

void i2c_init(void){//Initzialisieren der I2C (TWI) Schnittstelle
  TWSR = 0;
  TWBR = ((SYSCLK/SCL_CLOCK)-16)/2;
}

void i2c_start(void){//Start I2C
  TWCR = (1<<TWSTA) | (1<<TWEN) | (1<<TWINT) | (1<<TWIE);
}

void i2c_stop(void){
  TWCR = (1<<TWEN) | (1<<TWSTO) | (1<<TWINT);
}

void i2c_reset(void){
  i2c_stop();
  twi_state = 0;
  motor = TWDR;
  motor = 0;
  TWCR = 0x80;
  TWAMR = 0;
  TWAR = 0;
  TWDR = 0;
  TWSR = 0;
  TWBR = 0;
  i2c_init();
  i2c_start();
  i2c_write_byte(0);
}

void i2c_write_byte(char byte){
  TWSR = 0x00;
  TWDR = byte;
  TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWIE);
}


void I2C_WriteByte(int8_t byte){//Write to I2C
  // move byte to send into TWI Data Register
  TWDR = byte;
  // clear interrupt flag (TWINT = 1)
  // enable i2c bus (TWEN = 1)
  // enable interrupt (TWIE = 1)
  TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWIE);
}

void I2C_ReceiveByte(void){//Receive byte and send ACK
  TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWIE) | (1<<TWEA);
}

void I2C_ReceiveLastByte(void){//I2C receive last byte and send no ACK
  TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWIE);
}

SIGNAL (TWI_vect){
  static unsigned char missing_motor;
  switch(twi_state++){
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // Writing the Data
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  case 0:
    while(Mixer.Motor[motor][MIXER_GAS] <= 0 && motor < MAX_MOTORS){
      motor++;  // skip if not used
    }
    if(motor == MAX_MOTORS){// writing finished -> now read
      motor = 0;
      twi_state = 3;
      i2c_write_byte(0x53+(motorread*2));
    }
    else{
      i2c_write_byte(0x52+(motor*2));
    }
    break;
  case 1:
    i2c_write_byte(Motor[motor++].SetPoint);
    break;
  case 2:
    if(TWSR == 0x30)
    {
      if(!missing_motor) missing_motor = motor;
      if((Motor[motor-1].State & MOTOR_STATE_ERROR_MASK) < MOTOR_STATE_ERROR_MASK) Motor[motor-1].State++; // increment error counter and handle overflow
    }
    i2c_stop();
    I2CTimeout = 10;
    twi_state = 0;
    i2c_start();
    break;
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // Reading Data
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  case 3:
    //Transmit 1st byte for reading
    if(TWSR != 0x40)  // Error?
    {
      Motor[motorread].State &= ~MOTOR_STATE_PRESENT_MASK; // clear present bit
      motorread++;
      if(motorread >= MAX_MOTORS) motorread = 0;
      i2c_stop();
      twi_state = 0;
    }
    else
    {
      Motor[motorread].State |= MOTOR_STATE_PRESENT_MASK; // set present bit
      I2C_ReceiveByte();
    }
    MissingMotor = missing_motor;
    missing_motor = 0;
    break;
  case 4: //Read 1st byte and transmit 2nd Byte
    Motor[motorread].Current = TWDR;
    I2C_ReceiveLastByte(); //nack
    break;
  case 5:
    //Read 2nd byte
    Motor[motorread].MaxPWM = TWDR;
    motorread++; // next motor
    if(motorread >= MAX_MOTORS) motorread = 0;
    i2c_stop();
    twi_state = 0;
    break;

    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // writing Gyro-Offset
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  case 8:
    i2c_write_byte(0x98); // Address of the DAC
    break;
  case 9:
    i2c_write_byte(0x10); // Update Channel A
    break;
  case 10:
    i2c_write_byte(analogueOffset.pitch); // Value
    break;
  case 11:
    i2c_write_byte(0x80); // Value
    break;
  case 12:
    i2c_stop();
    I2CTimeout = 10;
    i2c_start();
    break;
  case 13:
    i2c_write_byte(0x98); // Address of the DAC
    break;
  case 14:
    i2c_write_byte(0x12); // Update Channel B
    break;
  case 15:
    i2c_write_byte(analogueOffset.roll); // Value
    break;
  case 16:
    i2c_write_byte(0x80); // Value
    break;
  case 17:
    i2c_stop();
    I2CTimeout = 10;
    i2c_start();
    break;
  case 18:
    i2c_write_byte(0x98); // Address of the DAC
    break;
  case 19:
    i2c_write_byte(0x14); // Update Channel C
    break;
  case 20:
    i2c_write_byte(analogueOffset.yaw); // Value
    break;
  case 21:
    i2c_write_byte(0x80); // Value
    break;
  case 22:
    i2c_stop();
    I2CTimeout = 10;
    twi_state = 0;
    break;
  default: twi_state = 0;
    break;
  }
  TWCR |= 0x80;
}
