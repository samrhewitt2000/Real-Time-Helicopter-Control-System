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

extern volatile int32_t yaw_angle_decimal;  // Global variable to store yaw angle ticks
extern volatile int32_t yaw_angle_int;

int32_t yaw_angle_ticks_to_decimal(int32_t quad_enc_ticks);

int32_t yaw_angle_ticks_to_int(int32_t quad_enc_ticks);

int32_t yaw_angle_to_ticks(int32_t angle);

void change_yaw_angle(int32_t yaw_angle_change, int32_t *rotor_PWM);


#endif /* YAW_CONTROL_H_ */
