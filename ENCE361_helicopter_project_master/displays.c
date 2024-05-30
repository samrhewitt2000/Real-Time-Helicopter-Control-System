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

#include "displays.h"

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

    altitude_percent = (330 * (initial_ADC_val - current_ADC_val)) / 4095;
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


void final_displays(helicopter_state_t heli_state)
{
    char string1[17];
    char string2[17];
    char string3[17];

    usnprintf (string1, sizeof(string1), "TAIL PWM: %2d %%  ", tail_duty_cycle);
    OLEDStringDraw (string1, 0, 0);
    usnprintf (string2, sizeof(string2), "MAIN PWM: %2d %%  ", main_duty_cycle);
    OLEDStringDraw (string2, 0, 1);
    usnprintf (string3, sizeof(string3), "STATE: %2d        ", heli_state);
    OLEDStringDraw (string3, 0, 2);
}
