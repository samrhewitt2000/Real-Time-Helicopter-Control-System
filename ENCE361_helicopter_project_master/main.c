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

#include "inc/hw_ints.h"

#include "circ_buffer.h"
#include "quad_enc.h"
#include "displays.h"
#include "ADC.h"
#include "buttons.h"
#include "PWM.h"

//*****************************************************************************
// Constants
//*****************************************************************************
#define BUF_SIZE 10

// *******************************************************
// Display state enum
// *******************************************************
typedef enum {
    STATE_PERC,             // display altitude percentages state
    STATE_MEAN_ADC_VAL,     // display mean adc values state
    STATE_OFF,              // screen off state
} display_state_t;

// *******************************************************
// Helicopter state enum
// *******************************************************
typedef enum {
    LANDED,
    TAKEOFF,
    FLYING,
    LANDING
} helicopter_state_t;


//********************************************************
// Function to set the freq, duty cycle of M1PWM5 (tail motor)
// ********************************************************
void kill_motors(helicopter_state_t *current_heli_state)
{
    //set main motor PWM signal to zero
    PWMGenPeriodSet(PWM_MAIN_BASE, PWM_MAIN_GEN, 0);
    PWMPulseWidthSet(PWM_MAIN_BASE, PWM_MAIN_OUTNUM, 0);

    //set tail motor PWM signal to zero
    PWMGenPeriodSet(PWM_TAIL_BASE, PWM_TAIL_GEN, 0);
    PWMPulseWidthSet(PWM_TAIL_BASE, PWM_TAIL_OUTNUM, 0);

    *current_heli_state = LANDED;
}



//********************************************************
//
// ********************************************************
void initialise_program(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF))
    {
    }
    initButtons();
    initClocks ();
    initADC ();
    initDisplay ();
    initYaw ();
    initialise_rotor_PWM ();
    initialise_tail_PWM ();
    PWMOutputState(PWM_MAIN_BASE, PWM_MAIN_OUTBIT, true);
    PWMOutputState(PWM_TAIL_BASE, PWM_TAIL_OUTBIT, true);
    initSysTick ();
}



int main(void)
{
    int32_t initial_ADC_val = 0;    // initialize first value
    int32_t current_ADC_val = 0;    // initialize first value

    uint32_t ui32RotorFreq = PWM_START_RATE_HZ;
    uint32_t ui32RotorDuty = PWM_FIXED_DUTY;
    uint32_t ui32TailFreq = PWM_START_RATE_HZ;
    uint32_t ui32TailDuty = PWM_FIXED_DUTY;
    initCircBuf (&g_inBuffer, BUF_SIZE);

    // calculate exactly how long this needs to be
    SysCtlDelay (SysCtlClockGet() / 6); // delay so that buffer can fill
    initial_ADC_val = get_ADC_val(&g_inBuffer, BUF_SIZE);

    display_state_t current_state = STATE_PERC; //initialize display state
    helicopter_state_t current_heli_state = FLYING; //initialize display state
    int32_t prev_switch_state = GPIOPinRead (SWITCH_PORT_BASE, SWITCH_PIN) == SWITCH_PIN;
    int32_t current_switch_state;

    kill_motors(&current_heli_state);

    IntMasterEnable();

    while (1)
    {
        //
        // Background task: calculate the (approximate) mean of the values in the
        // circular buffer and display it, together with the sample number.
        current_ADC_val = get_ADC_val(&g_inBuffer, BUF_SIZE);

//        if (checkButton(LEFT) == PUSHED) {
//            initial_ADC_val = current_ADC_val;
//        }

        if ((checkButton (UP) == PUSHED) && (ui32RotorDuty < PWM_MAX_DUTY ))
        {
            ui32RotorDuty += 10;
            set_rotor_PWM (ui32RotorFreq, ui32RotorDuty);
        }
        if ((checkButton (DOWN) == PUSHED) && (ui32RotorDuty > PWM_MIN_DUTY ))
        {
            ui32RotorDuty -= 10;
            set_rotor_PWM (ui32RotorFreq, ui32RotorDuty);
        }

        if ((checkButton (RIGHT) == PUSHED) && (ui32TailDuty < PWM_MAX_DUTY ))
        {
            ui32TailDuty += 10;
            set_tail_PWM (ui32TailFreq, ui32TailDuty);
        }
        if ((checkButton (LEFT) == PUSHED) && (ui32TailDuty > PWM_MIN_DUTY ))
        {
            ui32TailDuty -= 10;
            set_tail_PWM (ui32TailFreq, ui32TailDuty);
        }

        current_switch_state = GPIOPinRead (SWITCH_PORT_BASE, SWITCH_PIN) == SWITCH_PIN;
        // Increment current_heli_state when the switch goes down (low)
        if (current_switch_state != prev_switch_state)
        {
            if (current_heli_state == LANDED && current_switch_state == SWITCH_NORMAL)
            {
                current_heli_state = TAKEOFF;
            }
            else if (current_heli_state == FLYING && current_switch_state != SWITCH_NORMAL)
            {
                current_heli_state = LANDING;
            }
            prev_switch_state = current_switch_state;
        }

        switch(current_heli_state)
        {
            case LANDED:
                if (current_switch_state != prev_switch_state)
                {

                }
                break;
            case TAKEOFF:
                break;
            case FLYING:
                break;
            case LANDING:
                break;
        }

        SysCtlDelay (SysCtlClockGet() / 24);  // Update display at ~ 2 Hz

    }
}

