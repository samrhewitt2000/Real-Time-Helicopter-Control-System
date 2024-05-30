#ifndef BUTTONS_H_
#define BUTTONS_H_

//*****************************************************************************
//
//      buttons.h
//
// Support for a set of FOUR specific buttons on the Tiva/Orbit.
// ENCE361 sample code.
// The buttons are:  UP and DOWN (on the Orbit daughterboard) plus
// LEFT and RIGHT on the Tiva.
//
//*****************************************************************************
//
// Author:          Caleb Westbury & Sam Hewitt
// Last modified:   May 2024
//
//*****************************************************************************
//
// Based on P.J. Bones' buttons4.h code from 2018
//
//*****************************************************************************

#include <stdint.h>
#include <stdbool.h>

//*****************************************************************************
// Constants
//*****************************************************************************

// Enumeration for button names
enum butNames {UP = 0, DOWN, LEFT, RIGHT, SWITCH, NUM_BUTS};

// Enumeration for button states
enum butStates {RELEASED = 0, PUSHED, NO_CHANGE};

// UP button configuration
#define UP_BUT_PERIPH  SYSCTL_PERIPH_GPIOE
#define UP_BUT_PORT_BASE  GPIO_PORTE_BASE
#define UP_BUT_PIN  GPIO_PIN_0
#define UP_BUT_NORMAL  false

// DOWN button configuration
#define DOWN_BUT_PERIPH  SYSCTL_PERIPH_GPIOD
#define DOWN_BUT_PORT_BASE  GPIO_PORTD_BASE
#define DOWN_BUT_PIN  GPIO_PIN_2
#define DOWN_BUT_NORMAL  false

// LEFT button configuration
#define LEFT_BUT_PERIPH  SYSCTL_PERIPH_GPIOF
#define LEFT_BUT_PORT_BASE  GPIO_PORTF_BASE
#define LEFT_BUT_PIN  GPIO_PIN_4
#define LEFT_BUT_NORMAL  true

// RIGHT button configuration
#define RIGHT_BUT_PERIPH  SYSCTL_PERIPH_GPIOF
#define RIGHT_BUT_PORT_BASE  GPIO_PORTF_BASE
#define RIGHT_BUT_PIN  GPIO_PIN_0
#define RIGHT_BUT_NORMAL  true

// SWITCH slider configuration
#define SWITCH_PERIPH SYSCTL_PERIPH_GPIOA
#define SWITCH_PORT_BASE  GPIO_PORTA_BASE
#define SWITCH_PIN  GPIO_PIN_7
#define SWITCH_NORMAL  true

// Debounce settings
#define NUM_BUT_POLLS_RELEASED 5 // Number of polls required to confirm button release
#define NUM_BUT_POLLS_PUSHED 1 // Number of polls required to confirm button press

//*****************************************************************************
// Helicopter state enum
//*****************************************************************************
typedef enum {
    YAW_REF,   // Yaw reference state
    LANDED,    // Helicopter landed state
    TAKEOFF,   // Helicopter takeoff state
    FLYING,    // Helicopter flying state
    LANDING    // Helicopter landing state
} helicopter_state_t;

//*****************************************************************************
// Globals to module
//*****************************************************************************

static bool but_state[NUM_BUTS];    // Current electrical state of buttons
static uint8_t but_count[NUM_BUTS]; // Debounce counter for buttons
static bool but_flag[NUM_BUTS];     // Button state change flag
static bool but_normal[NUM_BUTS];   // Normal state of buttons (pressed or released)
static int32_t prev_switch_state;   // Previous state of the switch
volatile extern int32_t current_ADC_val; // Current ADC value (externally defined)
extern helicopter_state_t heli_state;    // Current state of the helicopter (externally defined)
int32_t current_switch_state;       // Current state of the switch

//*****************************************************************************
// Function Prototypes
//*****************************************************************************

//*****************************************************************************
// initButtons: Initialize the variables associated with the set of buttons
// defined by the constants above.
void initButtons(void);

//*****************************************************************************
// updateButtons: Function designed to be called regularly. It polls all
// buttons once and updates variables associated with the buttons if
// necessary. It is efficient enough to be part of an ISR, e.g. from
// a SysTick interrupt.
void updateButtons(void);

//*****************************************************************************
// checkButton: Function returns the new button state if the button state
// (PUSHED or RELEASED) has changed since the last call, otherwise returns
// NO_CHANGE. The argument butName should be one of the constants in the
// enumeration butStates, excluding 'NUM_BUTS'. Safe under interrupt.
uint8_t checkButton(uint8_t butName);

//*****************************************************************************
// switch_task: Function to handle the switch state and transition the
// helicopter's state accordingly.
void switch_task(void);

//*****************************************************************************
// pushbuttons_task: Function to handle the actions associated with the
// pushbuttons for controlling the helicopter.
void pushbuttons_task(void);

#endif /* BUTTONS_H_ */
