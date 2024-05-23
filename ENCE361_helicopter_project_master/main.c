//*****************************************************************************
// 
//      main.c
//
// What does this module do? (Replace)
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

#include "PWM.h"
#include "circ_buffer.h"
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
#include "yaw_control.h"
#include "displays.h"
#include "ADC.h"
#include "buttons.h"
#include "inc/hw_ints.h"
#include "communications.h"
#include "alt_control.h"
#include "PID.h"
#include "quad_enc.h"

#define BUF_SIZE 10



//*****************************************************************************
//
//*****************************************************************************
void init_system(void)
{
    // Enable interrupts to the processor.
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF))
    {
    }
    initButtons();
    initClock ();
    initADC ();
    init_display();
    initCircBuf (&g_inBuffer, BUF_SIZE);
}



//*****************************************************************************
//
//*****************************************************************************
int main(void)
 {

    int32_t current_alt_percent;


    // calculate exactly how long this needs to be
    SysCtlDelay (SysCtlClockGet() / 6); // delay so that buffer can fill
    int32_t initial_ADC_val = get_alt_val(&g_inBuffer);
    int32_t current_ADC_val = initial_ADC_val;
    helicopter_state_t current_heli_state = LANDED;

    IntMasterEnable();



    while (1)
    {
        current_ADC_val = get_alt_val(&g_inBuffer);
        current_alt_percent = alt_val_to_percent(initial_ADC_val, current_ADC_val);

        display_main_duty_cycle(*ptr_main_rotor_duty, 0, 0);
        display_tail_duty_cycle(*ptr_tail_rotor_duty, 0, 1);

        display_alt_percent(current_alt_percent, 0, 2);
        display_yaw(0, 3, yaw_angle_int, yaw_angle_decimal);

        updateButtons();

        switch(current_heli_state)
        {
            case LANDED:
                set_rotor_PWM(0);
                if (checkButton(SWITCH) == PUSHED)
                {
                    current_heli_state = TAKEOFF;
                }
                break;
            case LANDING:
                change_altitude(current_alt_percent, -100);
                if (current_alt_percent == 0)
                {
                    current_heli_state = LANDED;
                }
                break;
            case TAKEOFF:
                change_altitude(current_alt_percent, 1);
                break;
            case FLYING:
                if (checkButton(SWITCH) == RELEASED)
                {
                    current_heli_state = LANDING;
                } else if (checkButton(UP) == PUSHED)
                {
                    change_altitude(current_alt_percent, 10);

                } else if (checkButton(DOWN) == PUSHED)
                {
                    change_altitude(current_alt_percent, -10);
                }
                break;
        }

        SysCtlDelay (SysCtlClockGet() / 48);  // Update at ~ 4 Hz            ~ 2 Hz = / 24
    }
}


