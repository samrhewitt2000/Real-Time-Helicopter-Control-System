//*****************************************************************************
//
//      kernel.c
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

#include "kernel.h"
#include <stdint.h>


//*****************************************************************************
//
//*****************************************************************************


static unsigned char numTasks = 6;
static unsigned long g_tickPeriod = 0;
volatile unsigned char currentTaskId = 0; // Initialize to the first task
volatile uint32_t systick_flag = 0;

//*****************************************************************************
//
//*****************************************************************************
void SysTickHandler(void)
{
    uint32_t tick_count = 0;


    // Find the next ready task
    unsigned char nextTaskId = (currentTaskId + 1) % numTasks;

    // Loop through tasks to find the next one that is ready
    while (tasks[nextTaskId].state != READY) {
        nextTaskId = (nextTaskId + 1) % numTasks;

        // Break the loop if we have checked all tasks to avoid infinite loop
        if (nextTaskId == currentTaskId) {
            return;
        }
    }

    // Update the current task ID to the next ready task
    currentTaskId = nextTaskId;
    tick_count++;
    }



//*****************************************************************************
// pK_init: Initialises protoKernel for up to maxTasks tasks
// Sets period of SysTick interrupt
//*****************************************************************************
void pK_init(unsigned char maxTasks, unsigned long tickPeriod)
{
    numTasks = maxTasks;
    g_tickPeriod = tickPeriod;
    SysTickPeriodSet(g_tickPeriod);
    SysTickEnable();
    SysTickIntEnable();
}



//*****************************************************************************
// pK_register_task: Registers a task with the protoKernel;
// Takes a pointer to the function which executes the task and
// an unsigned value for the 'priority' of the task
// (0 is highest priority).
// Tasks are in the ready state by default.
// Returns a unique 8-bit ID.
//*****************************************************************************
unsigned char pK_register_task(void (*taskEnter)(void), unsigned char priority)
{
    if (numTasks < MAX_TASKS)
    {
        tasks[numTasks].taskEnter = taskEnter;
        tasks[numTasks].priority = priority;
        tasks[numTasks].state = BLOCKED;
        return numTasks++;

    }
    return 0xFF; // Error: Task registration failed
}




void pK_start(void)
{
  SysTickPeriodSet(g_tickPeriod);
  SysTickEnable();
  SysTickIntEnable();
  //round robin
  while(1)
  {
      while (!systick_flag)
      {
      }
      if (tasks[currentTaskId].taskEnter && tasks[currentTaskId].state == READY)
      {
          tasks[currentTaskId].taskEnter();
      }
  }
}


//*****************************************************************************
// pK_unregister_task: Removes the nominated task from those
// registered with protoKernel. The task can be subsequently
// re-registered.
//*****************************************************************************
void pK_unregister_task(unsigned char taskId)
{
    if (taskId < numTasks)
    {
        // Remove the task
        tasks[taskId].taskEnter = NULL;
        tasks[taskId].priority = 0;
        tasks[taskId].state = BLOCKED;
    }
}

//*****************************************************************************
// pK_ready_task: Switches the task with ID 'taskId' to 'ready'
// so that it will be executed within the round robin.
//*****************************************************************************
void pK_ready_task(unsigned char taskId)
{
    if (taskId < numTasks)
    {
        tasks[taskId].state = READY;
    }
}

//*****************************************************************************
// pK_block_task: Switches the task with ID 'taskId' to
// 'blocked' so that it will not be executed.
//*****************************************************************************
void pK_block_task(unsigned char taskId)
{
    if (taskId < numTasks)
    {
        tasks[taskId].state = BLOCKED;
    }
}

//*****************************************************************************
// pK_task_state: Returns the current state of the task.
//*****************************************************************************
int pK_task_state(unsigned char taskId)
{
    if (taskId < numTasks)
    {
        return tasks[taskId].state;
    }
    return -1; // Error: Task ID out of range
}

//*****************************************************************************
// pK_get_current_task_id: Returns the current task ID.
//*****************************************************************************
unsigned char pK_get_current_task_id(void)
{
    return currentTaskId;
}
