/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Auto-generated by STM32CubeIDE
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */

#include <stdint.h>
#include <limits.h>

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

int main(void)
{
	uint32_t *pClockControRegister = (uint32_t*)0x40023830;
	uint32_t *pPortDModeTypeRegister = (uint32_t*)0x40020C00;
	uint32_t *pPortDOudRegister = (uint32_t*)0x40020C14;

	// Enable the clock for GPIOD peripheral in the AHB1ENR (Set the 3rd bit position)
	*pClockControRegister |= (1 << 3);


	// Configure the mode to output.
	// Clear the 24th and 25th bit positions.
	*pPortDModeTypeRegister &= ~(3 << 24);

	// Set the 24th bit to 1
	*pPortDModeTypeRegister |= (1 << 24);

	*pPortDModeTypeRegister |= (1 << 26);
	*pPortDModeTypeRegister |= (1 << 28);
	*pPortDModeTypeRegister |= (1 << 30);


	// Set the 12th bit of the output data register to make I/O pin 12 high.
	*pPortDOudRegister |= (1 << 12);
	*pPortDOudRegister |= (1 << 13);
	*pPortDOudRegister |= (1 << 14);
	*pPortDOudRegister |= (1 << 15);


    /* Loop forever */
	for(long long i = 0; i > -1; ++i)
	{
		// Add a very basic software LED toggle.
	   if ((i % 10000) == 0)
	     *pPortDOudRegister ^= 0x1000;

	   if (i == LLONG_MAX)
		   i = 0;
	}
}
