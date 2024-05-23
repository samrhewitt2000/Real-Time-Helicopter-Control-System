#ifndef KERNAL_H_
#define KERNAL_H_

//*****************************************************************************
//
//      kernal.h
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
void pK_ready_task (unsigned char taskId);

//*****************************************************************************
// pK_task_state: Returns the current state of the task.
//*****************************************************************************
int pK_task_state (unsigned char taskId);

#endif /*KERNAL_H_*/
