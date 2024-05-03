//*****************************************************************************
//
// displays.c - LED display logic for the Tiva board
//
// Author:  Caleb Westbury & Sam Hewitt
// Author:  Caleb Westbury & Sam Hewitt
// Last modified:   22/03/2024
//
//*****************************************************************************
// Based on the 'convert' series from 2016
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
#include "circBuffer.h"
#include "OrbitOLED/OrbitOLEDInterface.h"

#include "ADC.h"
#include "buttons5.h"
#include "displays.h"
#include "yaw.h"
#include "yaw.h"


//*****************************************************************************
//
// Function to display the mean ADC value (10-bit value, note) and sample count.
//
//*****************************************************************************

void
initDisplay (void)
{
    // intialise the Orbit OLED display
    OLEDInitialise ();
}

void
displayNothing(void)
{
    OLEDStringDraw("                ", 0, 0);
    OLEDStringDraw("                ", 0, 1);
    OLEDStringDraw("                ", 0, 2);
    OLEDStringDraw("                ", 0, 3);
}

void
displayADCVal(int32_t ADC_val, uint32_t display_col, uint32_t display_row)
{

    char string[17];  // 16 characters across the display

    // Form a new string for the line.  The maximum width specified for the
    //  number field ensures it is displayed right justified.
    usnprintf (string, sizeof(string), "Mean ADC: %3d", ADC_val);
    // Update line on display.
    OLEDStringDraw (string, display_col, display_row);
}


void
displayAltitudePerc(int32_t current_ADC_val, int32_t initial_ADC_val, uint32_t display_col, uint32_t display_row)
{

    char string[17];

    int32_t altitude_percent;

    altitude_percent = (330 * (initial_ADC_val - current_ADC_val)) / 4095;
    usnprintf (string, sizeof(string), "Alt: %2d %%  ", altitude_percent);
    OLEDStringDraw (string, display_col, display_row);
}

void
displayYaw(uint32_t display_col, uint32_t display_row)
{

    char string[17];

    yaw_angle_decimal = abs(((360 * yaw_ticks) % 448 * 10) / 448);

    int32_t yaw_angle_int = 360 * yaw_ticks / 448;

    usnprintf (string, sizeof(string), "Yaw: %d.%d Deg  ", yaw_angle_int, yaw_angle_decimal);
    OLEDStringDraw (string, display_col, display_row);

    
}
