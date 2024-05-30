#ifndef QUAD_ENC_H_
#define QUAD_ENC_H_

//*****************************************************************************
// 
//      quad_enc.h
//
// Header file for quadrature encoder interface
//
//*****************************************************************************

//
// Author:          Caleb Westbury & Sam Hewitt
// Last modified:   May 2024
//
//*****************************************************************************

// *******************************************************
// encoder phase enum
// *******************************************************
typedef enum {
    PHASE_1,     // 00
    PHASE_2,     // 01
    PHASE_3,     // 11
    PHASE_4      // 10
} phase_t;

// Global variables
extern volatile int32_t quad_enc_ticks;       // Global variable to store yaw angle ticks
extern volatile int32_t yaw_angle_decimal;    // Global variable to store yaw angle ticks
extern volatile phase_t current_phase;
extern volatile phase_t prev_phase;

// Function prototypes
void PB_IntHandler(void);   // Interrupt handler for quadrature encoder input
void initYaw(void);         // Initialization function for quadrature encoder

#endif /* QUAD_ENC_H_ */
