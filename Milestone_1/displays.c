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
displayADCVal(uint32_t ADC_val)
{
    displayNothing();
    char string[17];  // 16 characters across the display

    // Form a new string for the line.  The maximum width specified for the
    //  number field ensures it is displayed right justified.
    usnprintf (string, sizeof(string), "Mean ADC: %3d", ADC_val);
    // Update line on display.
    OLEDStringDraw (string, 0, 1);
}


void
displayAltitudePerc(uint32_t current_ADC_val, uint32_t initial_ADC_val)
{
    displayNothing();
    char string[17];

    uint32_t altitude_percent;
    if (current_ADC_val < initial_ADC_val) {
        altitude_percent = ((current_ADC_val - initial_ADC_val) / 4095 - initial_ADC_val);
    } else {
        altitude_percent = 0;
    }
    usnprintf (string, sizeof(string), "Alt: %2d percent", altitude_percent);
    OLEDStringDraw(string, 0, 0);
}
