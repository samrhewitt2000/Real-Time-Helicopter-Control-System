#ifndef YAW_CONTROL_H_
#define YAW_CONTROL_H_

//*****************************************************************************
// 
//      yaw_control.h
//
// Header file for yaw control functions and tasks
//
//*****************************************************************************

//
// Author:          Caleb Westbury & Sam Hewitt
// Last modified:   May 2024
//
//*****************************************************************************

#include <stdint.h>
#include "quad_enc.h"
#include "PWM.h"
#include "PID.h"

//extern volatile int32_t yaw_angle_int = 0;

extern unsigned char ref_yaw_task_ID;

//*****************************************************************************
// Function to convert yaw angle ticks to decimal value
//*****************************************************************************
int32_t yaw_angle_ticks_to_decimal(int32_t quad_enc_ticks);

//*****************************************************************************
// Function to initialize reference yaw
//*****************************************************************************
void init_ref_yaw(void);

//*****************************************************************************
// Function to convert yaw angle ticks to integer value
//*****************************************************************************
int32_t yaw_angle_ticks_to_int(int32_t quad_enc_ticks);

//*****************************************************************************
// Function to convert yaw angle to ticks
//*****************************************************************************
int32_t yaw_angle_to_ticks(int32_t angle);

//*****************************************************************************
// Function to change yaw angle
//*****************************************************************************
void change_yaw_angle(int32_t yaw_angle_change, int32_t rotor_PWM);

//*****************************************************************************
// Function for yaw control task
//*****************************************************************************
void yaw_control_task(void);

//*****************************************************************************
// Function for finding reference yaw task
//*****************************************************************************
void find_reference_yaw_task(void);

#endif /* YAW_CONTROL_H_ */
