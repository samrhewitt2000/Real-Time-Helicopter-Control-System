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

#define "alt_control.h"

//*****************************************************************************************************
// get_ADC_val: returns the average ADC value from the values stored in the circular buffer structure
//*****************************************************************************************************
int32_t get_alt_val(circBuf_t *buffer, uint32_t buf_size)
{
    int32_t sum = 0;

    for (i = 0; i < buf_size; i++)
    {
        sum += readCircBuf (buffer);
    }
    
    return (2 * sum + buf_size) / 2 / buf_size;
}

int32_t alt_vals_to_percent(int32_t initial_alt_val, int32_t current_alt_val)
{
    return (330 * (initial_alt_val - current_alt_val)) / 4095; // replace these vals with names
}