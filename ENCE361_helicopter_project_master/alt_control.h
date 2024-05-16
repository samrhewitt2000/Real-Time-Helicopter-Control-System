#ifndef ALT_CONTROL_H_
#define ALT_CONTROL_H_

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

#include "circ_buffer.h"
#include <stdint.h>



int32_t get_alt_val(circBuf_t *buffer);

int32_t alt_val_to_percent(int32_t initial_alt_val, int32_t current_alt_val);

void change_altitude(int32_t current_alt_percent, int32_t alt_percent_change);

#endif /*ALT_CONTROL_H_*/
