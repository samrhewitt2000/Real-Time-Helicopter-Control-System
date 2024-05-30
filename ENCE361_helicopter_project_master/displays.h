#ifndef DISPLAYS_H_
#define DISPLAYS_H_

//*****************************************************************************
// 
//      displays.h
//
// Header file for OLED display functions
//
//*****************************************************************************
//
// Author:          Caleb Westbury & Sam Hewitt
// Last modified:   May 2024
//
//*****************************************************************************

#include "circ_buffer.h"    // Circular buffer

extern circBuf_t g_inbuffer;   // External circular buffer

// Function prototypes
void initDisplay(void); // Initializes the display

void displayNothing(void);  // Clears the screen and displays nothing

void displayADCVal(int32_t ADC_val, uint32_t display_col, uint32_t display_row); // Displays ADC value on OLED display

void displayAltitudePerc(int32_t current_ADC_val, int32_t initial_ADC_val, uint32_t display_col, uint32_t display_row); // Displays altitude percentage on OLED display

void displayYaw(uint32_t display_col, uint32_t display_row); // Displays yaw angle on OLED display

void display_rotor_PWM(uint32_t display_col, uint32_t display_row, uint32_t ui32Freq); // Displays rotor PWM frequency on OLED display

void display_task(void); // Task to update display with yaw and altitude information

#endif /* DISPLAYS_H_ */
