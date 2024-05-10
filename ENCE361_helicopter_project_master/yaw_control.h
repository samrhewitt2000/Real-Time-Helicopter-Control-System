#ifndef YAW_CONTROL_H_
#define YAW_CONTROL_H_

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

#include "quad_enc.h"

volatile int32_t yaw_angle_decimal = 0;  // Global variable to store yaw angle ticks

int32_t yaw_angle_ticks_to_decimal(int32_t quad_enc_ticks);

int32_t yaw_angle_ticks_to_int(int32_t quad_enc_ticks);

#endif /* YAW_CONTROL_H_ */
