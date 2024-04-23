/*#######################################################################################*/
/* !!! THIS IS NOT FREE SOFTWARE !!!                                                       */
/*#######################################################################################*/
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + Copyright (c) 2008 Ingo Busker, Holger Buss
// + Nur f�r den privaten Gebrauch / NON-COMMERCIAL USE ONLY
// + FOR NON COMMERCIAL USE ONLY
// + www.MikroKopter.com
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + Es gilt f�r das gesamte Projekt (Hardware, Software, Bin�rfiles, Sourcecode und Dokumentation),
// + dass eine Nutzung (auch auszugsweise) nur f�r den privaten (nicht-kommerziellen) Gebrauch zul�ssig ist.
// + Sollten direkte oder indirekte kommerzielle Absichten verfolgt werden, ist mit uns (info@mikrokopter.de) Kontakt
// + bzgl. der Nutzungsbedingungen aufzunehmen.
// + Eine kommerzielle Nutzung ist z.B.Verkauf von MikroKoptern, Best�ckung und Verkauf von Platinen oder Baus�tzen,
// + Verkauf von Luftbildaufnahmen, usw.
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + Werden Teile des Quellcodes (mit oder ohne Modifikation) weiterverwendet oder ver�ffentlicht,
// + unterliegen sie auch diesen Nutzungsbedingungen und diese Nutzungsbedingungen incl. Copyright m�ssen dann beiliegen
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + Sollte die Software (auch auszugesweise) oder sonstige Informationen des MikroKopter-Projekts
// + auf anderen Webseiten oder sonstigen Medien ver�ffentlicht werden, muss unsere Webseite "http://www.mikrokopter.de"
// + eindeutig als Ursprung verlinkt werden
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + Keine Gew�hr auf Fehlerfreiheit, Vollst�ndigkeit oder Funktion
// + Benutzung auf eigene Gefahr
// + Wir �bernehmen keinerlei Haftung f�r direkte oder indirekte Personen- oder Sachsch�den
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + Die Portierung oder Nutzung der Software (oder Teile davon) auf andere Systeme (ausser der Hardware von www.mikrokopter.de) ist nur
// + mit unserer Zustimmung zul�ssig
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + Die Funktion printf_P() unterliegt ihrer eigenen Lizenz und ist hiervon nicht betroffen
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + Redistributions of source code (with or without modifications) must retain the above copyright notice,
// + this list of conditions and the following disclaimer.
// +   * Neither the name of the copyright holders nor the names of contributors may be used to endorse or promote products derived
// +     from this software without specific prior written permission.
// +   * The use of this project (hardware, software, binary files, sources and documentation) is only permitted
// +     for non-commercial use (directly or indirectly)
// +     Commercial use (for excample: selling of MikroKopters, selling of PCBs, assembly, ...) is only permitted
// +     with our written permission
// +   * If sources or documentations are redistributet on other webpages, out webpage (http://www.MikroKopter.de) must be
// +     clearly linked as origin
// +   * porting the sources to other systems or using the software on other systems (except hardware from www.mikrokopter.de) is not allowed
//
// +  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// +  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// +  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// +  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// +  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// +  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// +  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// +  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// +  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// +  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// +  POSSIBILITY OF SUCH DAMAGE.
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include <string.h>
#include "91x_lib.h"
#include "i2c.h"
#include "uart1.h"
#include "timer1.h"
#include "config.h"
#include "main.h"
#include "led.h"
#include "spi_slave.h"

#define I2C_SLAVE_ADDRESS         0x50

// I2C states
#define    I2C_UNDEF        0
#define    I2C_IDLE        1
#define I2C_TX_PROGRESS 2
#define    I2C_RX_PENDING    3
#define I2C_RX_PROGRESS    4
#define I2C_OFF            5

volatile u8 I2C_State = I2C_OFF; // only on byte! because of sync by nesting irqs
u8 I2C_StopPolling = 1;

// rxbuffer
volatile u8 I2C_RxBufferSize;
volatile u8 *I2C_RxBuffer;
// txbuffer
volatile u8 I2C_TxBufferSize;
volatile u8 *I2C_TxBuffer;

volatile u8 I2C_Direction;
volatile u8 I2C_Command;
// I2C Transfer buffers
volatile I2C_Heading_t            I2C_Heading;
volatile I2C_WriteAttitude_t    I2C_WriteAttitude;
volatile I2C_Mag_t                I2C_Mag;
volatile I2C_Version_t            MK3MAG_Version;
volatile I2C_Cal_t                I2C_WriteCal;
volatile I2C_Cal_t                I2C_ReadCal;

#define I2C1_TIMEOUT 500 // 500 ms
volatile u32 I2C1_Timeout = 0;

//--------------------------------------------------------------
void I2C1_Init(void)
{
    I2C_InitTypeDef   I2C_Struct;
    GPIO_InitTypeDef  GPIO_InitStructure;
    
    I2C_State = I2C_OFF;

    UART1_PutString("\r\n I2C init...");
    // enable Port 2 peripherie
    SCU_APBPeriphClockConfig(__GPIO2, ENABLE);
    // disable a reset state
    SCU_APBPeriphReset(__GPIO2, DISABLE);

    // free a busy bus

    // At switch on I2C devices can get in a state where they
    // are still waiting for a command due to all the bus lines bouncing
    // around at startup have started clocking data into the device(s).
    // Enable the ports as open collector port outputs
    // and clock out at least 9 SCL pulses, then generate a stop
    // condition and then leave the clock line high.

    // configure P2.2->I2C1_CLKOUT and P2.3->I2C1_DOUT to normal port operation
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Direction = GPIO_PinOutput;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Type = GPIO_Type_OpenCollector;
    GPIO_InitStructure.GPIO_IPInputConnected = GPIO_IPInputConnected_Disable;
    GPIO_InitStructure.GPIO_Alternate = GPIO_OutputAlt1;
    GPIO_Init(GPIO2, &GPIO_InitStructure);

    u8 i;
    u32 delay;
    // set SCL high and then SDA to low (start condition)
    GPIO_WriteBit(GPIO2, GPIO_Pin_2, Bit_SET);
    delay = SetDelay(1);
    while (!CheckDelay(delay));
    GPIO_WriteBit(GPIO2, GPIO_Pin_3, Bit_RESET);
    // toggle SCL at least 10 times from high to low to high
    for(i = 0; i < 10; i++)
    {
        delay = SetDelay(1);
        while (!CheckDelay(delay));

        GPIO_WriteBit(GPIO2, GPIO_Pin_2, Bit_RESET);
        delay = SetDelay(1);
        while (!CheckDelay(delay));
        GPIO_WriteBit(GPIO2, GPIO_Pin_2, Bit_SET);
    }
    delay = SetDelay(1);
    while (!CheckDelay(delay));
    // create stop condition setting SDA HIGH when SCL is HIGH
    GPIO_WriteBit(GPIO2, GPIO_Pin_3, Bit_SET);


    // reconfigure P2.2->I2C1_CLKOUT and P2.3->I2C1_DOUT
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Direction = GPIO_PinOutput;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Type = GPIO_Type_OpenCollector;
    GPIO_InitStructure.GPIO_IPInputConnected = GPIO_IPInputConnected_Enable;
    GPIO_InitStructure.GPIO_Alternate = GPIO_OutputAlt2; //I2C1_CLKOUT, I2C1_DOUT
    GPIO_Init(GPIO2, &GPIO_InitStructure);

    // enable I2C peripherie
    SCU_APBPeriphClockConfig(__I2C1,ENABLE);
    // reset I2C peripherie
    SCU_APBPeriphReset(__I2C1,ENABLE);
    SCU_APBPeriphReset(__I2C1,DISABLE);

    I2C_DeInit(I2C1);
    I2C_StructInit(&I2C_Struct);
    I2C_Struct.I2C_GeneralCall = I2C_GeneralCall_Disable;
    I2C_Struct.I2C_Ack = I2C_Ack_Enable;
    I2C_Struct.I2C_CLKSpeed = I2C1_CLOCK;
    I2C_Struct.I2C_OwnAddress = 0x00;
    I2C_Init(I2C1, &I2C_Struct);

    I2C_TxBuffer = NULL;
    I2C_TxBufferSize = 0;

    I2C_RxBuffer = NULL;
    I2C_RxBufferSize = 0;

    I2C_Cmd(I2C1, ENABLE);
    I2C_ITConfig(I2C1, ENABLE);

    VIC_Config(I2C1_ITLine, VIC_IRQ , PRIORITY_I2C1);

    I2C1_Timeout = SetDelay(10*I2C1_TIMEOUT);
    I2C_Heading.Heading = -1;
    I2C_GenerateSTOP(I2C1, ENABLE);
    I2C_State = I2C_IDLE;

    I2C_StopPolling = 0; // start polling

    UART1_PutString("ok");
}


//--------------------------------------------------------------
void I2C1_Deinit(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    I2C_StopPolling = 1;// stop polling
    UART1_PutString("\r\n I2C deinit...");
    I2C_GenerateStart(I2C1, DISABLE);
    I2C_GenerateSTOP(I2C1, ENABLE);
    VIC_ITCmd(I2C1_ITLine, DISABLE);
    I2C_State = I2C_OFF;
    I2C_ITConfig(I2C1, DISABLE);
    I2C_Cmd(I2C1, DISABLE);
    I2C_DeInit(I2C1);
    SCU_APBPeriphClockConfig(__I2C1, DISABLE);

    // set ports to input
    SCU_APBPeriphClockConfig(__GPIO2, ENABLE);
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Direction =     GPIO_PinInput;
    GPIO_InitStructure.GPIO_Pin =             GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Type =             GPIO_Type_PushPull;
    GPIO_InitStructure.GPIO_IPInputConnected =     GPIO_IPInputConnected_Disable;
    GPIO_InitStructure.GPIO_Alternate =     GPIO_InputAlt1;
    GPIO_Init(GPIO2, &GPIO_InitStructure);

    I2C_TxBuffer = NULL;
    I2C_TxBufferSize = 0;

    I2C_RxBuffer = NULL;
    I2C_RxBufferSize = 0;

    I2C1_Timeout = SetDelay(10*I2C1_TIMEOUT);
    I2C_Heading.Heading = -1;

    UART1_PutString("ok");
}


//--------------------------------------------------------------
void I2C1_IRQHandler(void)
{
    static u8 Rx_Idx = 0, Tx_Idx = 0, crc = 0;
    static u8 I2C_PrimRxBuffer[10]; // must be larger than any of the secondary rx buffers
    u16 status;

    //IENABLE;  // do not enable IRQ nesting for I2C!!!!

    // detemine I2C State
    status = I2C_GetLastEvent(I2C1);

    if(status & (I2C_FLAG_AF|I2C_FLAG_BERR))  // if an acknowledge failure or bus error occured
    {    // Set and subsequently clear the STOP bit while BTF is set.
        while(I2C_GetFlagStatus (I2C1, I2C_FLAG_BTF) != RESET)
        {
            I2C_GenerateSTOP (I2C1, ENABLE);  // free the bus
            I2C_GenerateSTOP (I2C1, DISABLE); // free the bus
        }
        I2C_State = I2C_IDLE;
        VIC_ITCmd(I2C1_ITLine, DISABLE);
        LED_GRN_OFF;
        return;
    }
    else
    {    // depending on current i2c state
        switch (status)
        {
            // the start condition was initiated on the bus
            case I2C_EVENT_MASTER_MODE_SELECT:
                LED_GRN_ON;
                // update current bus state variable
                switch(I2C_Direction)
                {
                    case I2C_MODE_TRANSMITTER:
                        I2C_State = I2C_TX_PROGRESS;
                        break;

                    case I2C_MODE_RECEIVER:
                        if ((I2C_RxBuffer == NULL) || (I2C_RxBufferSize == 0))
                        {
                            I2C_GenerateSTOP (I2C1, ENABLE);
                            VIC_ITCmd(I2C1_ITLine, DISABLE);
                            LED_GRN_OFF;
                            I2C_State = I2C_IDLE;
                            return;
                        }
                        else
                        {
                            I2C_State = I2C_RX_PROGRESS;
                        }
                        break;

                    default: // invalid direction
                        I2C_GenerateSTOP (I2C1, ENABLE);
                        VIC_ITCmd(I2C1_ITLine, DISABLE);
                        LED_GRN_OFF;
                        I2C_State = I2C_IDLE;
                        return;
                }
                // enable acknowledge
                I2C_AcknowledgeConfig (I2C1, ENABLE);
                // send address/direction byte on the bus
                I2C_Send7bitAddress(I2C1, I2C_SLAVE_ADDRESS, I2C_Direction);
                break;

            // the address byte was send
            case I2C_EVENT_MASTER_MODE_SELECTED:
                // Clear EV6 by set again the PE bit
                I2C_Cmd(I2C1, ENABLE);
                // reset checksum
                crc = 0;
                switch(I2C_State)
                {
                    case I2C_TX_PROGRESS:
                    // send command 1st data byte (allways the command id)
                    I2C_SendData(I2C1, I2C_Command);
                    crc += I2C_Command;
                    Tx_Idx = 0;
                    // reset timeout
                    I2C1_Timeout = SetDelay(I2C1_TIMEOUT); // after inactivity the I2C1 bus will be reset
                    break;

                    case I2C_RX_PROGRESS:
                    Rx_Idx = 0;
                    break;

                    default: // unknown I2C state
                    // should never happen
                    I2C_GenerateSTOP (I2C1, ENABLE);
                    LED_GRN_OFF;
                    VIC_ITCmd(I2C1_ITLine, DISABLE);
                    I2C_State = I2C_IDLE;
                    return;
                    break;
                }
                break;

            // the master has transmitted a byte and slave has been acknowledged
            case I2C_EVENT_MASTER_BYTE_TRANSMITTED:

                // some bytes have to be transmitted
                if(Tx_Idx < I2C_TxBufferSize)
                {
                    if(I2C_TxBuffer != NULL)
                    {
                        I2C_SendData(I2C1, I2C_TxBuffer[Tx_Idx]);
                           crc += I2C_TxBuffer[Tx_Idx];
                    }
                    else
                    {
                        I2C_SendData(I2C1, 0x00);
                    }
                }
                else if(Tx_Idx == I2C_TxBufferSize) // the last tx buffer byte was send
                {
                    // send crc byte at the end
                    crc = ~crc; // flip all bits in the checksum
                    I2C_SendData(I2C1, crc);
                }
                else if(Tx_Idx == (I2C_TxBufferSize+1) )
                {
                    I2C_SendData(I2C1, 0xAA); // send a dummybyte
                }
                else // last byte was send
                {
                    // generate stop or repeated start condition
                    if ((I2C_RxBuffer != NULL) && (I2C_RxBufferSize > 0)) // is any answer byte expected?
                    {
                        I2C_Direction = I2C_MODE_RECEIVER; // switch to master receiver after repeated start condition
                        I2C_GenerateStart(I2C1, ENABLE);   // initiate repeated start condition on the bus
                    }
                    else
                    {   // stop communication
                        I2C_GenerateSTOP(I2C1, ENABLE);    // generate stop condition to free the bus
                        VIC_ITCmd(I2C1_ITLine, DISABLE);
                        LED_GRN_OFF;
                        DebugOut.Analog[15]++;
                        I2C_State = I2C_IDLE;            // ready for new actions
                        
                    }
                }
                Tx_Idx++;
                break;

            // the master has received a byte from the slave
            case I2C_EVENT_MASTER_BYTE_RECEIVED:
                // some bytes have to be received
                if (Rx_Idx < I2C_RxBufferSize)
                {     // copy received byte  from the data register to the rx-buffer
                    I2C_PrimRxBuffer[Rx_Idx] = I2C_ReceiveData(I2C1);
                    // update checksum
                    crc += I2C_PrimRxBuffer[Rx_Idx];
                }
                // if the last byte (crc) was received
                else if ( Rx_Idx == I2C_RxBufferSize)
                {
                    // generate a STOP condition on the bus before reading data register
                    I2C_GenerateSTOP(I2C1, ENABLE);
                    // compare last byte with checksum
                    crc = ~crc;// flip all bits in calulated checksum
                    if(crc == I2C_ReceiveData(I2C1))
                    {    
                        // copy primary rx buffer content to rx buffer if exist
                        if(I2C_RxBuffer != NULL)
                        {
                            memcpy((u8 *)I2C_RxBuffer, (u8 *)I2C_PrimRxBuffer, I2C_RxBufferSize);
                        }
                        I2C1_Timeout = SetDelay(I2C1_TIMEOUT);
                        DebugOut.Analog[15]++;
                    }
                    else // checksum error detected
                    {
                        DebugOut.Analog[14]++;
                    }
                    VIC_ITCmd(I2C1_ITLine, DISABLE);
                    LED_GRN_OFF;
                    I2C_State = I2C_IDLE;
                    return;
                }
                Rx_Idx++;
                // if the 2nd last byte was received disable acknowledge for the last one
                if ( Rx_Idx == I2C_RxBufferSize )
                {
                    I2C_AcknowledgeConfig (I2C1, DISABLE);
                }
                break;

            default:
                break;
        }
    }

    //IDISABLE;     // do not enable IRQ nesting for I2C!!!!
}
//----------------------------------------------------------------
void I2C1_SendCommand(u8 command)
{
    // disable I2C IRQ to check state
    VIC_ITCmd(I2C1_ITLine, DISABLE);
    // If I2C transmission is in progress
    if(I2C_State == I2C_IDLE)
    {
        // update current command id
          I2C_Command = command;
        // set pointers to data area with respect to the command id
        switch (command)
        {
            case I2C_CMD_VERSION:
                I2C_RxBuffer = (u8 *)&MK3MAG_Version;
                I2C_RxBufferSize = sizeof(MK3MAG_Version);
                I2C_TxBuffer = NULL;
                I2C_TxBufferSize = 0;
                break;
            case I2C_CMD_WRITE_CAL:
                I2C_RxBuffer = (u8 *)&I2C_ReadCal;
                I2C_RxBufferSize = sizeof(I2C_ReadCal);
                I2C_TxBuffer = (u8 *)&I2C_WriteCal;
                I2C_TxBufferSize = sizeof(I2C_WriteCal);
                break;
            case I2C_CMD_READ_MAG:
                I2C_RxBuffer = (u8 *)&I2C_Mag;
                I2C_RxBufferSize = sizeof(I2C_Mag);
                I2C_TxBuffer = NULL;
                I2C_TxBufferSize = 0;
                break;
            case I2C_CMD_READ_HEADING:
                I2C_RxBuffer = (u8 *)&I2C_Heading;
                I2C_RxBufferSize = sizeof(I2C_Heading);
                I2C_TxBuffer =  (u8 *)&I2C_WriteAttitude;
                I2C_TxBufferSize = sizeof(I2C_WriteAttitude);
                // update attitude from spi rx buffer
                VIC_ITCmd(SSP0_ITLine, DISABLE); // avoid spi buffer update during copy
                I2C_WriteAttitude.Roll = FromFlightCtrl.AngleRoll;
                I2C_WriteAttitude.Nick = FromFlightCtrl.AngleNick;
                VIC_ITCmd(SSP0_ITLine, ENABLE);
                break;
            default: // unknown command id
                I2C_RxBuffer = NULL;
                I2C_RxBufferSize = 0;
                I2C_TxBuffer =  NULL;
                I2C_TxBufferSize = 0;
                break;
        }
        // set direction to master transmitter
        I2C_Direction = I2C_MODE_TRANSMITTER;
        // test on busy flag and clear it
        I2C_CheckEvent( I2C1, I2C_FLAG_BUSY );
        // enable I2C IRQ again
        VIC_ITCmd(I2C1_ITLine, ENABLE);
        // initiate start condition on the bus
        I2C_GenerateStart(I2C1, ENABLE);
        // to be continued in the I2C1_IRQHandler() above
    } // EOF I2C_State == I2C_IDLE
    else // I2C_State != I2C_IDLE
    {
        // re-enable I2C IRQ again
        VIC_ITCmd(I2C1_ITLine, ENABLE);
    }    
}

//----------------------------------------------------------------
void I2C1_GetMK3MagVersion(void)
{
    u8 msg[64];
    u8 repeat;
    u32 timeout;
    
    UART1_PutString("\r\n Getting Version from MK3MAG");
    // stop polling of other commands
    I2C_StopPolling = 1;

    MK3MAG_Version.Major = 0xFF;
    MK3MAG_Version.Minor = 0xFF;
    MK3MAG_Version.Patch = 0xFF;
    MK3MAG_Version.Compatible = 0xFF;
    // polling of version info
    repeat = 0;
    do
    {
        I2C1_SendCommand(I2C_CMD_VERSION);
        timeout = SetDelay(250); 
           do
        {
            if (MK3MAG_Version.Major != 0xFF) break; // break loop on success
        }while (!CheckDelay(timeout));
        UART1_PutString(".");
        repeat++;
    }while ((MK3MAG_Version.Major == 0xFF) && (repeat < 12)); // 12*250ms=3s
    // if we got it
    if (MK3MAG_Version.Major != 0xFF)
    {
        sprintf(msg, "\r\n MK3MAG V%d.%d%c", MK3MAG_Version.Major, MK3MAG_Version.Minor, 'a' + MK3MAG_Version.Patch);
        UART1_PutString(msg);
        sprintf(msg, " Compatible: %d", MK3MAG_Version.Compatible);
        UART1_PutString(msg);
    }
    else UART1_PutString("\n\r No version information from MK3Mag.");

    I2C_StopPolling = 0; // enable polling of heading command
}


//----------------------------------------------------------------
void I2C1_UpdateCompass(void)
{
    static u32 TimerCompassUpdate = 0;

    if( (I2C_State == I2C_OFF) || I2C_StopPolling ) return;
    
    if(CheckDelay(TimerCompassUpdate))
    {
        // check for incomming compass calibration request
        // update CalByte from spi input queue
        fifo_get(&CompassCalcStateFiFo, (u8 *)&(I2C_WriteCal.CalByte));
        // send new calstate
        if(I2C_ReadCal.CalByte != I2C_WriteCal.CalByte)
        {
            I2C1_SendCommand(I2C_CMD_WRITE_CAL);
        }
        else // request current heading
        {
            I2C1_SendCommand(I2C_CMD_READ_HEADING);
        }
        TimerCompassUpdate = SetDelay(20);    // every 20 ms are 50 Hz
    }
}
