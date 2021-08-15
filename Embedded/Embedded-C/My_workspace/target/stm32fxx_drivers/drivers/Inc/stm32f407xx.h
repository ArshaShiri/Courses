/*
 * stm32fxx.h
 *
 *  Created on: Jul 27, 2021
 *      Author: arsha
 */

#include <stdint.h>

#ifndef INC_STM32F407XX_H_
#define INC_STM32F407XX_H_

/**********************************START:Processor Specific Details **********************************/
/*
 * ARM Cortex Mx Processor NVIC ISERx register Addresses
 */
// Interrupt Set-enable Register
#define NVIC_ISER0          ( (volatile uint32_t*)0xE000E100 )
#define NVIC_ISER1          ( (volatile uint32_t*)0xE000E104 )
#define NVIC_ISER2          ( (volatile uint32_t*)0xE000E108 )
#define NVIC_ISER3          ( (volatile uint32_t*)0xE000E10c )

/*
 * ARM Cortex Mx Processor Priority Register Address Calculation
 */
#define NVIC_PR_BASE_ADDRESS 	((volatile uint32_t*)0xE000E400)

/*
 * ARM Cortex Mx Processor number of priority bits implemented in Priority Register
 */
#define NO_PR_BITS_IMPLEMENTED  4

/*
 * ARM Cortex Mx Processor NVIC ICERx register Addresses
 */
// Interrupt Clear-enable Register
#define NVIC_ICER0 			((volatile uint32_t*)0XE000E180)
#define NVIC_ICER1			((volatile uint32_t*)0XE000E184)
#define NVIC_ICER2  		((volatile uint32_t*)0XE000E188)
#define NVIC_ICER3			((volatile uint32_t*)0XE000E18C)

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

/**********************************peripheral register definition structures **********************************/

/*
 * Note : Registers of a peripheral are specific to MCU
 * e.g : Number of Registers of SPI peripheral of STM32F4x family of MCUs may be different(more or less)
 * Compared to number of registers of SPI peripheral of STM32Lx or STM32F0x family of MCUs
 * Please check your Device RM
 */

typedef struct
{
	volatile uint32_t MODER;             /*! Address offset: 0x00 */
	volatile uint32_t OTYPER;            /*! Address offset: 0x04 */
	volatile uint32_t OSPEEDR;
	volatile uint32_t PUPDR;
	volatile uint32_t IDR;
	volatile uint32_t ODR;
	volatile uint32_t BSRR;
	volatile uint32_t LCKR;
	volatile uint32_t AFR[2];			/*!< AFR[0] : GPIO alternate function low register, AF[1] : GPIO alternate function high register    		Address offset: 0x20-0x24 */
} GPIO_RegisterDefinition_t;

/*
 * peripheral register definition structure for RCC
 */
typedef struct
{
  volatile uint32_t CR;            /*!< TODO,     										Address offset: 0x00 */
  volatile uint32_t PLLCFGR;       /*!< TODO,     										Address offset: 0x04 */
  volatile uint32_t CFGR;          /*!< TODO,     										Address offset: 0x08 */
  volatile uint32_t CIR;           /*!< TODO,     										Address offset: 0x0C */
  volatile uint32_t AHB1RSTR;      /*!< TODO,     										Address offset: 0x10 */
  volatile uint32_t AHB2RSTR;      /*!< TODO,     										Address offset: 0x14 */
  volatile uint32_t AHB3RSTR;      /*!< TODO,     										Address offset: 0x18 */
  uint32_t      RESERVED0;     	   /*!< Reserved, 0x1C                                                       */
  volatile uint32_t APB1RSTR;      /*!< TODO,     										Address offset: 0x20 */
  volatile uint32_t APB2RSTR;      /*!< TODO,     										Address offset: 0x24 */
  uint32_t      RESERVED1[2];      /*!< Reserved, 0x28-0x2C                                                  */
  volatile uint32_t AHB1ENR;       /*!< TODO,     										Address offset: 0x30 */
  volatile uint32_t AHB2ENR;       /*!< TODO,     										Address offset: 0x34 */
  volatile uint32_t AHB3ENR;       /*!< TODO,     										Address offset: 0x38 */
  uint32_t      RESERVED2;         /*!< Reserved, 0x3C                                                       */
  volatile uint32_t APB1ENR;       /*!< TODO,     										Address offset: 0x40 */
  volatile uint32_t APB2ENR;       /*!< TODO,     										Address offset: 0x44 */
  uint32_t      RESERVED3[2];      /*!< Reserved, 0x48-0x4C                                                  */
  volatile uint32_t AHB1LPENR;     /*!< TODO,     										Address offset: 0x50 */
  volatile uint32_t AHB2LPENR;     /*!< TODO,     										Address offset: 0x54 */
  volatile uint32_t AHB3LPENR;     /*!< TODO,     										Address offset: 0x58 */
  uint32_t      RESERVED4;         /*!< Reserved, 0x5C                                                       */
  volatile uint32_t APB1LPENR;     /*!< TODO,     										Address offset: 0x60 */
  volatile uint32_t APB2LPENR;     /*!< RTODO,     										Address offset: 0x64 */
  uint32_t      RESERVED5[2];      /*!< Reserved, 0x68-0x6C                                                  */
  volatile uint32_t BDCR;          /*!< TODO,     										Address offset: 0x70 */
  volatile uint32_t CSR;           /*!< TODO,     										Address offset: 0x74 */
  uint32_t      RESERVED6[2];      /*!< Reserved, 0x78-0x7C                                                  */
  volatile uint32_t SSCGR;         /*!< TODO,     										Address offset: 0x80 */
  volatile uint32_t PLLI2SCFGR;    /*!< TODO,     										Address offset: 0x84 */
  volatile uint32_t PLLSAICFGR;    /*!< TODO,     										Address offset: 0x88 */
  volatile uint32_t DCKCFGR;       /*!< TODO,     										Address offset: 0x8C */
  volatile uint32_t CKGATENR;      /*!< TODO,     										Address offset: 0x90 */
  volatile uint32_t DCKCFGR2;      /*!< TODO,     										Address offset: 0x94 */

} RCC_RRegisterDefinition_t;

/*
 * peripheral register definition structure for EXTI
 */
typedef struct
{
	volatile uint32_t IMR;    /*!< Give a short description,          	  	    Address offset: 0x00 */
	volatile uint32_t EMR;    /*!< TODO,                						Address offset: 0x04 */
	volatile uint32_t RTSR;   /*!< TODO,  									     Address offset: 0x08 */
	volatile uint32_t FTSR;   /*!< TODO, 										Address offset: 0x0C */
	volatile uint32_t SWIER;  /*!< TODO,  									   Address offset: 0x10 */
	volatile uint32_t PR;     /*!< TODO,                   					   Address offset: 0x14 */

}EXTI_RegDef_t;

/*
 * peripheral register definition structure for SYSCFG
 */
typedef struct
{
	volatile uint32_t MEMRMP;       /*!< Give a short description,                    Address offset: 0x00      */
	volatile uint32_t PMC;          /*!< TODO,     									  Address offset: 0x04      */
	volatile uint32_t EXTICR[4];    /*!< TODO , 									  Address offset: 0x08-0x14 */
	uint32_t      RESERVED1[2];     /*!< TODO          							      Reserved, 0x18-0x1C    	*/
	volatile uint32_t CMPCR;        /*!< TODO         								  Address offset: 0x20      */
	uint32_t      RESERVED2[2];     /*!<                                              Reserved, 0x24-0x28 	    */
	volatile uint32_t CFGR;         /*!< TODO                                         Address offset: 0x2C   	*/
} SYSCFG_RegDef_t;


/*
 * peripheral definitions ( Peripheral base addresses typecasted to xxx_RegisterDefinition_t)
 */

#define GPIOA  				((GPIO_RegisterDefinition_t*)GPIO_A_BASE_ADDRESS)
#define GPIOB  				((GPIO_RegisterDefinition_t*)GPIO_B_BASE_ADDRESS)
#define GPIOC  				((GPIO_RegisterDefinition_t*)GPIO_C_BASE_ADDRESS)
#define GPIOD  				((GPIO_RegisterDefinition_t*)GPIO_D_BASE_ADDRESS)
#define GPIOE  				((GPIO_RegisterDefinition_t*)GPIO_E_BASE_ADDRESS)
#define GPIOF  				((GPIO_RegisterDefinition_t*)GPIO_F_BASE_ADDRESS)
#define GPIOG  				((GPIO_RegisterDefinition_t*)GPIO_G_BASE_ADDRESS)
#define GPIOH  				((GPIO_RegisterDefinition_t*)GPIO_H_BASE_ADDRESS)
#define GPIOI  				((GPIO_RegisterDefinition_t*)GPIO_I_BASE_ADDRESS)

#define RCC 				((RCC_RRegisterDefinition_t*)RCC_BASE_ADDRESS)
#define EXTI				((EXTI_RegDef_t*)EXTI_BASE_ADDRESS)
#define SYSCFG				((SYSCFG_RegDef_t*)SYSCFG_BASE_ADDRESS)
/*
 * Clock Enable Macros for GPIOx peripherals
 */

#define GPIOA_PERIPHERAL_CLOCK_ENABLE()    	(RCC->AHB1ENR |= (1 << 0))
#define GPIOB_PERIPHERAL_CLOCK_ENABLE()		(RCC->AHB1ENR |= (1 << 1))
#define GPIOC_PERIPHERAL_CLOCK_ENABLE()		(RCC->AHB1ENR |= (1 << 2))
#define GPIOD_PERIPHERAL_CLOCK_ENABLE()		(RCC->AHB1ENR |= (1 << 3))
#define GPIOE_PERIPHERAL_CLOCK_ENABLE()		(RCC->AHB1ENR |= (1 << 4))
#define GPIOF_PERIPHERAL_CLOCK_ENABLE()		(RCC->AHB1ENR |= (1 << 5))
#define GPIOG_PERIPHERAL_CLOCK_ENABLE()		(RCC->AHB1ENR |= (1 << 6))
#define GPIOH_PERIPHERAL_CLOCK_ENABLE()		(RCC->AHB1ENR |= (1 << 7))
#define GPIOI_PERIPHERAL_CLOCK_ENABLE()		(RCC->AHB1ENR |= (1 << 8))

/*
 * Clock Enable Macros for I2Cx peripherals
 */
#define I2C1_PERIPHERAL_CLOCK_ENABLE() (RCC->APB1ENR |= (1 << 21))
#define I2C2_PERIPHERAL_CLOCK_ENABLE() (RCC->APB1ENR |= (1 << 22))
#define I2C3_PERIPHERAL_CLOCK_ENABLE() (RCC->APB1ENR |= (1 << 23))


/*
 * Clock Enable Macros for SPIx peripheralsbu
 */
#define SPI1_PERIPHERAL_CLOCK_ENABLE() (RCC->APB2ENR |= (1 << 12))
#define SPI2_PERIPHERAL_CLOCK_ENABLE() (RCC->APB1ENR |= (1 << 14))
#define SPI3_PERIPHERAL_CLOCK_ENABLE() (RCC->APB1ENR |= (1 << 15))
#define SPI4_PERIPHERAL_CLOCK_ENABLE() (RCC->APB2ENR |= (1 << 13))

/*
 * Clock Enable Macros for USARTx peripherals
 */
#define USART1_PCCK_EN() (RCC->APB2ENR |= (1 << 4))
#define USART2_PCCK_EN() (RCC->APB1ENR |= (1 << 17))
#define USART3_PCCK_EN() (RCC->APB1ENR |= (1 << 18))
#define UART4_PCCK_EN()  (RCC->APB1ENR |= (1 << 19))
#define UART5_PCCK_EN()  (RCC->APB1ENR |= (1 << 20))
#define USART6_PCCK_EN() (RCC->APB1ENR |= (1 << 5))

/*
 * Clock Enable Macros for SYSCFG peripheral
 */

#define SYSCFG_PCLK_EN() (RCC->APB2ENR |= (1 << 14))


/*
 * Clock Disable Macros for GPIOx peripherals
 */

#define GPIOA_PERIPHERAL_DISABLE()		(RCC->AHB1ENR &= ~(1 << 0))

/*
 * Clock Disable Macros for SPIx peripherals
 */

/*
 * Clock Disable Macros for USARTx peripherals
 */


/*
 * Clock Disable Macros for SYSCFG peripheral
 */

/*
 *  Macros to reset GPIOx peripherals
 */
// Set to one and then set back to zero to reset.
// To do while is a trick to have two statements in one macro.
#define GPIOA_REG_RESET()               do{ (RCC->AHB1RSTR |= (1 << 0)); (RCC->AHB1RSTR &= ~(1 << 0)); }while(0)
#define GPIOB_REG_RESET()               do{ (RCC->AHB1RSTR |= (1 << 1)); (RCC->AHB1RSTR &= ~(1 << 1)); }while(0)
#define GPIOC_REG_RESET()               do{ (RCC->AHB1RSTR |= (1 << 2)); (RCC->AHB1RSTR &= ~(1 << 2)); }while(0)
#define GPIOD_REG_RESET()               do{ (RCC->AHB1RSTR |= (1 << 3)); (RCC->AHB1RSTR &= ~(1 << 3)); }while(0)
#define GPIOE_REG_RESET()               do{ (RCC->AHB1RSTR |= (1 << 4)); (RCC->AHB1RSTR &= ~(1 << 4)); }while(0)
#define GPIOF_REG_RESET()               do{ (RCC->AHB1RSTR |= (1 << 5)); (RCC->AHB1RSTR &= ~(1 << 5)); }while(0)
#define GPIOG_REG_RESET()               do{ (RCC->AHB1RSTR |= (1 << 6)); (RCC->AHB1RSTR &= ~(1 << 6)); }while(0)
#define GPIOH_REG_RESET()               do{ (RCC->AHB1RSTR |= (1 << 7)); (RCC->AHB1RSTR &= ~(1 << 7)); }while(0)
#define GPIOI_REG_RESET()               do{ (RCC->AHB1RSTR |= (1 << 8)); (RCC->AHB1RSTR &= ~(1 << 8)); }while(0)

/*
 *  returns port code for given GPIOx base address
 */
/*
 * This macro returns a code( between 0 to 7) for a given GPIO base address(x)
 */
#define GPIO_BASEADDR_TO_CODE(x)      ( (x == GPIOA) ? 0 :\
										(x == GPIOB) ? 1 :\
										(x == GPIOC) ? 2 :\
										(x == GPIOD) ? 3 :\
								        (x == GPIOE) ? 4 :\
								        (x == GPIOF) ? 5 :\
								        (x == GPIOG) ? 6 :\
								        (x == GPIOH) ? 7 :\
								        (x == GPIOI) ? 8 : 0)

//some generic macros

#define ENABLE 				1
#define DISABLE 			0
#define SET 				ENABLE
#define RESET 				DISABLE
#define GPIO_PIN_SET        SET
#define GPIO_PIN_RESET      RESET
#define FLAG_RESET          RESET
#define FLAG_SET 			SET

#endif /* INC_STM32F407XX_H_ */
