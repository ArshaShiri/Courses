/*
 * stm32fxx.h
 *
 *  Created on: Jul 27, 2021
 *      Author: arsha
 */

#ifndef INC_STM32F407XX_H_
#define INC_STM32F407XX_H_

/*
 * base addresses of Flash and SRAM memories
 */

#define FLASH_BASE_ADDRESS								0x08000000U
#define S_RAM_1_BASE_ADDRESS							0x20000000U  // 112kb
#define S_RAM_2_BASE_ADDRESS							0x2001C000U
#define ROM												0x1FFF0000U  // System memory in reference manual
#define S_RAM											S_RAM_1_BASE_ADDRESS

/*
 * AHBx and APBx Bus Peripheral base addresses
 */

#define PERIPH_BASE_ADDRESS									0x40000000U
#define APB1_PERIPH_BASE_ADDRESS							PERIPH_BASE_ADDRESS
#define APB2_PERIPH_BASE_ADDRESS							0x40010000U
#define AHB1_PERIPH_BASE_ADDRESS							0x40020000U
#define AHB2_PERIPH_BASE_ADDRESS							0x50000000U

/*
 * Base addresses of peripherals which are hanging on AHB1 bus
 * TODO : Complete for all other peripherals
 */

#define GPIO_A_BASE_ADDRESS									(AHB1_PERIPH_BASE_ADDRESS + 0x0000)
#define GPIO_B_BASE_ADDRESS									(AHB1_PERIPH_BASE_ADDRESS + 0x0400)
#define GPIO_C_BASE_ADDRESS									(AHB1_PERIPH_BASE_ADDRESS + 0x0800)
#define GPIO_D_BASE_ADDRESS									(AHB1_PERIPH_BASE_ADDRESS + 0x0C00)
#define GPIO_E_BASE_ADDRESS									(AHB1_PERIPH_BASE_ADDRESS + 0x1000)
#define GPIO_F_BASE_ADDRESS									(AHB1_PERIPH_BASE_ADDRESS + 0x1400)
#define GPIO_G_BASE_ADDRESS									(AHB1_PERIPH_BASE_ADDRESS + 0x1800)
#define GPIO_H_BASE_ADDRESS									(AHB1_PERIPH_BASE_ADDRESS + 0x1C00)
#define GPIO_I_BASE_ADDRESS									(AHB1_PERIPH_BASE_ADDRESS + 0x2000)
#define RCC_BASE_ADDRESS									(AHB1_PERIPH_BASE_ADDRESS + 0x3800)

/*
 * Base addresses of peripherals which are hanging on APB1 bus
 * TODO : Complete for all other peripherals
 */

#define I2C_1_BASE_ADDRESS							(APB1_PERIPH_BASE_ADDRESS + 0x5400)
#define I2C_2_BASE_ADDRESS							(APB1_PERIPH_BASE_ADDRESS + 0x5800)
#define I2C_3_BASE_ADDRESS							(APB1_PERIPH_BASE_ADDRESS + 0x5C00)

#define SPI_2_BASE_ADDRESS							(APB1_PERIPH_BASE_ADDRESS + 0x3800)
#define SPI_3_BASE_ADDRESS							(APB1_PERIPH_BASE_ADDRESS + 0x3C00)

#define USART_2_BASE_ADDRESS						(APB1_PERIPH_BASE_ADDRESS + 0x4400)
#define USART_3_BASE_ADDRESS						(APB1_PERIPH_BASE_ADDRESS + 0x4800)

#define UART_4_BASE_ADDRESS							(APB1_PERIPH_BASE_ADDRESS + 0x4C00)
#define UART_5_BASE_ADDRESS							(APB1_PERIPH_BASE_ADDRESS + 0x5000)

/*
 * Base addresses of peripherals which are hanging on APB2 bus
 * TODO : Complete for all other peripherals
 */

#define EXTI_BASE_ADDRESS							(APB2_PERIPH_BASE_ADDRESS + 0x3C00)
#define SPI1_BASE_ADDRESS							(APB2_PERIPH_BASE_ADDRESS + 0x3000)
#define SYSCFG_BASE_ADDRESS	        				(APB2_PERIPH_BASE_ADDRESS + 0x3800)
#define USART1_BASE_ADDRESS							(APB2_PERIPH_BASE_ADDRESS + 0x1000)
#define USART6_BASE_ADDRESS							(APB2_PERIPH_BASE_ADDRESS + 0x1400)

#endif /* INC_STM32F407XX_H_ */