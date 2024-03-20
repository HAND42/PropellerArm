#include "91x_lib.h"
#include "led.h"

void Led_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    SCU_APBPeriphClockConfig(__GPIO5, ENABLE); // Enable the GPIO5 Clock      
      
    /*Configure LED_GRN at pin GPIO5.6 and LED_ROT at pin GPIO5.7*/
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Direction = GPIO_PinOutput;
    GPIO_InitStructure.GPIO_Pin =         GPIO_Pin_6 | GPIO_Pin_7; 
    GPIO_InitStructure.GPIO_Type =         GPIO_Type_PushPull ;
    GPIO_InitStructure.GPIO_Alternate = GPIO_OutputAlt1  ;
    GPIO_Init(GPIO5, &GPIO_InitStructure);
    
    LED_GRN_OFF;
    LED_RED_OFF;                                    
}



