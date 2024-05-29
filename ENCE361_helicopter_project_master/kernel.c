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


unsigned char num_tasks = 6;
static unsigned long g_tickPeriod = 0;
volatile unsigned char currentTaskId = 0; // Initialize to the first task
volatile uint32_t systick_flag = 0;
task_t tasks[MAX_TASKS];
uint32_t tick_count = 0;

//*****************************************************************************
//
//*****************************************************************************
void SysTickHandler(void)
{
    tick_count++;
    if (tick_count >= TICK_COUNT_RESET_THRESHOLD)
    {
        tick_count = 0;
    }
}



//*****************************************************************************
// pK_init: Initialises protoKernel for up to maxTasks tasks
// Sets period of SysTick interrupt
//*****************************************************************************
void pK_init(unsigned char maxTasks, unsigned long tickPeriod)
{
    num_tasks = maxTasks;
    g_tickPeriod = tickPeriod;
    SysTickPeriodSet(g_tickPeriod);
    SysTickEnable();
    SysTickIntEnable();
}


void (*task_functions[MAX_TASKS])(void);


//*****************************************************************************
// pK_register_task: Registers a task with the protoKernel;
// Takes a pointer to the function which executes the task and
// an unsigned value for the 'priority' of the task
// (0 is highest priority).
// Tasks are in the ready state by default.s
// Returns a unique 8-bit ID.
//*****************************************************************************
unsigned char pK_register_task(void (*taskEnter)(void), uint32_t interval)
{
    if (num_tasks < MAX_TASKS)
    {
        tasks[num_tasks].taskEnter = taskEnter;
//        tasks[num_tasks].priority = priority;      unsigned char priority,
        tasks[num_tasks].state = BLOCKED;
        uint32_t time = 0; //initialize time as 0 in order to execute first according to priority
        tasks[num_tasks].time = time;
        tasks[num_tasks].interval = interval;

        unsigned char taskID = num_tasks;
        num_tasks++;
        return taskID;
    }
    return 0xFF; // Error: Task registration failed
}



void pK_start(void)
{
    int any_task_ready = 0;

    // Find the next ready task
    unsigned char nextTaskId = (currentTaskId + 1) % num_tasks;

    // Loop through tasks to find the next one that is ready
    while (tasks[nextTaskId].state != READY) {
        if (tasks[nextTaskId].state == READY)
        {
            any_task_ready = 1;
        }


        nextTaskId = (nextTaskId + 1) % num_tasks;

        // Break the loop if we have checked all tasks to avoid infinite loop
        if (nextTaskId == currentTaskId)
        {
            if (!any_task_ready)
            {
                return;
            }
            any_task_ready = 0;
        }
    }
    //execute if time interval has elapsed and update time
    if (tick_count >= tasks[nextTaskId].time)
    {
        tasks[nextTaskId].time += tasks[nextTaskId].interval;
        tasks[nextTaskId].taskEnter();
    }

    // Update the current task ID to the next ready task
    currentTaskId = nextTaskId;

}

//
//void pK_start(void)
//{
//  SysTickPeriodSet(g_tickPeriod);
//  SysTickEnable();
//  SysTickIntEnable();
//  //round robin
//  while(1)
//  {
//      while (!systick_flag)
//      {
//      }
//      if (tasks[currentTaskId].taskEnter && tasks[currentTaskId].state == READY)
//      {
//          tasks[currentTaskId].taskEnter();
//      }
//  }
//}


//*****************************************************************************
// pK_unregister_task: Removes the nominated task from those
// registered with protoKernel. The task can be subsequently
// re-registered.
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
// pK_ready_task: Switches the task with ID 'taskId' to 'ready'
// so that it will be executed within the round robin.
//*****************************************************************************
void pK_ready_task(unsigned char taskId)
{
    if (taskId < num_tasks)
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
    if (taskId < num_tasks)
    {
        tasks[taskId].state = BLOCKED;
    }
}

//*****************************************************************************
// pK_task_state: Returns the current state of the task.
//*****************************************************************************
int pK_task_state(unsigned char taskId)
{
    if (taskId < num_tasks)
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



void pK_block_all_tasks(void)
{
    int32_t i = 0;  // Initialize loop counter outside the loop

    // Loop through tasks to find the next one that is ready
    while (i < num_tasks)  // Condition without initialization or increment here
    {
        if (tasks[i].state == READY)
        {
            tasks[i].state = BLOCKED;
        }
        i++;  // Increment i within the loop
    }
}

