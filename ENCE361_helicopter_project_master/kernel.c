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
    //num_tasks = maxTasks;
    num_tasks = 0;
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
// Tasks are in the ready state by default.s
// Returns a unique 8-bit ID.
//*****************************************************************************
unsigned char pK_register_task(void (*taskEnter)(void), uint32_t priority, uint32_t interval)
{
    if (num_tasks <= MAX_TASKS)
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



//*****************************************************************************
// pK_start: Starts the round-robin scheduling of the tasks (if any) that have
// been registered and that are 'ready'.
//*****************************************************************************
void pK_start(void)
{
    // Ensure that there is at least one task registered
    if (num_tasks == 0) {
        return; // No tasks to schedule
    }

    // Execute the first task immediately
    if (tasks[currentTaskId].taskEnter) {
        tasks[currentTaskId].taskEnter();
    }

    //to finish
}



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



//*****************************************************************************
//
//*****************************************************************************
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

