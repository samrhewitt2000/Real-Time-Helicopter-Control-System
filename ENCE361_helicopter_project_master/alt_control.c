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
#include "circ_buffer.h"
#include "PID.h"
#include <stdint.h>
#include <stdbool.h>
#include "driverlib/PWM.h"
#include "ADC.h"
#include "buttons.h"

//set K values     NEED CHANGING
#define Kp 100
#define Ki 100
#define Kd 100
#define Kc 0

#define BUF_SIZE 10

//*****************************************************************************************************
// get_ADC_val: returns the average ADC value from the values stored in the circular buffer structure
//*****************************************************************************************************
int32_t get_alt_val(circBuf_t *buffer)
{
    int32_t sum = 0;

    for (i = 0; i < BUF_SIZE; i++)
    {
        sum += readCircBuf (buffer);
    }
    
    return (2 * sum + BUF_SIZE) / 2 / BUF_SIZE;
}



int32_t alt_vals_to_percent(int32_t initial_alt_val, int32_t current_alt_val)
{
    return (330 * (initial_alt_val - current_alt_val)) / 4095; // replace these vals with names
}




void increase_altitude(int32_t alt_percent)
{

    //get altitude
    int32_t current_alt_val = get_alt_val(*buffer, buf_size);
    int32_t current_alt_percent = alt_vals_to_percent(current_alt_val);

    int32_t setpoint = (current alt _percent + 10);
    if setpoint > 100 //check not greater than 100
    {
        setpoint = 100;
    }

    //calculate control
    control = controller (setpoint, int32_t sensor_reading, Kp, Ki, Kd, Kc);

    //send to motors
    set_main_PWM (control/10);//control is divided by 100 within PWM function so divide by 10?

}

void decrease_altitude(int32_t alt_percent)
{


    //get altitude
    int32_t current_alt_val = get_alt_val(*buffer, buf_size);
    int32_t current_alt_percent = alt_vals_to_percent(current_alt_val);

    int32_t setpoint = (current alt _percent - 10);
    if setpoint < 0
    {
        setpoint = 0;
    }

    //calculate control
    control = controller (setpoint, int32_t sensor_reading, Kp, Ki, Kd, Kc);

    //send to PWM and motors
    set_main_PWM (control / 10); //control is divided by 100 within PWM function so divide by 10?
}


int32_t return_main_control




