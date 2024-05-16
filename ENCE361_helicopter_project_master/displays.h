#ifndef DISPLAYS_H_
#define DISPLAYS_H_

//*****************************************************************************
// 
//      displays.h
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

// *******************************************************
// initDisplay: Initialise the display
// *******************************************************
void init_display (void);

void display_nothing(void);

void displayADCVal(int32_t ADC_val, uint32_t display_col, uint32_t display_row);

void display_alt_percent(int32_t alt_percent, uint32_t display_col, uint32_t display_row);

void display_yaw(uint32_t display_col, uint32_t display_row, int32_t yaw_angle_int, int32_t yaw_angle_decimal);

void display_main_duty_cycle(int32_t duty_cycle, uint32_t display_col, uint32_t display_row);

void display_tail_duty_cycle(int32_t tail_duty_cycle, uint32_t display_col, uint32_t display_row);

#endif /*DISPLAYS_H_*/
