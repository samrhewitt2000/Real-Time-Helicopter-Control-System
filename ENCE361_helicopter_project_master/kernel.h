#ifndef KERNEL_H_
#define KERNEL_H_

//*****************************************************************************
//
//      kernel.h
//
// Header file for ProtoKernel
//
//*****************************************************************************
//
// Author:          Caleb Westbury & Sam Hewitt
// Last modified:   May 2024
//
//*****************************************************************************

#include <stdint.h>             // Standard integer types
#include <stdbool.h>            // Boolean types
#include <stdlib.h>             // Standard library
#include "inc/hw_types.h"       // Hardware types
#include "driverlib/sysctl.h"   // System control driver library
#include "driverlib/systick.h"  // SysTick driver library

#define MAX_TASKS 100                   // Maximum number of tasks
#define TICK_COUNT_RESET_THRESHOLD 100000000 // Threshold for tick count reset

//*****************************************************************************
// Task state enumeration
//*****************************************************************************
typedef enum {
    BLOCKED,    // Task is blocked
    READY       // Task is ready for execution
} task_state_t;

//*****************************************************************************
// Task structure
//*****************************************************************************
typedef struct {
    void (*taskEnter)(void);    // Pointer to the task entry function
    task_state_t state;         // Current state of the task
    uint32_t time;              // Time of last execution
    uint32_t interval;          // Execution interval
} task_t;

//*****************************************************************************
// Task IDs structure (not sure if it's a struct or typedef)
//*****************************************************************************
typedef struct {
    void (*SWITCH_TASK)(void);          // Switch task function
    void (*PUSH_BUTTONS_TASK)(void);    // Push buttons task function
    void (*ALT_CONTROL_TASK)(void);     // Altitude control task function
    void (*YAW_CONTROL_TASK)(void);     // Yaw control task function
    void (*TRANSITION_TASK)(void);      // Transition task function
    void (*REF_YAW_TASK)(void);         // Reference yaw task function
    void (*DISPLAY_TASK)(void);         // Display task function
} task_ID_t;

extern task_t tasks[MAX_TASKS];    // Array of tasks
extern unsigned char num_tasks;    // Number of tasks
extern task_ID_t *task_IDs;        // Task IDs

//*****************************************************************************
// Function prototypes
//*****************************************************************************
void SysTickHandler(void);  // SysTick interrupt handler

void pK_init(unsigned char maxTasks, unsigned long tickPeriod); // ProtoKernel initialization

unsigned char pK_register_task(void (*taskEnter)(void), uint32_t interval); // Register a task

void pK_start(void);    // Start ProtoKernel task scheduling

void pK_unregister_task(unsigned char taskId);   // Unregister a task

void pK_ready_task(unsigned char taskId);       // Set a task to ready state

void pK_block_task(unsigned char taskId);       // Block a task

int pK_task_state(unsigned char taskId);        // Get the state of a task

unsigned char pK_get_current_task_id(void);     // Get the ID of the current task

void pK_block_all_tasks(void);                  // Block all tasks

#endif /* KERNEL_H_ */
