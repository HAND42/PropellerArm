

/**
  ******************************************************************************

  I2C Setup For STM32F446RE
  Author:   ControllersTech
  Updated:  31st Jan 2020

  ******************************************************************************
  Copyright (C) 2017 ControllersTech.com

  This is a free software under the GNU license, you can redistribute it and/or modify it under the terms
  of the GNU General Public License version 3 as published by the Free Software Foundation.
  This software library is shared with public for educational purposes, without WARRANTY and Author is not liable for any damages caused directly
  or indirectly by this software, read more about this on the GNU General Public License.

  ******************************************************************************
*/

#pragma once
#include <stdint.h>
#include "DisplayData.h";
#include "System_Config.h"
#include "TwinPropellerArmSTM32F411_configuration.h"

#define USART2_TX_BUFFER_SIZE 100

void I2C3_Begin(void);
void I2C3_Init(void);
void Error_Handler(void);
void I2C3_ReadData(void);
void I2C3_ER_IRQHandler(void);
void I2C3_ReadData(void);
void I2C3_EV_IRQHandler(void);
void I3C3_Stop(void);