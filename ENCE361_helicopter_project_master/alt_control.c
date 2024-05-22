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

#define FLOAT_CONVERSION_FACTOR 10
#define Kp 1.0 * FLOAT_CONVERSION_FACTOR
#define Ki 1.0 * FLOAT_CONVERSION_FACTOR
#define Kd 1.0 * FLOAT_CONVERSION_FACTOR
#define Kg 33.3 * FLOAT_CONVERSION_FACTOR
#define BUF_SIZE 10



//*****************************************************************************************************
// get_ADC_val: returns the average ADC value from the values stored in the circular buffer structure
//*****************************************************************************************************
int32_t get_alt_val(circBuf_t *buffer)
{
    int32_t sum = 0;
    int32_t i = 0;

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
    return (330 * (initial_alt_val - current_alt_val)) / 4095; // replace these vals with names
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

    rotor_PWM = controller (desired_alt_percent, current_alt_percent, Kp, Ki, Kd, Kg, FLOAT_CONVERSION_FACTOR);

    //set_main_PWM (rotor_PWM);
}
