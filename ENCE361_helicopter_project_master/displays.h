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

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/adc.h"
#include "driverlib/pwm.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/interrupt.h"
#include "driverlib/debug.h"
#include "utils/ustdlib.h"
#include "OrbitOLED/OrbitOLEDInterface.h"

#include "circ_buffer.h"
#include "ADC.h"
#include "buttons.h"
#include "PWM.h"
#include "quad_enc.h"

extern uint32_t main_duty_cycle;
extern uint32_t tail_duty_cycle;

// *******************************************************
// initDisplay: Initialise the display
// *******************************************************
void initDisplay (void);


//*****************************************************************************
//displayNothing: clears screen and displays nothing
//*****************************************************************************
void displayNothing(void);

//*****************************************************************************
//displayADCVal: Displays the current value of the ADC on the OLED display
//*****************************************************************************
void displayADCVal(int32_t ADC_val, uint32_t display_col, uint32_t display_row);

//*****************************************************************************
//displayAltitudePerc: Displays the current altitude on the OLED display
//*****************************************************************************
void displayAltitudePerc(int32_t current_ADC_val, int32_t initial_ADC_val, uint32_t display_col, uint32_t display_row);

//*****************************************************************************
//displayYaw: Displays the current yaw angle on the OLED display
//*****************************************************************************
void displayYaw(uint32_t display_col, uint32_t display_row);

//*****************************************************************************
//display_rotor_PWM: Displays the current PWM frequency                            CHANGE THIS
//*****************************************************************************
void display_rotor_PWM(uint32_t display_col, uint32_t display_row, uint32_t ui32Freq);

//void display_rotor_duty_cycle(int32_t duty_cycle, uint32_t display_col, uint32_t display_row);

//void display_tail_duty_cycle(int32_t tail_duty_cycle, uint32_t display_col, uint32_t display_row);

void final_displays(helicopter_state_t heli_state);

#endif /*DISPLAYS_H_*/
