//*****************************************************************************
//
//      kernel.c
//
// Kernel for task scheduling and execution
//
//*****************************************************************************
//
// Author:          Caleb Westbury & Sam Hewitt
// Last modified:   May 2024
//
//*****************************************************************************

#include "kernel.h"             // Kernel header file
#include <stdint.h>             // Standard integer types
#include "ADC.h"                // ADC functions
#include "inc/hw_memmap.h"      // Memory map definitions
#include "inc/hw_types.h"       // Hardware types
#include "driverlib/adc.h"      // ADC driver library
#include "driverlib/pwm.h"      // PWM driver library
#include "driverlib/gpio.h"     // GPIO driver library
#include "driverlib/sysctl.h"   // System control driver library
#include "driverlib/systick.h"  // SysTick driver library
#include "driverlib/interrupt.h"   // Interrupt driver library
#include "driverlib/debug.h"    // Debug driver library
#include "utils/ustdlib.h"      // Micro Standard Library
#include "OrbitOLED/OrbitOLEDInterface.h"    // OLED interface

//*****************************************************************************
// Global variables
//*****************************************************************************
unsigned char num_tasks = 0;
static unsigned long g_tickPeriod = 0;
volatile unsigned char current_task_ID = 0;
volatile uint32_t systick_flag = 0;
task_t tasks[MAX_TASKS];
static uint32_t tick_count = 0;

//*****************************************************************************
// SysTick interrupt handler
//*****************************************************************************
void SysTickHandler(void)
{
    ADCProcessorTrigger(ADC0_BASE, 3); // Trigger ADC conversion
    g_ulSampCnt++; // Increment sample count
    updateButtons(); // Update button states

    tick_count++; // Increment tick count
    if (tick_count >= TICK_COUNT_RESET_THRESHOLD)
    {
        tick_count = 0; // Reset tick count after threshold
    }
}

//*****************************************************************************
// pK_init: Initializes the kernel with specified maximum number of tasks and tick period
//*****************************************************************************
void pK_init(unsigned char maxTasks, unsigned long tickPeriod)
{
    g_tickPeriod = tickPeriod;
    current_task_ID = 0;
    SysTickPeriodSet(g_tickPeriod);
    SysTickIntRegister(SysTickHandler);
    SysTickIntEnable();
    SysTickEnable();
}

//*****************************************************************************
// pK_register_task: Registers a task with the kernel, assigns an ID, and sets interval
//*****************************************************************************
unsigned char pK_register_task(void (*taskEnter)(void), uint32_t interval)
{
    if (num_tasks <= MAX_TASKS)
    {
        tasks[num_tasks].taskEnter = taskEnter;
        tasks[num_tasks].state = BLOCKED;
        tasks[num_tasks].time = 0;
        tasks[num_tasks].interval = interval;

        unsigned char taskID = num_tasks;
        num_tasks++;
        return taskID;
    }
    return 0xFF; // Error: Task registration failed
}

//*****************************************************************************
// pK_start: Starts task execution
//*****************************************************************************
void pK_start(void)
{
    unsigned char checked_tasks = 0;
    current_task_ID = 0;

    // Check all tasks to find a ready one
    while (checked_tasks < num_tasks)
    {
        // Check if the task is ready
        if (tasks[current_task_ID].state == READY)
        {
            // Task is ready, check if its time interval has elapsed
            if (tick_count >= tasks[current_task_ID].time)
            {
                tasks[current_task_ID].time += tasks[current_task_ID].interval + tick_count - tasks[current_task_ID].time;

                // Check if taskEnter is not NULL before calling it
                if (tasks[current_task_ID].taskEnter != NULL)
                {
                    tasks[current_task_ID].taskEnter();
                }

                // Update the current task ID to the next ready task
                current_task_ID = (current_task_ID + 1) % num_tasks;

                return; // Exit after executing one task
            }
        }
        if (tasks[current_task_ID].time >= TICK_COUNT_RESET_THRESHOLD)
        {
            tasks[current_task_ID].time = 0;
        }
        // Move to the next task
        current_task_ID = (current_task_ID + 1) % num_tasks;
        checked_tasks++;
    }

    // If no task is ready to execute, the function will return without doing anything
}

//*****************************************************************************
// pK_unregister_task: Removes the task from the kernel's task list
//*****************************************************************************
void pK_unregister_task(unsigned char taskId)
{
    if (taskId < num_tasks)
    {
        // Remove the task
        tasks[taskId].taskEnter = NULL;
        tasks[taskId].interval = 0;
        tasks[taskId].state = BLOCKED;
    }
}

//*****************************************************************************
// pK_ready_task: Sets the state of the task to READY 
