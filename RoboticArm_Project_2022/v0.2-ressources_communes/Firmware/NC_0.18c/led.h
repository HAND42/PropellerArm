#ifndef _LED_H
#define _LED_H

#define LED_GRN_ON    GPIO_WriteBit(GPIO5, GPIO_Pin_6, Bit_SET)
#define LED_GRN_OFF GPIO_WriteBit(GPIO5, GPIO_Pin_6, Bit_RESET)
#define LED_GRN_TOGGLE     if (GPIO_ReadBit(GPIO5, GPIO_Pin_6)) LED_GRN_OFF; else LED_GRN_ON;

#define LED_RED_ON    GPIO_WriteBit(GPIO5, GPIO_Pin_7, Bit_SET)
#define LED_RED_OFF GPIO_WriteBit(GPIO5, GPIO_Pin_7, Bit_RESET)
#define LED_RED_TOGGLE     if (GPIO_ReadBit(GPIO5, GPIO_Pin_7)) LED_RED_OFF; else LED_RED_ON;

void Led_Init(void);

#endif //_LED_H


