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
displayADCVal(uint32_t current_ADC_val);

void
displayAltitudePerc(uint32_t current_ADC_val, uint32_t initial_ADC_Val);

void
displayNothing(void);

#endif /*DISPLAYS_H_*/
