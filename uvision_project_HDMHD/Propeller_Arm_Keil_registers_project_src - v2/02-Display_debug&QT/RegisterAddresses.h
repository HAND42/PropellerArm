// Addresses for the STM32F411.  See the RM0383 ST document ("Reference manual STM32F411xC/E
// advanced Arm-Based 32-bit MCUs") for details.
#pragma once

#define PWR_BASE_ADDRESS     0x40007000
#define PWR_CR               PWR_BASE_ADDRESS + 0x00  // PWR power control register

#define RCC_BASE_ADDRESS     0x40023800
#define RCC_CR               RCC_BASE_ADDRESS + 0x00  // RCC clock control register
#define RCC_PLLCFGR          RCC_BASE_ADDRESS + 0x04  // RCC PLL configuration register
#define RCC_CFGR             RCC_BASE_ADDRESS + 0x08  // RCC clock configuration register
#define RCC_AHB1ENR          RCC_BASE_ADDRESS + 0x30  // RCC AHB1 peripheral clock enable register (pg 116)
#define RCC_APB1ENR          RCC_BASE_ADDRESS + 0x40  // RCC APB1 peripheral clock enable register (pg 117)
#define RCC_APB2ENR          RCC_BASE_ADDRESS + 0x44  // RCC APB2 peripheral clock enable register (pg 120)

#define GPIOA_BASE_ADDRESS   0x40020000
#define GPIOA_MODER          GPIOA_BASE_ADDRESS + 0x00 // GPIO port mode register
#define GPIOA_OSPEEDR        GPIOA_BASE_ADDRESS + 0x08 // GPIO port output speed register
#define GPIOA_PUPR           GPIOA_BASE_ADDRESS + 0x0C // GPIO port pull-up/pull-down register
#define GPIOA_ODR            GPIOA_BASE_ADDRESS + 0x14 // GPIO port output data register
#define GPIOA_AFRL           GPIOA_BASE_ADDRESS + 0x20 // GPIO alternate function low register
#define GPIOA_AFRH           GPIOA_BASE_ADDRESS + 0x24 // GPIO alternate function high register

#define GPIOB_BASE_ADDRESS   0x40020400
#define GPIOB_MODER          GPIOB_BASE_ADDRESS + 0x00 // GPIO port mode register
#define GPIOB_OTYPER			  GPIOB_BASE_ADDRESS + 0x04 // GPIO port output type register
#define GPIOB_OSPEEDR        GPIOB_BASE_ADDRESS + 0x08 // GPIO port output speed register
#define GPIOB_PUPR           GPIOB_BASE_ADDRESS + 0x0C // GPIO port pull-up/pull-down register
#define GPIOB_ODR            GPIOB_BASE_ADDRESS + 0x14 // GPIO port output data register
#define GPIOB_AFRL           GPIOB_BASE_ADDRESS + 0x20 // GPIO alternate function low register
#define GPIOB_AFRH           GPIOB_BASE_ADDRESS + 0x24 // GPIO alternate function high register

#define GPIOC_BASE_ADDRESS   0x40020800
#define GPIOC_MODER          GPIOC_BASE_ADDRESS + 0x00 // GPIO port mode register
#define GPIOC_OTYPER			  GPIOC_BASE_ADDRESS + 0x04 // GPIO port output type register
#define GPIOC_OSPEEDR        GPIOC_BASE_ADDRESS + 0x08 // GPIO port output speed register
#define GPIOC_PUPR           GPIOC_BASE_ADDRESS + 0x0C // GPIO port pull-up/pull-down register
#define GPIOC_ODR            GPIOC_BASE_ADDRESS + 0x14 // GPIO port output data register
#define GPIOC_AFRL           GPIOC_BASE_ADDRESS + 0x20 // GPIO alternate function low register
#define GPIOC_AFRH           GPIOC_BASE_ADDRESS + 0x24 // GPIO alternate function high register

#define GPIOE_BASE_ADDRESS   0x40021000
#define GPIOE_MODER          GPIOE_BASE_ADDRESS + 0x00 // GPIO port mode register
#define GPIOE_OSPEEDR        GPIOE_BASE_ADDRESS + 0x08 // GPIO port output speed register
#define GPIOE_PUPR           GPIOE_BASE_ADDRESS + 0x0C // GPIO port pull-up/pull-down register
#define GPIOE_ODR            GPIOE_BASE_ADDRESS + 0x14 // GPIO port output data register
#define GPIOE_BSRR           GPIOE_BASE_ADDRESS + 0x18 // GPIO port bit set/reset register
#define GPIOE_AFRL           GPIOE_BASE_ADDRESS + 0x20 // GPIO alternate function low register
#define GPIOE_AFRH           GPIOE_BASE_ADDRESS + 0x24 // GPIO alternate function high register

#define SPI1_BASE_ADDRESS    0x40013000
#define SPI1_CR1             SPI1_BASE_ADDRESS + 0x00 // SPI control register 1
#define SPI1_SR              SPI1_BASE_ADDRESS + 0x08 // SPI status register
#define SPI1_DR              SPI1_BASE_ADDRESS + 0x0C // SPI data register
#define SPI1_BSRR            SPI1_BASE_ADDRESS + 0x18 // GPIO port bit set/reset register

#define I2C1_BASE_ADDRESS    0x40005400
#define I2C1_CR1             I2C1_BASE_ADDRESS + 0x00 // I2C Control register 1
#define I2C1_CR2             I2C1_BASE_ADDRESS + 0x04 // I2C Control register 2
#define I2C1_OAR1            I2C1_BASE_ADDRESS + 0x08 // I2C Own address register 1
#define I2C1_DR              I2C1_BASE_ADDRESS + 0x10 // I2C Data register
#define I2C1_SR1             I2C1_BASE_ADDRESS + 0x14 // I2C Status register 1
#define I2C1_SR2             I2C1_BASE_ADDRESS + 0x18 // I2C Status register 2
#define I2C1_CCR             I2C1_BASE_ADDRESS + 0x1C // I2C Clock control register
#define I2C1_TRISE           I2C1_BASE_ADDRESS + 0x20 // I2C TRISE register

#define USART1_BASE_ADDRESS  0x40011000
#define USART1_SR            USART1_BASE_ADDRESS + 0x00 // Status register
#define USART1_DR            USART1_BASE_ADDRESS + 0x04 // Data register
#define USART1_BRR           USART1_BASE_ADDRESS + 0x08 // Baud rate register
#define USART1_CR1           USART1_BASE_ADDRESS + 0x0C // Control register 1
#define USART1_CR3           USART1_BASE_ADDRESS + 0x14 // Control register 3

#define USART2_BASE_ADDRESS  0x40004400
#define USART2_SR            USART2_BASE_ADDRESS + 0x00 // Status register
#define USART2_DR            USART2_BASE_ADDRESS + 0x04 // Data register
#define USART2_BRR           USART2_BASE_ADDRESS + 0x08 // Baud rate register
#define USART2_CR1           USART2_BASE_ADDRESS + 0x0C // Control register 1
#define USART2_CR3           USART2_BASE_ADDRESS + 0x14 // Control register 3

#define USART6_BASE_ADDRESS  0x40011400
#define USART6_SR            USART6_BASE_ADDRESS + 0x00 // Status register
#define USART6_DR            USART6_BASE_ADDRESS + 0x04 // Data register
#define USART6_BRR           USART6_BASE_ADDRESS + 0x08 // Baud rate register
#define USART6_CR1           USART6_BASE_ADDRESS + 0x0C // Control register 1
#define USART6_CR3           USART6_BASE_ADDRESS + 0x14 // Control register 3

#define DMA1_BASE_ADDRESS           0x40026000
#define DMA1_HISR                   DMA1_BASE_ADDRESS + 0x04 // DMA high interrupt status register
#define DMA1_HIFCR                  DMA1_BASE_ADDRESS + 0x0C // DMA high interrupt flag clear register
#define DMA1_S4CR                   DMA1_BASE_ADDRESS + (0x10 + (4 * 0x18)) // DMA stream 4 configuration register
#define DMA1_S4NDTR                 DMA1_BASE_ADDRESS + (0x14 + (4 * 0x18)) // DMA stream 4 number of data register
#define DMA1_S4PAR                  DMA1_BASE_ADDRESS + (0x18 + (4 * 0x18)) // DMA stream 4 peripheral address register
#define DMA1_S4M0AR                 DMA1_BASE_ADDRESS + (0x1C + (4 * 0x18)) // DMA stream 4 memory 0 address register
#define DMA1_S4FCR                  DMA1_BASE_ADDRESS + (0x24 + (4 * 0x24)) // DMA stream 4 FIFO control register
#define DMA1_S5CR                   DMA1_BASE_ADDRESS + (0x10 + (5 * 0x18)) // DMA stream 5 configuration register
#define DMA1_S5NDTR                 DMA1_BASE_ADDRESS + (0x14 + (5 * 0x18)) // DMA stream 5 number of data register
#define DMA1_S5PAR                  DMA1_BASE_ADDRESS + (0x18 + (5 * 0x18)) // DMA stream 5 peripheral address register
#define DMA1_S5M0AR                 DMA1_BASE_ADDRESS + (0x1C + (5 * 0x18)) // DMA stream 5 memory 0 address register
#define DMA1_S5FCR                  DMA1_BASE_ADDRESS + (0x24 + (5 * 0x24)) // DMA stream 5 FIFO control register
#define DMA1_S6CR                   DMA1_BASE_ADDRESS + (0x10 + (6 * 0x18)) // DMA stream 6 configuration register
#define DMA1_S6NDTR                 DMA1_BASE_ADDRESS + (0x14 + (6 * 0x18)) // DMA stream 6 number of data register
#define DMA1_S6PAR                  DMA1_BASE_ADDRESS + (0x18 + (6 * 0x18)) // DMA stream 6 peripheral address register
#define DMA1_S6M0AR                 DMA1_BASE_ADDRESS + (0x1C + (6 * 0x18)) // DMA stream 6 memory 0 address register
#define DMA1_S6FCR                  DMA1_BASE_ADDRESS + (0x24 + (6 * 0x24)) // DMA stream 6 FIFO control register

#define DMA2_BASE_ADDRESS           0x40026400
#define DMA2_LISR                   DMA2_BASE_ADDRESS + 0x00 // DMA low interrupt status register
#define DMA2_HISR                   DMA2_BASE_ADDRESS + 0x04 // DMA high interrupt status register
#define DMA2_LIFCR                  DMA2_BASE_ADDRESS + 0x08 // DMA low interrupt flag clear register
#define DMA2_HIFCR                  DMA2_BASE_ADDRESS + 0x0C // DMA high interrupt flag clear register
#define DMA2_S1CR                   DMA2_BASE_ADDRESS + (0x10 + (1 * 0x18)) // DMA stream 1 configuration register
#define DMA2_S1NDTR                 DMA2_BASE_ADDRESS + (0x14 + (1 * 0x18)) // DMA stream 1 number of data register
#define DMA2_S1PAR                  DMA2_BASE_ADDRESS + (0x18 + (1 * 0x18)) // DMA stream 1 peripheral address register
#define DMA2_S1M0AR                 DMA2_BASE_ADDRESS + (0x1C + (1 * 0x18)) // DMA stream 1 memory 0 address register
#define DMA2_S1FCR                  DMA2_BASE_ADDRESS + (0x24 + (1 * 0x24)) // DMA stream 1 FIFO control register
#define DMA2_S6CR                   DMA2_BASE_ADDRESS + (0x10 + (6 * 0x18)) // DMA stream 6 configuration register
#define DMA2_S6NDTR                 DMA2_BASE_ADDRESS + (0x14 + (6 * 0x18)) // DMA stream 6 number of data register
#define DMA2_S6PAR                  DMA2_BASE_ADDRESS + (0x18 + (6 * 0x18)) // DMA stream 6 peripheral address register
#define DMA2_S6M0AR                 DMA2_BASE_ADDRESS + (0x1C + (6 * 0x18)) // DMA stream 6 memory 0 address register
#define DMA2_S6FCR                  DMA2_BASE_ADDRESS + (0x24 + (6 * 0x24)) // DMA stream 6 FIFO control register

#define NVIC_BASE_ADDRESS    0xE000E100  // See pg 218 of PM0214
#define NVIC_ISER0           NVIC_BASE_ADDRESS + 0x00  // "Interrupt set-enable registers" See pg 209 of PM0214
#define NVIC_ISER1           NVIC_BASE_ADDRESS + 0x04  // "Interrupt set-enable registers" See pg 209 of PM0214
#define NVIC_ISER2           NVIC_BASE_ADDRESS + 0x08  // "Interrupt set-enable registers" See pg 209 of PM0214

#define FLASH_BASE_ADDRESS   0x40023C00
#define FLASH_ACR            FLASH_BASE_ADDRESS + 0x00  // Flash access control register (pg 58)

#define ACCESS(address)      *((volatile unsigned int*)(address))
	
