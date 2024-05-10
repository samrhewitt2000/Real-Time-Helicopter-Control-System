#ifndef YAW_H_
#define YAW_H_

//*****************************************************************************
// 
//      yaw_control.c
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

typedef enum {

    PHASE_1,     // 00
    PHASE_2,     // 01
    PHASE_3,     // 11
    PHASE_4      // 10
} phase_t;

extern volatile int32_t yaw_ticks;  // Global variable to store yaw angle ticks

extern volatile int32_t yaw_angle_decimal;  // Global variable to store yaw angle ticks

extern volatile phase_t current_phase;

extern volatile phase_t prev_phase;

//phase_t get_current_phase(void);

void
PB_IntHandler(void);

void
initYaw (void);

#endif /* YAW_H_ */
