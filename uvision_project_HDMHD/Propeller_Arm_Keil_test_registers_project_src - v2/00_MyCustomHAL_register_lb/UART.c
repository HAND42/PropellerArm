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

#include "RegisterAddresses.h"
#include <stdio.h>
#include "UART.h"
#include "stm32f411xe.h"

#define MAX_BUFFER_SIZE 2000
unsigned char dmaBuffer6[MAX_BUFFER_SIZE];
unsigned char dmaBuffer2[MAX_BUFFER_SIZE];

volatile int uart2Busy = 0;
volatile int uart6Busy = 0;

  /*****************************************************************************************************************************************/
 /*		---------------------------------------------	USART 2 		---------------------------------------------------------------------*/
/*****************************************************************************************************************************************/

// DMA1 Stream5 will be triggered for UART2 DMA TX (i.e when sending a character to the terminal)
void DMA1_Stream5_IRQHandler(void)
{
	// See pg 187.  Bit 11 is the "Stream 5 transfer complete interrupt flag".  This bit
	// will be set when the DMA reception is complete.
	if((ACCESS(DMA1_HISR) & (1 << 11)) != 0)
	{
		// Clear the transfer complete interrupt (pg 188)
		ACCESS(DMA1_HIFCR) |= (1 << 11);

		// Enable the DMA transfer stream. (pg 192)
		ACCESS(DMA1_S6CR) |= 1;
	}
}

// DMA1 Stream6 will be triggered for UART2 DMA RX (i.e. when receiving a character from the terminal)
void DMA1_Stream6_IRQHandler(void)
{
	
	// See pg 187.  Bit 21 is the "Stream 6 transfer complete interrupt flag".  This bit
	// will be set when the DMA transfer is complete.
	if((ACCESS(DMA1_HISR) & (1 << 21)) != 0)
	{
		// Clear the transfer complete interrupt (pg 188)
		ACCESS(DMA1_HIFCR) |= (1 << 21);

		// Enable the DMA reception stream (pg 192)
		ACCESS(DMA1_S5CR) |= 1;
		
		// See pg 552.  Here we specify that we want an interrupt generated once the
		// USART transmission is complete.
		ACCESS(USART2_CR1) |= (1 << 6);
	}
}

void USART2_IRQHandler(void)
{
	// See pg 549.  Bit 6 of the status register will be set when the UART
	// transmission has completed.
	if((ACCESS(USART2_SR) & (1 << 6)) != 0)
	{
		// Clear the interrupt. (...So that it doesn't continually trigger)
		ACCESS(USART2_CR1) &= ~(1 << 6);

		// Clear the busy flag to allow the next transmission.
		uart2Busy = 0;
	}
}



void Uart2GpioInit()
{
	// Give a clock to port A as we'll be using one of its pins for transfer of data.
	ACCESS(RCC_AHB1ENR) |= 1;

	// See pg 19 of the ST UM1842 document.  We'll be using USART2.  USART2 TX occurs on
	// PA2 and USART2 RX occurs on PA3 so we set this pin to alternate mode.
	ACCESS(GPIOA_MODER) |= ((1 << 5) | (1 << 7));

	// See pg 149 of ST RM0383 document.  USART2 is AF7.  And pg 160 of the same document
	// shows alternate function for pins 2 and 3 are set using alternate function low register
	// bits 8-thru-11.
	ACCESS(GPIOA_AFRL) |= ((7 << 8) | (7 << 12));

	// Set PA2 and PA3 to high speed
	ACCESS(GPIOA_OSPEEDR) |= ((3 << 4) | (3 << 6));
}

void Uart2DmaInit()
{
	// Enable a clock for DMA1
	ACCESS(RCC_AHB1ENR) |= (1 << 21);

	// See pg 189 for details of the DMA stream configuration register
	// Setting bit 6 specifies memory-to-peripheral communication.
	// Setting bit 10 specifies to increment the memory pointer after each data transfer.  This
	// allows the DMA device to progressively step through the dmaBuffer array.
	// Setting bit 27 specifies to use channel 4 of stream 5.
	ACCESS(DMA1_S6CR) |= ((1 << 6) | (1 << 27));
	ACCESS(DMA1_S6CR) |= (1 << 10);
	ACCESS(DMA1_S5CR) |= ((1 << 27));
	
	// Enable interrupts for DMA1_Stream5 and DMA1_Stream6.  See page 209 of ST's PM0214 document
	// for info on this register and pg 202 of RM0383 for the position in the NVIC for these
	// interrupts.
	ACCESS(NVIC_ISER0) |= ((1 << 16) | (1 << 17));

	// The DMAx_SyNDTR register holds the number of data items to transfer (pg 192).  We're doing
	// one character at a time.  Note that bits 11-12 of the DMA configuration register (pg 190)
	// allows us to configure how large each "data item" is (8, 16 or 32 bits).  Since we're doing
	// 8 bit characters, and this corresponds to a setting of "00", we don't need to set this.
	ACCESS(DMA1_S5NDTR) = 1;
	ACCESS(DMA1_S6NDTR) = 1;

	// See pg 193.  This register holds the peripheral data register.  Since we're using USART2
	// we set it to its data register.
	ACCESS(DMA1_S5PAR) = USART2_DR;
	ACCESS(DMA1_S6PAR) = USART2_DR;

	// See pg 193.  This register holds the memory address of the data we want to transfer.  Since
	// we transfer only one character at a time the "buffer" only needs to be one character in size.
	// Also, since the data received is the data we then want to immediately transfer, we just use
	// the same buffer for both receiving and transfer.
	
	ACCESS(DMA1_S5M0AR) = (unsigned int)(dmaBuffer2);
	ACCESS(DMA1_S6M0AR) = (unsigned int)(dmaBuffer2);

	// Enable an interrupt to occur when the DMA operations complete (pg 192)
	ACCESS(DMA1_S5CR) |= (1 << 4);
	ACCESS(DMA1_S6CR) |= (1 << 4);

	// Enable the DMA reception stream (pg 192)
	ACCESS(DMA1_S5CR) |= 1;
}

void Uart2Init(void)
{
	Uart2GpioInit();

	Uart2DmaInit();

	// Give a clock USART2.  See pg 117.
	ACCESS(RCC_APB1ENR) |= (1 << 17);

	// Here we set the baud rate.  This is explained on 519 of ST RM0383.  The peripheral
	// clock is 16 MHz by default.  So, the calculation for the USARTDIV is:
	// DesiredBaudRate = 16MHz / 16*USARTDIV
	// Note that the fractional part of USARTDIV is represented with only 4 bits.  So
	// if we use 9600 this will result in a small error of 0.02% (see pg 522).  Therefore
	// the baud rate is actually 9,598.  Again, see pg 522.
	//ACCESS(USART2_BRR) |= 0x683;
	
	
	//Baud Rate = 115,200
	ACCESS(USART2_BRR) = (2<<0) | (27<<4);

	// Enable USART2 for transmitting data.  See page 552.
	// Bit 2 = Receiver enabled
	// Bit 3 = Transmitter enabled
	// Bit 13 = Enable the UART
	ACCESS(USART2_CR1) |= ((1 << 2) | (1 << 3) | (1 << 13));
	
	// Enable UART2 DMA for receiving/transferring data
	ACCESS(USART2_CR3) |= ((1 << 6) | (1 << 7));

	// See pg 200 of RM0383 ("STM32F411xC/E Reference Manual") for the interrupt
	// vector table.  You'll see USART2 is interrupt 38.
	// See pg 209 of PM0214 ("STM32F4 Series Programming Manual") for info on "ISER"
	// which is one of the "interrupt set-enable registers".  We enable interrupt
	// number 38.
	ACCESS(NVIC_ISER1) |= (1 << (38 - 32));
}

void SendString(char* string)
{
	
	// Here we block until the previous transfer completes.
	while(uart2Busy == 1);
	uart2Busy = 1;

	// Copy the string into the DMA buffer and also calculate its length.
	int lengthOfString = 0;
	while(lengthOfString < MAX_BUFFER_SIZE && string[lengthOfString])
	{
		dmaBuffer2[lengthOfString] = string[lengthOfString];
		++lengthOfString;
	}

	// See pg 192.  This register holds the number of data items to transfer.
	ACCESS(DMA1_S6NDTR) = lengthOfString;

	// See pg 193.  This register holds the peripheral data register.  Since we're
	// using USART2 we set it to its data register.
	ACCESS(DMA1_S6PAR) = USART2_DR;

	// See pg 193.  This register holds the memory address of the data we want to transfer.
	ACCESS(DMA1_S6M0AR) = (unsigned int)dmaBuffer2;

	// Enable interrupt to occur upon completion of transfer
	ACCESS(DMA1_S6CR) |= (1 << 4);

	// Enable the stream
	ACCESS(DMA1_S6CR) |= 1;

	// Clear the transfer complete flag in the UART SR
	ACCESS(USART2_SR) &= ~(1 << 6);

	// Enable DMA transmission
	ACCESS(USART2_CR3) |= (1 << 7);
}

  /*****************************************************************************************************************************************/
 /*		---------------------------------------------	USART 6 		---------------------------------------------------------------------*/
/*****************************************************************************************************************************************/


// DMA2 Stream1 will be triggered for UART6 DMA TX (i.e when sending a character to the terminal)
void DMA2_Stream1_IRQHandler(void)
{
	// See pg 187.  Bit 11 is the "Stream 1 transfer complete interrupt flag".  This bit
	// will be set when the DMA reception is complete.
	if((ACCESS(DMA2_LISR) & (1 << 11)) != 0)
	{
		// Clear the transfer complete interrupt (pg 188)
		ACCESS(DMA2_LIFCR) |= (1 << 11);

		// Enable the DMA transfer stream. (pg 192)
		ACCESS(DMA2_S6CR) |= 1;
	}
}

// DMA2 Stream6 will be triggered for UART6 DMA RX (i.e. when receiving a character from the terminal)
void DMA2_Stream6_IRQHandler(void)
{
	
	// See pg 187.  Bit 21 is the "Stream 6 transfer complete interrupt flag".  This bit
	// will be set when the DMA transfer is complete.
	if((ACCESS(DMA2_HISR) & (1 << 21)) != 0)
	{
		// Clear the transfer complete interrupt (pg 188)
		ACCESS(DMA2_HIFCR) |= (1 << 21);

		// Enable the DMA reception stream (pg 192)
		ACCESS(DMA2_S1CR) |= 1;
		
		// See pg 552.  Here we specify that we want an interrupt generated once the
		// USART transmission is complete.
		ACCESS(USART6_CR1) |= (1 << 6);
	}
}


void USART6_IRQHandler(void)
{
	// See pg 548.  Bit 6 of the status register will be set when the UART
	// transmission has completed.
	if((ACCESS(USART6_SR) & (1 << 6)) != 0)
	{
		// Clear the interrupt. (...So that it doesn't continually trigger)
		ACCESS(USART6_CR1) &= ~(1 << 6);

		// Clear the busy flag to allow the next transmission.
		uart6Busy = 0;
	}
}


void Uart6GpioInit()
{
	// Give a clock to port C as we'll be using one of its pins for transfer of data.
	ACCESS(RCC_AHB1ENR) |= (1<<2);
	
	// See pg 23 of the ST UM1842 document.  We'll be using USART6 for QT communication.  USART6 TX occurs on
	// PC6 and USART6 RX occurs on PC7 so we set this pin to alternate mode.
	ACCESS(GPIOC_MODER) |= ((2 << 14) | (2 << 12));
	
	// See pg 150 of ST RM0383 document.  USART6 is AF8.  And pg 162 of the same document
	// shows alternate function for pins 6 and 7 are set using alternate function low register
	ACCESS(GPIOC_AFRL) |= ((8 << 28) | (8 << 24));
	
	// Set PC6 and PC7 to high speed
	ACCESS(GPIOC_OSPEEDR) |= ((3 << 12) | (3 << 14));
	
}

void Uart6DmaInit()
{
    // Enable a clock for DMA2
    ACCESS(RCC_AHB1ENR) |= (1 << 22);

    // See pg 187 for details of the DMA stream configuration register
	// Setting bit 6 specifies memory-to-peripheral communication.
	// Setting bit 10 specifies to increment the memory pointer after each data transfer.  This
	// allows the DMA device to progressively step through the dmaBuffer array.
	// Setting bits 27:26:25 specifies to use channel 5 of stream 6 and 1.
	ACCESS(DMA2_S6CR) |= ((1 << 6) | (5 << 25));
	ACCESS(DMA2_S6CR) |= (1 << 10);
	ACCESS(DMA2_S1CR) |= ((5 << 25));

	 // See pg 204 of RM083 for details of the NVIC configuration register
    // Enable interrupt for DMA2_Stream1&6
	ACCESS(NVIC_ISER1) = (1 << 25);  // Enable interrupt 57 - DMA2 Stream 1
	ACCESS(NVIC_ISER2) = (1 << 5);   // Enable interrupt 69 - DMA2 Stream 6
	
	// The DMAx_SyNDTR register holds the number of data items to transfer (pg 192).  We're doing
	// one character at a time.  Note that bits 11-12 of the DMA configuration register (pg 190)
	// allows us to configure how large each "data item" is (8, 16 or 32 bits).  Since we're doing
	// 8 bit characters, and this corresponds to a setting of "00", we don't need to set this.
	// Stream 1 & 6 enable / flag stream ready when read low
	ACCESS(DMA2_S1NDTR) = 1;
	ACCESS(DMA2_S6NDTR) = 1;

	// See pg 193.  This register holds the peripheral data register.  Since we're using USART6
	// we set it to its data register.
	ACCESS(DMA2_S1PAR) = USART6_DR;
	ACCESS(DMA2_S6PAR) = USART6_DR;

	// See pg 193.  This register holds the memory address of the data we want to transfer.  Since
	// we transfer only one character at a time the "buffer" only needs to be one character in size.
	// Also, since the data received is the data we then want to immediately transfer, we just use
	// the same buffer for both receiving and transfer.	
	ACCESS(DMA2_S1M0AR) = (unsigned int)(dmaBuffer6);
	ACCESS(DMA2_S6M0AR) = (unsigned int)(dmaBuffer6);

	// Enable an interrupt to occur when the DMA operations complete (pg 192)
	ACCESS(DMA2_S1CR) |= (1 << 4);
	ACCESS(DMA2_S6CR) |= (1 << 4);

	// Enable the DMA reception stream (pg 192)
	ACCESS(DMA2_S1CR) |= 1;
}


void Uart6Init()
{
	Uart6GpioInit();

	Uart6DmaInit();

	// Give a clock USART6.  See pg 117.
	ACCESS(RCC_APB2ENR) |= (1 << 5);

	// Here we set the baud rate.  This is explained on 519 of ST RM0383.  The peripheral
	// clock is 16 MHz by default.  So, the calculation for the USARTDIV is:
	// DesiredBaudRate = 16MHz / 16*USARTDIV
	// Note that the fractional part of USARTDIV is represented with only 4 bits.  So
	// if we use 9600 this will result in a small error of 0.02% (see pg 522).  Therefore
	// the baud rate is actually 9,598.  Again, see pg 522.
	//ACCESS(USART1_BRR) |= 0x683;
	
	
	//Baud Rate = 115,200
	//ACCESS(USART6_BRR) = (4<<0) | (54<<4);
	
	//Baud Rate = 9,600
	ACCESS(USART6_BRR) = (7<<0) | (651<<4);

	// Enable USART6 for transmitting data.
	ACCESS(USART6_CR1) |= ((1 << 3) | (1 << 13));

	// See pg 200 of RM0383 ("STM32F411xC/E Reference Manual") for the interrupt
	// vector table.  You'll see USART6 is interrupt 71.
	// See pg 209 of PM0214 ("STM32F4 Series Programming Manual") for info on "ISER"
	// which is one of the "interrupt set-enable registers".  We enable interrupt
	// number 71.
	ACCESS(NVIC_ISER2) |= (1 << (71 - 64));
}


void QTSendFloat(char* values)
{
    // Convert the float to a string using a format that suits your needs
    //char floatString[20]; // Adjust the array size as needed
    //snprintf(floatString, sizeof(floatString), "%.2f", value);

    // Here we block until the previous transfer completes.
    while (uart6Busy == 1);
    uart6Busy = 1;

    // Copy the string into the DMA buffer and also calculate its length.
    int lengthOfString = 0;
    while (lengthOfString < MAX_BUFFER_SIZE && values[lengthOfString])
    {
        dmaBuffer6[lengthOfString] = values[lengthOfString];
        ++lengthOfString;
    }

    // See pg 192. This register holds the number of data items to transfer.
    ACCESS(DMA2_S6NDTR) = lengthOfString;

    // See pg 193. This register holds the peripheral data register. Since we're
    // using USART6, set it to its data register.
    ACCESS(DMA2_S6PAR) = USART6_DR;

    // See pg 193. This register holds the memory address of the data we want to transfer.
    ACCESS(DMA2_S6M0AR) = (unsigned int)dmaBuffer6;

    // Enable interrupt to occur upon completion of transfer
    ACCESS(DMA2_S6CR) |= (1 << 4);

    // Enable the stream
    ACCESS(DMA2_S6CR) |= 1;

    // Clear the transfer complete flag in the UART SR
    ACCESS(USART6_SR) &= ~(1 << 6);

    // Enable DMA transmission
    ACCESS(USART6_CR3) |= (1 << 7);
}

