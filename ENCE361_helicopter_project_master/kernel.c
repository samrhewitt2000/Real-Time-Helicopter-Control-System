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
    debug_display(0, 1, tick_count, heli_state);
    if (tick_count >= TICK_COUNT_RESET_THRESHOLD)
    {
        tick_count = 0;
    }
    ADCProcessorTrigger(ADC0_BASE, 3);
    updateButtons();
    g_ulSampCnt++;
}



//*****************************************************************************
// pK_init: Initialises protoKernel for up to maxTasks tasks
// Sets period of SysTick interrupt
//*****************************************************************************
void pK_init(unsigned char maxTasks, unsigned long tickPeriod)
{
    //num_tasks_registered = maxTasks;
    num_tasks_registered = 0;
    //g_tickPeriod = tickPeriod;
    //SysTickPeriodSet(g_tickPeriod);


    // Set up the period for the SysTick timer.  The SysTick
    // timer period is set as a function of the system clock.
    SysTickPeriodSet (tickPeriod);
    //
    // Register the interrupt handler
    SysTickIntRegister (SysTickHandler);

    //
    // Enable interrupt and device
    SysTickIntEnable ();
    SysTickEnable ();
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
    if (num_tasks_registered <= MAX_TASKS)
    {
        tasks[num_tasks_registered].taskEnter = taskEnter;
//        tasks[num_tasks_registered].priority = priority;      unsigned char priority,
        tasks[num_tasks_registered].state = BLOCKED;
        uint32_t time = 0; //initialize time as 0 in order to execute first according to priority
        tasks[num_tasks_registered].time = time;
        tasks[num_tasks_registered].interval = interval;

        unsigned char taskID = num_tasks_registered;
        num_tasks_registered++;
        return taskID;
    }
    return 0xFF; // Error: Task registration failed
}



void pK_start(void)
{
    unsigned char checked_tasks = 0;
//    unsigned char current_task_ID = (current_task_ID + 1) % num_tasks_registered;

    // Check all tasks to find a ready one
    while (checked_tasks < num_tasks_registered)
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
                current_task_ID = (current_task_ID + 1) % num_tasks_registered;

                return; // Exit after executing one task
            }
        }

        // Move to the next task
        current_task_ID = (current_task_ID + 1) % num_tasks_registered;
        checked_tasks++;
    }

    // If no task is ready to execute, the function will return without doing anything
}




//*****************************************************************************
// pK_unregister_task: Removes the nominated task from those
// registered with protoKernel. The task can be subsequently
// re-registered.
//*****************************************************************************
void pK_unregister_task(unsigned char taskId)
{
    if (taskId < num_tasks_registered)
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
    if (taskId < num_tasks_registered)
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
    if (taskId < num_tasks_registered)
    {
        tasks[taskId].state = BLOCKED;
    }
}

//*****************************************************************************
// pK_task_state: Returns the current state of the task.
//*****************************************************************************
int pK_task_state(unsigned char taskId)
{
    if (taskId < num_tasks_registered)
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
    return current_task_ID;
}



//*****************************************************************************
//
//*****************************************************************************
void pK_block_all_tasks(void)
{
    int32_t i = 0;  // Initialize loop counter outside the loop

    // Loop through tasks to find the next one that is ready
    while (i < num_tasks_registered)  // Condition without initialization or increment here
    {
        if (tasks[i].state == READY)
        {
            tasks[i].state = BLOCKED;
        }
        i++;  // Increment i within the loop
    }
}

