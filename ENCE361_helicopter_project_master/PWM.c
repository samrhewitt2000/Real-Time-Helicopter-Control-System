//*****************************************************************************
// 
//      PWM.c
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
// Based on P.J. Bones' pwmGen.c code from 2018
//
//*****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/pin_map.h" //Needed for pin configure
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/pwm.h"
#include "driverlib/systick.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"

#include "buttons.h"
#include "PWM.h"

/**********************************************************
 * Generates a single PWM signal on Tiva board pin J4-05 =
 * PC5 (M0PWM7).  This is the same PWM output as the
 * helicopter main rotor.
 **********************************************************/

/***********************************************************
 * ISR for the SysTick interrupt (used for button debouncing).
 ***********************************************************/
//void
//SysTickIntHandler (void)
//{
//    //
//    // Poll the buttons
//    updateButtons();
//    //
//    // It is not necessary to clear the SysTick interrupt.
//}



/***********************************************************
 * Initialisation functions: clock, SysTick, PWM
 ***********************************************************
 * Clock
 ***********************************************************/
void initClocks (void)
{
    // Set the clock rate to 20 MHz
    SysCtlClockSet (SYSCTL_SYSDIV_10 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |
                   SYSCTL_XTAL_16MHZ);

    // Set the PWM clock rate (using the prescaler)
    SysCtlPWMClockSet(PWM_DIVIDER_CODE);
}



/*************************************************************
 * SysTick interrupt
 ************************************************************/
void initSysTick (void)
{
    //
    // Set up the period for the SysTick timer.  The SysTick
    // timer period is set as a function of the system clock.
    SysTickPeriodSet (SysCtlClockGet() / SYSTICK_RATE_HZ);
    //
    // Register the interrupt handler
    SysTickIntRegister (SysTickIntHandler);
    //
    // Enable interrupt and device
    SysTickIntEnable ();
    SysTickEnable ();
}



/*********************************************************
 * initialise_rotor_PWM
 * M0PWM7 (J4-05, PC5) is used for the main rotor motor
 *********************************************************/
void initialise_rotor_PWM (void)
{
    SysCtlPeripheralEnable(PWM_MAIN_PERIPH_PWM);
    SysCtlPeripheralEnable(PWM_MAIN_PERIPH_GPIO);

    GPIOPinConfigure(PWM_MAIN_GPIO_CONFIG);
    GPIOPinTypePWM(PWM_MAIN_GPIO_BASE, PWM_MAIN_GPIO_PIN);

    PWMGenConfigure(PWM_MAIN_BASE, PWM_MAIN_GEN,
                    PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC);
    // Set the initial PWM parameters
    set_rotor_PWM (PWM_START_RATE_HZ, PWM_FIXED_DUTY);

    PWMGenEnable(PWM_MAIN_BASE, PWM_MAIN_GEN);

    // Disable the output.  Repeat this call with 'true' to turn O/P on.
    PWMOutputState(PWM_MAIN_BASE, PWM_MAIN_OUTBIT, false);
}



/*********************************************************
 * initialise_tail_PWM
 * M1PWM5 (J3-10, PF1) is used for the tail rotor motor
 *********************************************************/
void initialise_tail_PWM(void)
{
    // Enable PWM peripheral and GPIO peripheral for tail rotor
    SysCtlPeripheralEnable(PWM_TAIL_PERIPH_PWM);
    SysCtlPeripheralEnable(PWM_TAIL_PERIPH_GPIO);

    // Configure pin for PWM output
    GPIOPinConfigure(PWM_TAIL_GPIO_CONFIG);
    GPIOPinTypePWM(PWM_TAIL_GPIO_BASE, PWM_TAIL_GPIO_PIN);

    // Configure PWM generator for tail rotor
    PWMGenConfigure(PWM_TAIL_BASE, PWM_TAIL_GEN, PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC);

    // Set the initial PWM parameters for tail rotor
    set_tail_PWM(PWM_START_RATE_HZ, PWM_FIXED_DUTY);

    // Enable PWM generator for tail rotor
    PWMGenEnable(PWM_TAIL_BASE, PWM_TAIL_GEN);

    // Disable the output initially, can be enabled later
    PWMOutputState(PWM_TAIL_BASE, PWM_TAIL_OUTBIT, false);
}

// ^fix terminology? rotor/motor?^

/********************************************************
 * Function to set the freq, duty cycle of M0PWM7 (main motor)
 ********************************************************/
void set_rotor_PWM (uint32_t ui32Freq, uint32_t ui32Duty)
{
    // Calculate the PWM period corresponding to the freq.
    uint32_t ui32Period =
        SysCtlClockGet() / PWM_DIVIDER / ui32Freq;

    PWMGenPeriodSet(PWM_MAIN_BASE, PWM_MAIN_GEN, ui32Period);
    PWMPulseWidthSet(PWM_MAIN_BASE, PWM_MAIN_OUTNUM,
        ui32Period * ui32Duty / 100);
}



/********************************************************
 * Function to set the freq, duty cycle of M1PWM5 (tail motor)
 ********************************************************/
void set_tail_PWM(uint32_t ui32Freq, uint32_t ui32Duty)
{
    // Calculate the PWM period corresponding to the freq.
    uint32_t ui32Period = SysCtlClockGet() / PWM_DIVIDER / ui32Freq;

    // Configure the PWM period and duty cycle
    PWMGenPeriodSet(PWM_TAIL_BASE, PWM_TAIL_GEN, ui32Period);
    PWMPulseWidthSet(PWM_TAIL_BASE, PWM_TAIL_OUTNUM, ui32Period * ui32Duty / 100);
}


/********************************************************
 * Function to set the freq, duty cycle of M1PWM5 (tail motor)
 ********************************************************/
void kill_motors(void)
{
    PWMGenPeriodSet(PWM_MAIN_BASE, PWM_MAIN_GEN, 0);
    PWMPulseWidthSet(PWM_MAIN_BASE, PWM_MAIN_OUTNUM, 0);

    PWMGenPeriodSet(PWM_TAIL_BASE, PWM_TAIL_GEN, 0);
    PWMPulseWidthSet(PWM_TAIL_BASE, PWM_TAIL_OUTNUM, 0);
}

//int
//main (void)
//{
//    uint32_t ui32Freq = PWM_START_RATE_HZ;
//
//    initClocks ();
//
//    // As a precaution, make sure that the peripherals used are reset
//    SysCtlPeripheralReset (PWM_MAIN_PERIPH_GPIO); // Used for PWM output
//    SysCtlPeripheralReset (PWM_MAIN_PERIPH_PWM);  // Main Rotor PWM
//    SysCtlPeripheralReset (UP_BUT_PERIPH);        // UP button GPIO
//    SysCtlPeripheralReset (DOWN_BUT_PERIPH);      // DOWN button GPIO
//
//    initButtons ();  // Initialises 4 pushbuttons (UP, DOWN, LEFT, RIGHT)
//    initialisePWM ();
//    initSysTick ();
//
//    // Initialisation is complete, so turn on the output.
//    PWMOutputState(PWM_MAIN_BASE, PWM_MAIN_OUTBIT, true);
//
//    //
//    // Enable interrupts to the processor.
//    IntMasterEnable ();
//
//    //
//    // Loop forever, controlling the PWM frequency and
//    // maintaining the the PWM duty cycle.
//    while (1)
//    {
//        // Background task: Check for button pushes and control
//        // the PWM frequency within a fixed range.
//        if ((checkButton (UP) == PUSHED) && (ui32Freq < PWM_RATE_MAX_HZ))
//        {
//            ui32Freq += PWM_RATE_STEP_HZ;
//            setPWM (ui32Freq, PWM_FIXED_DUTY);
//        }
//        if ((checkButton (DOWN) == PUSHED) && (ui32Freq > PWM_RATE_MIN_HZ))
//        {
//            ui32Freq -= PWM_RATE_STEP_HZ;
//            setPWM (ui32Freq, PWM_FIXED_DUTY);
//        }
//    }
//}
