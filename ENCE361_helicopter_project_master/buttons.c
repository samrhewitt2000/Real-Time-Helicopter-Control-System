//*****************************************************************************
//
//      buttons.c
//
// Support for a set of FOUR specific buttons on the Tiva/Orbit.
// ENCE361 sample code.
// The buttons are:  UP and DOWN (on the Orbit daughterboard) plus
// LEFT and RIGHT on the Tiva.
//
// Note that pin PF0 (the pin for the RIGHT pushbutton - SW2 on
// the Tiva board) needs special treatment - See PhilsNotesOnTiva.rtf.
//
//*****************************************************************************
//
// Author:          Caleb Westbury & Sam Hewitt
// Last modified:   May 2024
//
//*****************************************************************************
//
// Based P.J. Bones' buttons4.c code from 2018
//
//*****************************************************************************

#include "buttons.h"
#include "circ_buffer.h"
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/debug.h"
#include "inc/tm4c123gh6pm.h"  // Board specific defines (for PF0)
#include "kernel.h"
#include "PWM.h"



// *****************************************************************************
// initButtons: Initialize the variables associated with the set of buttons
// defined by the constants in the buttons2.h header file.
// *****************************************************************************
void initButtons(void)
{
    int i;

    // Initialize SWITCH button (active HIGH)
    SysCtlPeripheralEnable(SWITCH_PERIPH);
    GPIOPinTypeGPIOInput(SWITCH_PORT_BASE, SWITCH_PIN);
    GPIOPadConfigSet(SWITCH_PORT_BASE, SWITCH_PIN, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPD);
    but_normal[SWITCH] = SWITCH_NORMAL;
    prev_switch_state = GPIOPinRead(SWITCH_PORT_BASE, SWITCH_PIN) == SWITCH_PIN;

    // Initialize UP button (active HIGH)
    SysCtlPeripheralEnable(UP_BUT_PERIPH);
    GPIOPinTypeGPIOInput(UP_BUT_PORT_BASE, UP_BUT_PIN);
    GPIOPadConfigSet(UP_BUT_PORT_BASE, UP_BUT_PIN, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPD);
    but_normal[UP] = UP_BUT_NORMAL;

    // Initialize DOWN button (active HIGH)
    SysCtlPeripheralEnable(DOWN_BUT_PERIPH);
    GPIOPinTypeGPIOInput(DOWN_BUT_PORT_BASE, DOWN_BUT_PIN);
    GPIOPadConfigSet(DOWN_BUT_PORT_BASE, DOWN_BUT_PIN, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPD);
    but_normal[DOWN] = DOWN_BUT_NORMAL;

    // Initialize LEFT button (active LOW)
    SysCtlPeripheralEnable(LEFT_BUT_PERIPH);
    GPIOPinTypeGPIOInput(LEFT_BUT_PORT_BASE, LEFT_BUT_PIN);
    GPIOPadConfigSet(LEFT_BUT_PORT_BASE, LEFT_BUT_PIN, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
    but_normal[LEFT] = LEFT_BUT_NORMAL;

    // Initialize RIGHT button (active LOW)
    // Note that PF0 requires unlocking before configuration
    SysCtlPeripheralEnable(RIGHT_BUT_PERIPH);
    GPIO_PORTF_LOCK_R = GPIO_LOCK_KEY; // Unlock PF0
    GPIO_PORTF_CR_R |= GPIO_PIN_0; // Allow changes to PF0
    GPIO_PORTF_LOCK_R = GPIO_LOCK_M; // Re-lock PF0
    GPIOPinTypeGPIOInput(RIGHT_BUT_PORT_BASE, RIGHT_BUT_PIN);
    GPIOPadConfigSet(RIGHT_BUT_PORT_BASE, RIGHT_BUT_PIN, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
    but_normal[RIGHT] = RIGHT_BUT_NORMAL;

    // Initialize button status for each button
    for (i = 0; i < NUM_BUTS; i++)
    {
        but_state[i] = but_normal[i];
        but_count[i] = 0;
        but_flag[i] = false;
    }
}



// *****************************************************************************
// updateButtons: Function designed to be called regularly. It polls all
// buttons once and updates variables associated with the buttons if
// necessary.  It is efficient enough to be part of an ISR, e.g. from
// a SysTick interrupt.
// Debounce algorithm: A state machine is associated with each button.
// A state change occurs only after NUM_BUT_POLLS consecutive polls have
// read the pin in the opposite condition, before the state changes and
// a flag is set.  Set NUM_BUT_POLLS according to the polling rate.
// *****************************************************************************
void updateButtons(void)
{
    bool but_value[NUM_BUTS];
    int i;

    // Read the pins; true means HIGH, false means LOW
    but_value[UP] = (GPIOPinRead(UP_BUT_PORT_BASE, UP_BUT_PIN) == UP_BUT_PIN);
    but_value[DOWN] = (GPIOPinRead(DOWN_BUT_PORT_BASE, DOWN_BUT_PIN) == DOWN_BUT_PIN);
    but_value[LEFT] = (GPIOPinRead(LEFT_BUT_PORT_BASE, LEFT_BUT_PIN) == LEFT_BUT_PIN);
    but_value[RIGHT] = (GPIOPinRead(RIGHT_BUT_PORT_BASE, RIGHT_BUT_PIN) == RIGHT_BUT_PIN);
    but_value[SWITCH] = (GPIOPinRead(SWITCH_PORT_BASE, SWITCH_PIN) == SWITCH_PIN);

    // Iterate through the buttons, updating button variables as required
    for (i = 0; i < NUM_BUTS; i++)
    {
        if (but_value[i] != but_state[i])
        {
            but_count[i]++;
            if ((but_count[i] >= NUM_BUT_POLLS_PUSHED) && (but_value[i] == 1))
            {
                but_state[i] = but_value[i];
                but_flag[i] = true;    // Reset by call to checkButton()
                but_count[i] = 0;
            }
            else if ((but_count[i] >= NUM_BUT_POLLS_RELEASED) && (but_value[i] == 0))
            {
                but_state[i] = but_value[i];
                but_flag[i] = true;
                but_count[i] = 0;
            }
        }
        else
        {
            but_count[i] = 0;
        }
    }
}

// *****************************************************************************
// checkButton: Function returns the new button logical state if the button
// logical state (PUSHED or RELEASED) has changed since the last call,
// otherwise returns NO_CHANGE.
// *****************************************************************************
uint8_t checkButton(uint8_t butName)
{
    if (but_flag[butName])
    {
        but_flag[butName] = false;
        if (but_state[butName] == but_normal[butName])
            return RELEASED;
        else
            return PUSHED;
    }
    return NO_CHANGE;
}



// *****************************************************************************
// switch_task: Manages the helicopter state transitions based on the switch state
// *****************************************************************************
void switch_task(void)
{
    current_switch_state = GPIOPinRead(SWITCH_PORT_BASE, SWITCH_PIN) == SWITCH_PIN;
    if (current_switch_state != prev_switch_state)
    {
        if (current_switch_state == SWITCH_NORMAL && heli_state == LANDED)
        {
            heli_state = TAKEOFF;
        }
        else if (current_switch_state != prev_switch_state && current_switch_state != SWITCH_NORMAL && heli_state == FLYING)
        {
            heli_state = LANDING;
        }
    }
    prev_switch_state = current_switch_state;
}



// *****************************************************************************
// push_buttons_task: Handles button presses to control the helicopter's rotor and tail PWM
// *****************************************************************************
void push_buttons_task(void)
{
    if (checkButton(UP) == PUSHED && heli_state == FLYING)
    {
        set_rotor_PWM(250, *ptr_main_duty_cycle + 10);  // Increase altitude by 10%
        // Example: increase altitude by 10%
        //change_altitude(alt_val_to_percent(initial_ADC_val, current_ADC_val), 10);
    }
    if (checkButton(DOWN) == PUSHED && heli_state == FLYING)
    {
        set_rotor_PWM(250, *ptr_main_duty_cycle - 10);  // Decrease altitude by 10%
        // Example: decrease altitude by 10%
        //change_altitude(alt_val_to_percent(initial_ADC_val, current_ADC_val), -10);
    }
    if (checkButton(LEFT) == PUSHED && heli_state == FLYING)
    {
        set_tail_PWM(250, *ptr_tail_duty_cycle + 15);  // Adjust tail rotor to turn left
    }
    if (checkButton(RIGHT) == PUS
