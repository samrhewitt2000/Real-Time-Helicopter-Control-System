//*****************************************************************************
//
//      alt_control.c
//
// Controls the helicopter altitude based on ADC value and button input
//
//*****************************************************************************
//
// Author:          Caleb Westbury & Sam Hewitt
// Last modified:   May 2024
//
//*****************************************************************************


#include "kernel.h"
#include "alt_control.h"
#include "PWM.h"
#include "circ_buffer.h"
#include "PID.h"
#include "displays.h"
#include "ADC.h"


//*****************************************************************************
// Global variables
//*****************************************************************************
#define FLOAT_CONVERSION_FACTOR 100  // conversion factor for floating point calculations
#define Kp 0.1 * FLOAT_CONVERSION_FACTOR  // proportional gain
#define Ki 0.1 * FLOAT_CONVERSION_FACTOR  // integral gain
#define Kd 0.1 * FLOAT_CONVERSION_FACTOR  // derivative gain
#define Kg 33.3 * FLOAT_CONVERSION_FACTOR  // gain constant for altitude control
#define BUF_SIZE 10  // size of the circular buffer

static int32_t current_alt_percent;
volatile int32_t *ptr_current_alt_percent;
volatile int32_t current_altitude = 0;



//*****************************************************************************************************
// get_ADC_val: returns the average ADC value from the values stored in the circular buffer structure
//*****************************************************************************************************
int32_t get_alt_val(circBuf_t *buffer)
{
    int32_t sum = 0;
    int i;
    for (i = 0; i < BUF_SIZE; i++)
    {
        sum += readCircBuf (buffer); //sum the values in the circular buffer
    }

    return (2 * sum + BUF_SIZE) / 2 / BUF_SIZE; //return the average value
}



//*****************************************************************************
//alt_val_to_percent: Converts the ADC value to a percentage of the altitude
//*****************************************************************************
int32_t alt_val_to_percent(int32_t initial_alt_val, int32_t current_alt_val)
{
    // Calculate the altitude percentage
    current_alt_percent = ((330 * (initial_alt_val - current_alt_val)) / 4095);

    // Update the pointer to the current altitude percentage
    ptr_current_alt_percent = &current_alt_percent;  
    return current_alt_percent;  // Return the current altitude percentage
}



//*****************************************************************************
//change_altitude: Changes the altitude by a specified percentage
//*****************************************************************************
void change_altitude(int32_t current_alt_percent, int32_t alt_percent_change)
{
    // Calculate the desired altitude percentage
    int32_t desired_alt_percent = current_alt_percent + alt_percent_change;

    // Ensure the desired altitude percentage is within bounds
    if (desired_alt_percent >= 100)
    {
        desired_alt_percent = 100;
    }
    else if (desired_alt_percent < 0)
    {
        desired_alt_percent = 0;
    }

    int32_t offset = 330;
    // Calculate the control action using the PID controller
    int32_t control_action = controller(desired_alt_percent, *ptr_current_alt_percent, Kp, Ki, Kd, offset, FLOAT_CONVERSION_FACTOR, PWM_MAX_DUTY, PWM_MIN_DUTY);
    // Set the PWM duty cycle to control the rotor
    set_rotor_PWM(PWM_START_RATE_HZ, control_action);
}


//*****************************************************************************
// alt_control_task: Main task for controlling the helicopter altitude
//*****************************************************************************
void alt_control_task(void)
{
    // Get current altitude value
    int32_t current_altitude = get_alt_val(&g_inBuffer);
    
    //convert current altitude to a percentage
    alt_val_to_percent(initial_ADC_val, current_altitude);

    }


