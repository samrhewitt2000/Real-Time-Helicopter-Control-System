//*****************************************************************************
// 
//      main.c
//
// What does this module do? (Replace)
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
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/adc.h"
#include "driverlib/pwm.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/interrupt.h"
#include "driverlib/debug.h"
#include "utils/ustdlib.h"
#include "OrbitOLED/OrbitOLEDInterface.h"

#include "inc/hw_ints.h"

#include "circ_buffer.h"
#include "quad_enc.h"
#include "displays.h"
#include "ADC.h"
#include "buttons.h"
#include "PWM.h"
#include "kernel.h"
#include "alt_control.h"
#include "yaw_control.h"

//*****************************************************************************
// Constants
//*****************************************************************************
#define BUF_SIZE 10



// *******************************************************
// Display state enum
// *******************************************************
typedef enum {
    STATE_PERC,             // display altitude percentages state
    STATE_MEAN_ADC_VAL,     // display mean adc values state
    STATE_OFF,              // screen off state
} display_state_t;



// *******************************************************
// Global Variables
// *******************************************************
uint32_t ui32RotorFreq = PWM_START_RATE_HZ;
uint32_t ui32RotorDuty = PWM_FIXED_DUTY;
uint32_t ui32TailFreq = PWM_START_RATE_HZ;
uint32_t ui32TailDuty = PWM_FIXED_DUTY;
display_state_t display_state = STATE_PERC; //initialize display state
extern helicopter_state_t heli_state; //initialize display state
helicopter_state_t prev_heli_state;

// Perform the task registration for the protokernal
task_ID_t task_IDs;

//********************************************************
//
// ********************************************************
void initialise_program(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF))
    {
    }

    initClocks ();
    // Initialize the protoKernel with a maximum of 10 tasks and a tick period
    pK_init(MAX_TASKS, SysCtlClockGet() / SYSTICK_RATE_HZ); // e.g., 10ms tick period

    initButtons();
    initADC ();
    initDisplay ();
    initYaw ();
    initialise_rotor_PWM ();
    initialise_tail_PWM ();
    PWMOutputState(PWM_MAIN_BASE, PWM_MAIN_OUTBIT, true);
    PWMOutputState(PWM_TAIL_BASE, PWM_TAIL_OUTBIT, true);
    //initSysTick ();

    // System initialization (e.g., clock setup, peripherals)
    //SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    initCircBuf (&g_inBuffer, BUF_SIZE);
    // calculate exactly how long this needs to be
    SysCtlDelay (SysCtlClockGet() / 6); // delay so that buffer can fill
}



//*****************************************************************************
//
//*****************************************************************************
void register_all_pk_tasks(task_ID_t *task_IDs)
{
    task_IDs->SWITCH_TASK = pK_register_task(switch_task, 0, 100);
    task_IDs->PUSH_BUTTONS_TASK = pK_register_task(pushbuttons_task, 1, 500);
    task_IDs->ALT_CONTROL_TASK = pK_register_task(alt_control_task, 2, 1000);
    task_IDs->YAW_CONTROL_TASK = pK_register_task(yaw_control_task, 3, 200);
    task_IDs->GET_YAW_REF_TASK = pK_register_task(get_yaw_ref_task, 4, 300);
}



int main(void)
{
    //Initialise peripherals etc
    initialise_program();

    // Kill motors for software reset
    kill_motors(&heli_state);

    register_all_pk_tasks(&task_IDs);

    IntMasterEnable();

    // Read in relevant peripheral values
    extern int32_t prev_switch_state;
    //prev_switch_state = GPIOPinRead (SWITCH_PORT_BASE, SWITCH_PIN) == SWITCH_PIN;
    prev_switch_state = 0;
    extern int32_t current_switch_state;
    current_switch_state = prev_switch_state;
    int32_t initial_ADC_val = get_ADC_val(&g_inBuffer, BUF_SIZE);

    while (1)
    {


        // Background task: calculate the (approximate) mean of the values in the circular buffer and display it, together with the sample number.
        //current_switch_state = GPIOPinRead (SWITCH_PORT_BASE, SWITCH_PIN) == SWITCH_PIN;

        switch(heli_state)
        {
            case LANDED:
                // set rotor and tail motors to zero

                if (prev_heli_state != LANDED)
                {
                    kill_motors(&heli_state); // precautionary
                }
                pK_ready_task(task_IDs.SWITCH_TASK);

                break;
            case TAKEOFF:
                // helicopter calibrates to reference yaw when take off switch pressed
                if (prev_heli_state != LANDED)
                {
                    pK_ready_task(task_IDs.GET_YAW_REF_TASK);
                }
                pK_ready_task(task_IDs.SWITCH_TASK);
                pK_ready_task(task_IDs.PUSH_BUTTONS_TASK);
                pK_ready_task(task_IDs.YAW_CONTROL_TASK);
                pK_ready_task(task_IDs.ALT_CONTROL_TASK);

                break;
            case FLYING:
                // helicopter doesnt spaz when both yaw and altitude pressed consecutively
                // alt in range 0 - 100 and pwm duty in range 2 - 98
                break;
            case LANDING:
                // When helicopter is landing pressing buttons or switches do nothing
                // helicopter should return to reference yaw and land smoothly
                break;
        }

        debug_display(0, 1, current_switch_state, heli_state);

        pK_start();

        pK_block_all_tasks();

        prev_heli_state = heli_state;
    }
}

