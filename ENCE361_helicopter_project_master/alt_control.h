#ifndef ALT_CONTROL_H
#define ALT_CONTROL_H
//*****************************************************************************
// 
//      alt_control.h
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

int32_t get_alt_val(circBuf_t *buffer, uint32_t buf_size);

int32_t alt_vals_to_percent(int32_t initial_alt_val, int32_t current_alt_val);


void decrease_altitude(int32_t alt_percent);
void increase_altitude(int32_t alt_percent);


#endif /*ALT_CONTROL_H*/
