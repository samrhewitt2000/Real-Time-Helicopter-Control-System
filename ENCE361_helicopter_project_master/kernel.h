#ifndef KERNEL_H_
#define KERNEL_H_

//*****************************************************************************
//
//      kernel.h
//
// What does this function do? (Replace)
//
//*****************************************************************************
//
// Author:          Caleb Westbury & Sam Hewitt
// Last modified:   May 2024
//
//*****************************************************************************
//
// Based on AUTHOR's FILENAME.c code from YEAR (replace bold if applicable otherwise delete)
//
//*****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"

#define MAX_TASKS 100



//*****************************************************************************
//
//******************************************************************************
typedef enum {
    READY,
    BLOCKED
} task_state_t;

//*****************************************************************************
//
//******************************************************************************
typedef struct {
    void (*taskEnter)(void);
    unsigned char priority;
    task_state_t state;
} task_t;



//*****************************************************************************
//task IDs struct
//*****************************************************************************
typedef struct {
    unsigned char SWITCH_TASK;
    unsigned char PUSH_BUTTONS_TASK;
    unsigned char ALT_CONTROL_TASK;
    unsigned char YAW_CONTROL_TASK;
} task_ID_t;


extern task_t tasks[MAX_TASKS];
extern unsigned char num_tasks;
extern task_t tasks[MAX_TASKS];


//*****************************************************************************
//
//*****************************************************************************
void SysTickHandler(void);



//*****************************************************************************
// pK_init: Initialises protoKernel for up to maxTasks tasks
// Sets period of SysTick interrupt
//*****************************************************************************
void pK_init (unsigned char maxTasks, unsigned long tickPeriod);

//*****************************************************************************
// pK_register_task: Registers a task with the protoKernel;
// Takes a pointer to the function which executes the task and
// an unsigned value for the 'priority' of the task
// (0 is highest priority).
// Tasks are in the ready state by default.
// Returns a unique 8-bit ID.
//*****************************************************************************
unsigned char pK_register_task (void (*taskEnter) (void), unsigned char priority);

//*****************************************************************************
// pK_start: Starts the round-robin scheduling of the tasks (if any) that have
// been registered and that are 'ready'.
//*****************************************************************************
void pK_start (void);

//*****************************************************************************
// pK_unregister_task: Removes the nominated task from those
// registered with protoKernel. The task can be subsequently
// re-registered.
//*****************************************************************************
void pK_unregister_task (unsigned char taskId);

//*****************************************************************************
// pK_ready_task: Switches the task with ID 'taskId' to 'ready'
// so that it will be executed within the round robin.
//*****************************************************************************
void pK_ready_task (unsigned char taskId);

//*****************************************************************************
// pK_block_task: Switches the task with ID 'taskId' to
// 'blocked' so that it will not be executed.
//*****************************************************************************
void pK_block_task (unsigned char taskId);

//*****************************************************************************
// pK_task_state: Returns the current state of the task.
//*****************************************************************************
int pK_task_state (unsigned char taskId);

//*****************************************************************************
// pK_get_current_task_id: Returns the current task ID.
//*****************************************************************************
unsigned char pK_get_current_task_id(void);

//*****************************************************************************
// pK_block_all_tasks: Switches all tasks to
// 'blocked' so that they will not be executed.
//*****************************************************************************
void pK_block_all_tasks(void);

#endif /*KERNEL_H_*/
