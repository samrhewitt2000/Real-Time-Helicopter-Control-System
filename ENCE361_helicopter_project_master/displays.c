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
#include "circ_buffer.h"
#include "OrbitOLED/OrbitOLEDInterface.h"
#include "ADC.h"
#include "buttons.h"
#include "displays.h"
#include "yaw_control.h"


//*****************************************************************************
//
//*****************************************************************************
void init_display (void)
{
    // intialise the Orbit OLED display
    OLEDInitialise ();
}



//*****************************************************************************
//
//*****************************************************************************
void display_nothing(void)
{
    OLEDStringDraw("                ", 0, 0);
    OLEDStringDraw("                ", 0, 1);
    OLEDStringDraw("                ", 0, 2);
    OLEDStringDraw("                ", 0, 3);
}



//*****************************************************************************
//
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
//
//*****************************************************************************
void display_alt_percent(int32_t alt_percent, uint32_t display_col, uint32_t display_row)
{
    char string[17];

    usnprintf (string, sizeof(string), "Alt: %2d %%  ", alt_percent);

    OLEDStringDraw (string, display_col, display_row);
}



//*****************************************************************************
//
//*****************************************************************************
void display_yaw(uint32_t display_col, uint32_t display_row, int32_t yaw_angle_int, int32_t yaw_angle_decimal)
{
    char string[17];

    if (yaw_angle_int < 0) 
    {
        usnprintf (string, sizeof(string), "Yaw: -%d.%d Deg  ", abs(yaw_angle_int), yaw_angle_decimal);
    }
    else 
    {
        usnprintf (string, sizeof(string), "Yaw:  %d.%d Deg  ", yaw_angle_int, yaw_angle_decimal);
    }

    OLEDStringDraw (string, display_col, display_row);
}



//*****************************************************************************
//
//*****************************************************************************
void display_main_duty_cycle(int32_t duty_cycle, uint32_t display_col, uint32_t display_row)
{
    char string[7];
    usnprintf(string, sizeof(string), "Main duty Cycle: %2d %%  ", duty_cycle);
    OLEDStringDraw(string, display_col, display_row);
}



//*****************************************************************************
//
//*****************************************************************************
void display_tail_duty_cycle(int32_t tail_duty_cycle, uint32_t display_col, uint32_t display_row)
{
    char string[7];
    usnprintf(string, sizeof(string), "Tail duty Cycle: %2d %% ", tail_duty_cycle);
    OLEDStringDraw(string, display_col, display_row);
}
