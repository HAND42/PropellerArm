/*
 * STM32 Projects
 *
 * Copyright (c) 2017 - Terence M. Darwen - tmdarwen.com
 *
 * The MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

// The STM32F411 comes with a LSM303DLHC 3D accelerometer on the board.  This 
// header allows for initializing the device and sending realtime acceleroemeter 
// info to a terminal via UART connected to PA2.  See UART.h for more info on 
// how data is transfered to the terminal by UART.
#pragma once

#include "stm32f411xe.h"

void AccelerometerInit(void);
void GetAccelerometerValues(short* x, short* y, short* z);

//Added for LCD interface I2C
void I2CWaitIfBusy(void);
void I2CEnableAcknowledge(void);
void I2CStartRestart(void);
void I2CSendSlaveAddress(unsigned short address);
void I2CWriteByte(unsigned char data);
void I2CStop (void);
