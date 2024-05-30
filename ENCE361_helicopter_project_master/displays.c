//*****************************************************************************
// 
//      displays.c
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
#include "displays.h"
#include "quad_enc.h"
#include "alt_control.h"

//*****************************************************************************
//
//*****************************************************************************
void initDisplay (void)
{

    // intialise the Orbit OLED display
    OLEDInitialise();
}



//*****************************************************************************
//displayNothing: clears screen and displays nothing
//*****************************************************************************
void displayNothing(void)
{
    OLEDStringDraw("                ", 0, 0);
    OLEDStringDraw("                ", 0, 1);
    OLEDStringDraw("                ", 0, 2);
    OLEDStringDraw("                ", 0, 3);
}



//*****************************************************************************
//displayADCVal: Displays the current value of the ADC on the OLED display
//*****************************************************************************
void displayADCVal(int32_t ADC_val, uint32_t display_col, uint32_t display_row)
{
    char string[17];  // 16 characters across the display

    // Form a new string for the line.  The maximum width specified for the
    //  number field ensures it is displayed right justified.
    usnprintf (string, sizeof(string), "Mean ADC: %3d", ADC_val);
    // Update line on display.
    OLEDStringDraw (string, display_col, display_row);
}



//*****************************************************************************
//displayAltitudePerc: Displays the current altitude on the OLED display
//*****************************************************************************
void displayAltitudePerc(int32_t current_ADC_val, int32_t initial_ADC_val, uint32_t display_col, uint32_t display_row)
{
    char string[17];

    int32_t altitude_percent;

    altitude_percent = alt_val_to_percent(initial_ADC_val, current_ADC_val);
    usnprintf (string, sizeof(string), "Alt: %2d %%  ", altitude_percent);
    OLEDStringDraw (string, display_col, display_row);
}



//*****************************************************************************
//displayYaw: Displays the current yaw angle on the OLED display
//*****************************************************************************
void displayYaw(uint32_t display_col, uint32_t display_row)
{
    char string[17];

    yaw_angle_decimal = abs(((360 * quad_enc_ticks) % 448 * 10) / 448);

    int32_t yaw_angle_int = abs(360 * quad_enc_ticks / 448);

    if (quad_enc_ticks < 0) {
        usnprintf (string, sizeof(string), "Yaw: -%d.%d Deg  ", yaw_angle_int, yaw_angle_decimal);
    }
    else {
        usnprintf (string, sizeof(string), "Yaw:  %d.%d Deg  ", yaw_angle_int, yaw_angle_decimal);
    }

    OLEDStringDraw (string, display_col, display_row);
}



//*****************************************************************************
//display_rotor_PWM: Displays the current PWM frequency                            CHANGE THIS
//*****************************************************************************
void display_rotor_PWM(uint32_t display_col, uint32_t display_row, uint32_t ui32Freq)
{
    char string[17];

    usnprintf (string, sizeof(string), "R_PWM: %2d %%  ", ui32Freq);
    OLEDStringDraw (string, display_col, display_row);
}



void display_task(void)
{
    displayYaw(0, 3);
    displayAltitudePerc(get_ADC_val(&g_inBuffer, BUF_SIZE), initial_ADC_val, 0, 0);
}

