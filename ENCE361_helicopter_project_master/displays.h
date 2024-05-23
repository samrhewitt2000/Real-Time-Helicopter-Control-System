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
void initDisplay (void);

void displayNothing(void);

void displayADCVal(int32_t ADC_val, uint32_t display_col, uint32_t display_row);

void displayAltitudePerc(int32_t current_ADC_val, int32_t initial_ADC_val, uint32_t display_col, uint32_t display_row);

void displayYaw(uint32_t display_col, uint32_t display_row);

void display_rotor_PWM(uint32_t display_col, uint32_t display_row, uint32_t ui32Freq);

void display_rotor_duty_cycle(int32_t duty_cycle, uint32_t display_col, uint32_t display_row);

void display_tail_duty_cycle(int32_t tail_duty_cycle, uint32_t display_col, uint32_t display_row)

#endif /*DISPLAYS_H_*/
