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
#include "communications.h"
//*****************************************************************************
// Constants
//*****************************************************************************




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
extern int32_t initial_ADC_val = 0;    // initialize first value
volatile extern int32_t current_ADC_val = 0;    // initialize first value
uint32_t ui32RotorFreq = PWM_START_RATE_HZ;
uint32_t ui32RotorDuty = PWM_FIXED_DUTY;
uint32_t ui32TailFreq = PWM_START_RATE_HZ;
uint32_t ui32TailDuty = PWM_FIXED_DUTY;
display_state_t current_state = STATE_PERC; //initialize display state
helicopter_state_t heli_state = LANDED; //initialize display state
int32_t current_switch_state;
uint32_t counter = 0;
extern unsigned char num_tasks;

// *******************************************************
// Function prototypes
// *******************************************************
void increase_altitude_task(void);
void decrease_altitude_task(void);



//********************************************************
// Function to set the freq, duty cycle of M1PWM5 (tail motor)
// ********************************************************
//void kill_motors(helicopter_state_t heli_state)
//{
//    //set main motor PWM signal to zero
//    PWMGenPeriodSet(PWM_MAIN_BASE, PWM_MAIN_GEN, 0);
//    PWMPulseWidthSet(PWM_MAIN_BASE, PWM_MAIN_OUTNUM, 0);
//
//    //set tail motor PWM signal to zero
//    PWMGenPeriodSet(PWM_TAIL_BASE, PWM_TAIL_GEN, 0);
//    PWMPulseWidthSet(PWM_TAIL_BASE, PWM_TAIL_OUTNUM, 0);
//
//    heli_state = LANDED;
//}



//********************************************************
//
// ********************************************************
void initialise_program(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF))
    {
    }

    initButtons();
    initClocks ();
    initADC ();
    initDisplay ();
    initYaw ();
    initialise_rotor_PWM ();
    initialise_tail_PWM ();
    PWMOutputState(PWM_MAIN_BASE, PWM_MAIN_OUTBIT, true);
    PWMOutputState(PWM_TAIL_BASE, PWM_TAIL_OUTBIT, true);
    initSysTick ();

    // System initialization (e.g., clock setup, peripherals)
    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    // Initialize the protoKernel with a maximum of 10 tasks and a tick period
    pK_init(MAX_TASKS, SysCtlClockGet() / 100); // e.g., 10ms tick period

    //initialise buffer
    initCircBuf (&g_inBuffer, BUF_SIZE);
    SysCtlDelay (SysCtlClockGet() / 6); // delay so that buffer can fill
}

//void get_ADC_task(void)
//{
//    current_ADC_val = get_ADC_val(&g_inBuffer, BUF_SIZE);
//}


void buttons_task(void)
{
    updateButtons();
    counter += 1;
    if (checkButton(UP) == PUSHED && heli_state == FLYING)
    {
        //increase altitude by 10%
        change_altitude(alt_val_to_percent(initial_ADC_val,current_ADC_val), 10);
    }else if (checkButton(DOWN) == PUSHED && heli_state == FLYING)
    {
        //decrease altitude by 10%
        change_altitude(alt_val_to_percent(initial_ADC_val,current_ADC_val), -10);
    }else if (checkButton(SWITCH) == PUSHED && heli_state == LANDED)
    {
        heli_state = TAKEOFF;
    }else if (checkButton(SWITCH) == RELEASED && heli_state == FLYING)
    {
        heli_state = LANDING;
    }
}



void transition_task(void)
{
    switch(heli_state)
    {
        case TAKEOFF:
            //lift off by 1% :)
            change_altitude(alt_val_to_percent(initial_ADC_val, current_ADC_val), 10);
            heli_state = FLYING;
            break;
        case LANDING:
            change_altitude(alt_val_to_percent(initial_ADC_val, current_ADC_val), -100);
            if (alt_val_to_percent(initial_ADC_val, current_ADC_val) == 0)
            {
                kill_motors(heli_state);
            }
            break;
        case FLYING:
            break;
        case LANDED:
            break;
    }
}




void print_task(void)
{
    if (but_state[4] == 1)
    {
        heli_state = FLYING;
        change_altitude(alt_val_to_percent(initial_ADC_val, current_ADC_val), 10);
    }
}



void get_sensor_values(void)
{
    current_ADC_val = get_ADC_val(&g_inBuffer, BUF_SIZE);
}


//*****************************************************************************
//
//*****************************************************************************

void register_all_pk_tasks(task_ID_t *task_IDs)
{
    task_IDs->SWITCH_TASK = pK_register_task(switch_task, 0);
    task_IDs->PUSH_BUTTONS_TASK = pK_register_task(pushbuttons_task, 1);
    task_IDs->ALT_CONTROL_TASK = pK_register_task(alt_control_task, 2);
    task_IDs->YAW_CONTROL_TASK = pK_register_task(yaw_control_task, 2);
}



int main(void)
{
    initialise_program();


    // calculate exactly how long this needs to be

    initial_ADC_val = get_ADC_val(&g_inBuffer, BUF_SIZE);

    kill_motors(&heli_state);
    IntMasterEnable();

    int32_t *prev_switch_state = GPIOPinRead (SWITCH_PORT_BASE, SWITCH_PIN) == SWITCH_PIN;


    task_ID_t task_IDs[num_tasks];
    register_all_pk_tasks(&task_IDs);
//
//    taskIDs[INCREASE_ALT_TASK] = pK_register_task(increase_altitude_task, 0);
//    taskIDs[DECREASE_ALT_TASK] = pK_register_task(decrease_altitude_task, 0);
//
//    taskIDs[PUSH_BUTTONS_TASK] = pK_register_task(buttons_task, 0);
//    taskIDs[TRANSITION_TASK] = pK_register_task(transition_task, 0);
//    //unsigned char get_sensor_values_task_ID = pK_register_task(get_sensor_values_task, 0);
//    taskIDs[PRINT_TASK] = pK_register_task(print_task, 1);
////    taskIDs[GET_ADC_TASK] = pK_register_task(get_ADC_task, 0);


//    pK_ready_task(taskIDs[GET_ADC_TASK]);
//    pK_ready_task(taskIDs[TRANSITION_TASK]);
//    pK_ready_task(taskIDs[DECREASE_ALT_TASK]);
//    pK_ready_task(taskIDs[BUTTONS_TASK]);
//    pK_ready_task(taskIDs[INCREASE_ALT_TASK]);
//    //pK_ready_task(get_sensor_values_task_ID);
//    pK_ready_task(taskIDs[GET_ADC_TASK]);

    bool first_state_entry = true;

    while (1)
    {
        // Background task: calculate the (approximate) mean of the values in the circular buffer and display it, together with the sample number.
        current_switch_state = GPIOPinRead (SWITCH_PORT_BASE, SWITCH_PIN) == SWITCH_PIN;

        switch(heli_state)
        {
            case INIT:

                break;
            case LANDED:
                // set rotor and tail motors to zero
                if (first_state_entry)
                {
                    kill_motors(&heli_state); // precautionary
                    first_state_entry = false;
                }
                pK_ready_task(switch_task);
                break;
            case TAKEOFF:
                // helicopter calibrates to reference yaw when take off switch pressed

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


        pK_start();

        pK_block_all_tasks();


    }
}


