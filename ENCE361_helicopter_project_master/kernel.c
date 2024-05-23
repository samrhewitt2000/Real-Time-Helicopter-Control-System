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
//******************************************************************************
typedef enum {
    READY,
    BLOCKED
} task_state_t;

//*****************************************************************************
//
//*****************************************************************************
typedef struct {
    void (*taskEnter)(void);
    unsigned char priority;
    task_state_t state;
} task_t;

static task_t tasks[MAX_TASKS];
static unsigned char numTasks = 6;
static unsigned long g_tickPeriod = 0;
volatile unsigned char currentTaskId = 0; // Initialize to the first task
volatile uint32_t systick_flag = 0;

//*****************************************************************************
//
//*****************************************************************************
void SysTickHandler(void)
{
    systick_flag = 1;
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

    // Execute the current task if it has a valid function pointer
    if (tasks[currentTaskId].taskEnter) {
        tasks[currentTaskId].taskEnter();
    }

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

//*****************************************************************************
// pK_start: Starts the round-robin scheduling of the tasks (if any) that have
// been registered and that are 'ready'.
//*****************************************************************************
//void pK_start(void)
//{
//    // Ensure that there is at least one task registered
//    if (numTasks == 0) {
//        return; // No tasks to schedule
//    }
//
//    // Execute the first task immediately
//    if (tasks[currentTaskId].taskEnter) {
//        tasks[currentTaskId].taskEnter();
//    }
//    while(1)
//    {
//        //wait for systick
//        while (!systick_flag)
//        {
//
//        systick_flag = 0;
//
//        //find next task thats ready
//        unsigned char nextTaskId = (currentTaskId + 1) % numTasks;
//
//        //loop until ready task found
//        while (tasks[nextTaskId].state != READY)
//        {
//            nextTaskId = (nextTaskId + 1) % numTasks;
//
//            //break if all tasks checked
//            if (nextTaskId == currentTaskId)
//            {
//                break;
//            }
//        }
//
//        //set current to next ready task
//        currentTaskId = nextTaskId;
//
//        //execute current task
//        if (tasks[currentTaskId].taskEnter)
//        {
//            tasks[currentTaskId].taskEnter();
//        }
//        }
//
//    }
//    //to finish
//}
void pK_start(void)
{
  SysTickPeriodSet(g_tickPeriod);
  SysTickEnable();
  SysTickIntEnable();
  int j;
  //round robin
  while(1)
  {
      for (j=0; j < numTasks; j++)
      {
          if (tasks[j].state == READY)
          {

              //clear flag
              tasks[j].state = BLOCKED;

              //execute task
              tasks[j].taskEnter();
          }
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
