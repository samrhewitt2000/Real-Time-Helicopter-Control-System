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



int main(void)
{
    int32_t initial_ADC_val = 0;    // initialize first value
    int32_t current_ADC_val = 0;    // initialize first value
    
    // Enable interrupts to the processor.
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
    uint32_t ui32RotorFreq = PWM_START_RATE_HZ;
    uint32_t ui32RotorDuty = PWM_FIXED_DUTY;
    uint32_t ui32TailFreq = PWM_START_RATE_HZ;
    uint32_t ui32TailDuty = PWM_FIXED_DUTY;
    initCircBuf (&g_inBuffer, BUF_SIZE);

    //SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
//    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD_WPD);
//    GPIODirModeSet(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_DIR_MODE_OUT);


    // calculate exactly how long this needs to be
    SysCtlDelay (SysCtlClockGet() / 6); // delay so that buffer can fill
    initial_ADC_val = get_ADC_val(&g_inBuffer, BUF_SIZE);

    //prev_phase = get_current_phase();

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

        switch(current_state)
        {
        case STATE_PERC:
            displayAltitudePerc(current_ADC_val, initial_ADC_val, 0, 0);
            //GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);
            //SysCtlDelay(SysCtlClockGet() / yaw_angle);
            //GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0x00);
            displayYaw(0, 1);
            display_rotor_PWM(0, 2, ui32TailDuty); // using for debugging

            // using for debugging
            char string[17];
            usnprintf (string, sizeof(string), "state: %2d %%  ", current_heli_state);
            OLEDStringDraw (string, 0, 4);

            break;
        case STATE_MEAN_ADC_VAL:
            // Calculate and display the rounded mean of the buffer contents
            displayADCVal (current_ADC_val, 0, 1);
            break;
        case STATE_OFF:
            displayNothing();
            break;
        }

        SysCtlDelay (SysCtlClockGet() / 24);  // Update display at ~ 2 Hz

    }
}

