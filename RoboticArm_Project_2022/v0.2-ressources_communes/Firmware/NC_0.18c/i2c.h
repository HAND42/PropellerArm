#ifndef __I2C_H
#define __I2C_H

#define I2C_CMD_VERSION           0x01
#define I2C_CMD_READ_MAG         0x02
#define I2C_CMD_READ_HEADING    0x03
#define I2C_CMD_WRITE_CAL         0x04


typedef struct
{
  u8 Major;
  u8 Minor;
  u8 Patch;
  u8 Compatible;
} __attribute__((packed)) I2C_Version_t;

typedef struct
{
  s16 MagX;
  s16 MagY;
  s16 MagZ;
} __attribute__((packed)) I2C_Mag_t;


typedef struct
{
  s16 Nick;
  s16 Roll;
} __attribute__((packed)) I2C_WriteAttitude_t;


typedef struct
{
  u8 CalByte;
  u8 Dummy1;
  u8 Dummy2;
} __attribute__((packed)) I2C_Cal_t;

typedef struct
{
  s16 Heading;
} __attribute__((packed)) I2C_Heading_t;

extern volatile u32 I2C1_Timeout;

extern volatile I2C_Heading_t        I2C_Heading;
extern volatile I2C_Mag_t            I2C_Mag;
extern volatile I2C_Version_t        MK3MAG_Version;

void I2C1_Init(void);
void I2C1_Deinit(void);
void I2C1_SendCommand(u8 command);
void I2C1_GetMK3MagVersion(void);
void I2C1_UpdateCompass(void);

#endif // I2C_H

