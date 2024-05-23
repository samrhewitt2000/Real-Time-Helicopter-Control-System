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
// Helicopter state enum
// *******************************************************
typedef enum {
    LANDED,
    TAKEOFF,
    FLYING,
    LANDING
} helicopter_state_t;



// *******************************************************
// Global Variables
// *******************************************************
uint32_t ui32RotorFreq = PWM_START_RATE_HZ;
uint32_t ui32RotorDuty = PWM_FIXED_DUTY;
uint32_t ui32TailFreq = PWM_START_RATE_HZ;
uint32_t ui32TailDuty = PWM_FIXED_DUTY;
display_state_t display_state = STATE_PERC; //initialize display state
helicopter_state_t heli_state = LANDED; //initialize display state



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


    // Initialize the protoKernel with a maximum of 10 tasks and a tick period
    pK_init(MAX_TASKS, SysCtlClockGet() / 100); // e.g., 10ms tick period

    // System initialization (e.g., clock setup, peripherals)
    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    initCircBuf (&g_inBuffer, BUF_SIZE);
    // calculate exactly how long this needs to be
    SysCtlDelay (SysCtlClockGet() / 6); // delay so that buffer can fill
}



//*****************************************************************************
//
//*****************************************************************************
void register_all_pk_tasks(void)
{
    unsigned char switch_task_ID = pK_register_task(switch_task, 0);
    unsigned char pushbuttons_task_ID = pK_register_task(pushbuttons_task, 1);
    unsigned char alt_control_task_ID = pK_register_task(alt_control_task, 2);
    unsigned char yaw_control_task_ID = pK_register_task(yaw_control_task, 2);
}

void get_ADC_task(void)
{
    current_ADC_val = get_ADC_val(&g_inBuffer, BUF_SIZE);
}


void buttons_task(void)
{
    *counter += 1;
    if (checkButton(UP) == PUSHED && current_heli_state == FLYING)
    {
        //increase altitude by 10%
        change_altitude(alt_val_to_percent(initial_ADC_val,current_ADC_val), 10);
    }else if (checkButton(DOWN) == PUSHED && current_heli_state == FLYING)
    {
        //decrease altitude by 10%
        change_altitude(alt_val_to_percent(initial_ADC_val,current_ADC_val), -10);
    }else if (checkButton(SWITCH) == PUSHED && current_heli_state == LANDED)
    {
        current_heli_state = TAKEOFF;
    }else if (checkButton(SWITCH) == RELEASED && current_heli_state == FLYING)
    {
        current_heli_state = LANDING;
    }
}


void transition_task(void)
{
    switch(current_heli_state)
    {
        case TAKEOFF:
            //lift off by 1% :)
            change_altitude(alt_val_to_percent(initial_ADC_val, current_ADC_val), 10);
            current_heli_state = FLYING;
            break;
        case LANDING:
            change_altitude(alt_val_to_percent(initial_ADC_val, current_ADC_val), -100);
            if (alt_val_to_percent(initial_ADC_val, current_ADC_val) == 0)
            {
                kill_motors(&current_heli_state);
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
        current_heli_state = FLYING;
        change_altitude(alt_val_to_percent(initial_ADC_val, current_ADC_val), 10);
    }
}
//
//void get_sensor_values_task(void)
//{
//    current_ADC_val = get_ADC_val(&g_inBuffer, BUF_SIZE);
//}


int main(void)
{
    //Initialise peripherals etc
    initialise_program();

    // Kill motors for software reset
    kill_motors(&heli_state);

    // Perform the task registration for the protokernal
    register_all_pk_tasks();

    IntMasterEnable();

    // Read in relevant peripheral values
    int32_t prev_switch_state = GPIOPinRead (SWITCH_PORT_BASE, SWITCH_PIN) == SWITCH_PIN;
    int32_t initial_ADC_val = get_ADC_val(&g_inBuffer, BUF_SIZE);

    unsigned char increase_alt_task_ID = pK_register_task(increase_altitude_task, 0);
    unsigned char decrease_alt_task_ID = pK_register_task(decrease_altitude_task, 0);

    unsigned char buttons_task_ID = pK_register_task(buttons_task, 0);
    unsigned char transition_task_ID = pK_register_task(transition_task, 0);
    //unsigned char get_sensor_values_task_ID = pK_register_task(get_sensor_values_task, 0);
    unsigned char print_task_ID = pK_register_task(print_task, 1);
    unsigned char get_ADC_task_ID = pK_register_task(get_ADC_task, 0);


    pK_ready_task(get_ADC_task_ID);
    pK_ready_task(transition_task_ID);
    pK_ready_task(decrease_alt_task_ID);
    pK_ready_task(buttons_task_ID);
    pK_ready_task(increase_alt_task_ID);
    //pK_ready_task(get_sensor_values_task_ID);
    pK_ready_task(print_task_ID);

    pK_start();

    while (1)
    {


        // Background task: calculate the (approximate) mean of the values in the circular buffer and display it, together with the sample number.
        current_switch_state = GPIOPinRead (SWITCH_PORT_BASE, SWITCH_PIN) == SWITCH_PIN;

        switch(heli_state)
        {
            case LANDED:
                // set rotor and tail motors to zero
                switch_task();
                break;
            case TAKEOFF:
                // helicopter calibrates to reference yaw when take off switch pressed
                found_yaw = find_yaw_ref();

                break;
            case FLYING:
                // helicopter doesnt spaz when both yaw and altitude pressed consecutively
                // alt in range 0 - 100 and pwm duty in range 2 - 98
                switch_task();
                pushbuttons_task();
                alt_control_task();
                yaw_control_task();
                break;
            case LANDING:
                // When helicopter is landing pressing buttons or switches do nothing
                // helicopter should return to reference yaw and land smoothly
                break;
        }



    }
}

