/******************** (C) COPYRIGHT 2006 STMicroelectronics ********************
* File Name          : usb_endp.c
* Author             : MCD Application Team
* Date First Issued  : 10/01/2006 : V1.0
* Description        : Endpoint routines
********************************************************************************
* History:
* 10/01/2006 : V1.0
********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "91x_lib.h"
#include "uart1.h"
#include "usb_lib.h"
#include "usb_desc.h"
#include "usb.h"
#include "mkprotocol.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
u8 USB_Buffer[VIRTUAL_COM_PORT_DATA_SIZE];
u32 USB_BufferCount =0;
u32 count_in = 0;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : EP3_IN_Callback
* Description    :
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void EP3_OUT_Callback(void)
{
    u32 i;
    USB_BufferCount = GetEPRxCount(ENDP3);
    PMAToUserBufferCopy(USB_Buffer, ENDP3_RXADDR, USB_BufferCount );
    for(i=0; i<USB_BufferCount; i++)
    {
         MKProtocol_CollectSerialFrame(&USB_rx_buffer, USB_Buffer[i]);
    }
      SetEPRxValid(ENDP3);

      //sprintf(text,"USB (%ld): %s\n\r", USB_BufferRxCount, USB_BufferRx);
      //UART1_PutString(text);      
}
/*******************************************************************************
* Function Name  : EP1_IN_Callback
* Description    :
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void EP1_IN_Callback(void)
{
  count_in =0;
}
/******************* (C) COPYRIGHT 2006 STMicroelectronics *****END OF FILE****/

