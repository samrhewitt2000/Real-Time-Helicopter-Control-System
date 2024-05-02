#ifndef DISPLAYS_H_
#define DISPLAYS_H_

//*****************************************************************************
//
// displays.c - LED display logic for the Tiva board
//
// Author:  Caleb Westbury & Sam
// Last modified:   22/03/2024
//
//*****************************************************************************
// Based on the 'convert' series from 2016
//*****************************************************************************

// *******************************************************
// initDisplay: Initialise the display
void
initDisplay (void);

void
displayNothing(void);

void
displayADCVal(int32_t ADC_val, uint32_t display_col, uint32_t display_row);

void
displayAltitudePerc(int32_t current_ADC_val, int32_t initial_ADC_val, uint32_t display_col, uint32_t display_row);

void
displayYaw(int32_t current_ADC_val, int32_t initial_ADC_val, uint32_t display_col, uint32_t display_row);

#endif /*DISPLAYS_H_*/
