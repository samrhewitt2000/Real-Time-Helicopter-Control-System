//*****************************************************************************
// 
//      PWM.c
//
// Initializes and generates PWM signals to both main and tail rotors
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
#include "PWM.h"
#include "kernel.h"
#include "buttons.h"
#include "PWM.h"



volatile uint32_t *ptr_main_duty_cycle;
volatile uint32_t *ptr_tail_duty_cycle;

static uint32_t main_duty_cycle;
static uint32_t tail_duty_cycle;


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
extern helicopter_state_t heli_state;


///***********************************************************
// * Initialisation functions: clock, SysTick, PWM
// ***********************************************************
// * Clock
// ***********************************************************/
//void initClocks (void)
//{
//    // Set the clock rate to 20 MHz
//    SysCtlClockSet (SYSCTL_SYSDIV_10 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
//
//    // Set the PWM clock rate (using the prescaler)
//    SysCtlPWMClockSet(PWM_DIVIDER_CODE);
//}
//




/*********************************************************
 * initialise_rotor_PWM
 * M0PWM7 (J4-05, PC5) is used for the main rotor
 *********************************************************/
void initialise_rotor_PWM (void)
{
    //enable PWM peripheral and gpio peripheral of tail motor
    SysCtlPeripheralEnable(PWM_MAIN_PERIPH_PWM);
    SysCtlPeripheralEnable(PWM_MAIN_PERIPH_GPIO);

    //configure main motor gpio pins for PWM output
    GPIOPinConfigure(PWM_MAIN_GPIO_CONFIG);
    GPIOPinTypePWM(PWM_MAIN_GPIO_BASE, PWM_MAIN_GPIO_PIN);

    //configure PWM outpus for main motor
    PWMGenConfigure(PWM_MAIN_BASE, PWM_MAIN_GEN, PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC);


    //enable PWM generator for main motor
    PWMGenEnable(PWM_MAIN_BASE, PWM_MAIN_GEN);

    // Disable the output.  Repeat this call with 'true' to turn O/P on.
    PWMOutputState(PWM_MAIN_BASE, PWM_MAIN_OUTBIT, false);

    ptr_main_duty_cycle = &main_duty_cycle;

}



/*********************************************************
 * initialise_tail_PWM
 * M1PWM5 (J3-10, PF1) is used for the tail motor
 *********************************************************/
void initialise_tail_PWM(void)
{
    // Enable PWM peripheral and GPIO peripheral for tail motor
    SysCtlPeripheralEnable(PWM_TAIL_PERIPH_PWM);
    SysCtlPeripheralEnable(PWM_TAIL_PERIPH_GPIO);

    // Configure tail motor gpio pins for PWM output
    GPIOPinConfigure(PWM_TAIL_GPIO_CONFIG);
    GPIOPinTypePWM(PWM_TAIL_GPIO_BASE, PWM_TAIL_GPIO_PIN);

    // Configure PWM generator for tail motor
    PWMGenConfigure(PWM_TAIL_BASE, PWM_TAIL_GEN, PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC);

    // Set the initial PWM parameters for tail motor
    set_tail_PWM(PWM_START_RATE_HZ, PWM_FIXED_DUTY);

    // Enable PWM generator for tail motor
    PWMGenEnable(PWM_TAIL_BASE, PWM_TAIL_GEN);

    // Disable the output initially, can be enabled later
    PWMOutputState(PWM_TAIL_BASE, PWM_TAIL_OUTBIT, false);

    ptr_tail_duty_cycle = &tail_duty_cycle;
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
    main_duty_cycle = ui32Duty;
    }



//********************************************************
// Function to set the freq, duty cycle of M1PWM5 (tail motor)
//********************************************************
void set_tail_PWM(uint32_t ui32Freq, uint32_t ui32Duty)
{
    // Calculate the PWM period corresponding to the freq.
    uint32_t ui32Period = SysCtlClockGet() / PWM_DIVIDER / ui32Freq;

    // Configure the PWM period and duty cycle
    PWMGenPeriodSet(PWM_TAIL_BASE, PWM_TAIL_GEN, ui32Period);
    PWMPulseWidthSet(PWM_TAIL_BASE, PWM_TAIL_OUTNUM, ui32Period * ui32Duty / 100);
    tail_duty_cycle = ui32Duty;
}



//********************************************************
// Function to set the freq, duty cycle of both motors to zero
// ********************************************************
void kill_motors(void)
{
    set_rotor_PWM (0, 0);
    set_tail_PWM(0, 0);
    heli_state = LANDED;
}
