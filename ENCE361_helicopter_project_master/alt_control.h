#ifndef ALT_CONTROL_H_
#define ALT_CONTROL_H_

//*****************************************************************************
//
//      alt_control.h
//
// Header file for altitude control module. Defines the interface for
// controlling the helicopter altitude based on ADC values and button input.
//
//*****************************************************************************
//
// Author:          Caleb Westbury & Sam Hewitt
// Last modified:   May 2024
//
//*****************************************************************************

#include <stdint.h>
#include "circ_buffer.h"
#include "PWM.h"
#include "PID.h"

//*****************************************************************************
// Global Variables
//*****************************************************************************

extern volatile int32_t *ptr_current_alt_percent; // Pointer to the current altitude percentage

//*****************************************************************************
// Function Prototypes
//*****************************************************************************

/**
 * @brief Get the average ADC value from the circular buffer.
 * 
 * @param buffer Pointer to the circular buffer structure
 * @return int32_t Average ADC value
 */
int32_t get_alt_val(circBuf_t *buffer);

/**
 * @brief Convert ADC value to altitude percentage.
 * 
 * @param initial_alt_val Initial ADC value for altitude calibration
 * @param current_alt_val Current ADC value
 * @return int32_t Altitude percentage
 */
int32_t alt_val_to_percent(int32_t initial_alt_val, int32_t current_
