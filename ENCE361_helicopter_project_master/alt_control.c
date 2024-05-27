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
//
// Based on AUTHOR's FILENAME.c code from YEAR (replace bold if applicable otherwise delete)
//
//*****************************************************************************

#include "alt_control.h"
#include "PWM.h"
#include "circ_buffer.h"
#include "PID.h"
#include "kernel.h"

#define FLOAT_CONVERSION_FACTOR 100
#define Kp 1.0 * FLOAT_CONVERSION_FACTOR
#define Ki 1.0 * FLOAT_CONVERSION_FACTOR
#define Kd 1.0 * FLOAT_CONVERSION_FACTOR
#define Kg 33.3 * FLOAT_CONVERSION_FACTOR
#define BUF_SIZE 10
//#define change perc

//volatile int32_t main_rotor_PWM = 0;
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
        sum += readCircBuf (buffer);
    }

    return (2 * sum + BUF_SIZE) / 2 / BUF_SIZE;
}



//*****************************************************************************
//
//*****************************************************************************
int32_t alt_val_to_percent(int32_t initial_alt_val, int32_t current_alt_val)
{
    return ((330 * (initial_alt_val - current_alt_val)) / 4095); // replace these vals with names
}



//*****************************************************************************
//
//*****************************************************************************
void change_altitude(int32_t current_alt_percent, int32_t alt_percent_change)
{
    int32_t desired_alt_percent = current_alt_percent + alt_percent_change;

    if (desired_alt_percent > 100) //check not greater than 100
    {
        desired_alt_percent = 100;
    }
    else if (desired_alt_percent < 0)
    {
        desired_alt_percent = 0;
    }
    int32_t offset = 330;
    //set pwm to control action
    set_rotor_PWM (PWM_START_RATE_HZ ,controller (desired_alt_percent, current_alt_percent, Kp, Ki, Kd, offset, FLOAT_CONVERSION_FACTOR, PWM_MAX_DUTY, PWM_MIN_DUTY));
}

void alt_control_task(void)
{
    // Get current altitude value
    int32_t current_altitude = get_alt_val(&g_inBuffer);

    // Implement altitude control logic here

    // Example: Change altitude by 10% upwards
    change_altitude(current_altitude, 10);

    // Indicate task completion
    //pK_block_task(pK_get_current_task_id());
}
