#ifndef INC_LCD_CONFIG_H_
#define INC_LCD_CONFIG_H_

/* CONFIG FOR LIBRARY USER */
#define GPIO_PORT GPIOE

//4 pin mode -> pins
#define DATA4_Pin GPIO_PIN_7
#define DATA5_Pin GPIO_PIN_8
#define DATA6_Pin GPIO_PIN_9
#define DATA7_Pin GPIO_PIN_10

#define RS_Pin GPIO_PIN_11
#define E_Pin  GPIO_PIN_12
//RW Pin not used,connect to GND

//if you want to work with 8 bit mode uncomment the area which is given below


//#define LCD8Bit
//#define DATA0_Pin GPIO_PIN_3
//#define DATA1_Pin GPIO_PIN_4
//#define DATA2_Pin GPIO_PIN_5
//#define DATA3_Pin GPIO_PIN_6



#endif /* INC_LCD_CONFIG_H_ */
