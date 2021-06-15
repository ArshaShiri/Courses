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
#include <stdio.h>

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

// We allocate 1kb of stack to each task and the scheduler itself(total of 5kb)
// The stack is given from the top address in descending order.
#define SIZE_TASK_STACK         1024U
#define SIZE_SCHEDULER_STACK    1024U

#define SRAM_START_ADDRESS      0x20000000U
#define SIZE_OF_SRAM            ( (128) * (1024) )
#define SRAM_END_ADDRESS        ( (SRAM_START_ADDRESS) + (SIZE_OF_SRAM) )

// Task's 1 stack starts from the end of the ram.
#define T1_STACK_START          SRAM_END_ADDRESS
#define T2_STACK_START          ( (T1_STACK_START) - (SIZE_TASK_STACK) )
#define T3_STACK_START          ( (T2_STACK_START) - (SIZE_TASK_STACK) )
#define T4_STACK_START          ( (T3_STACK_START) - (SIZE_TASK_STACK) )
#define SCHEDULER_STACK_START   ( (T4_STACK_START) - (SIZE_TASK_STACK) )

void task1_handler(void);
void task2_handler(void);
void task3_handler(void);
void task4_handler(void);

int main(void)
{
    /* Loop forever */
	for(;;);
}

void task1_handler(void)
{
	while (1)
	{
		printf("This is task1\n");
	}
}

void task2_handler(void)
{
	while (1)
	{
		printf("This is task2\n");
	}
}

void task3_handler(void)
{
	while (1)
	{
		printf("This is task3\n");
	}
}

void task4_handler(void)
{
	while (1)
	{
		printf("This is task4\n");
	}
}
