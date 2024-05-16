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

#include <stdint.h>
#include <stdbool.h>
#include "quad_enc.h"
#include "yaw_control.h"
#include "PWM.h"
#include "alt_control.h"
#include "circ_buffer.h"
#include "PID.h"
#include <stdint.h>
#include <stdbool.h>
#include "driverlib/PWM.h"
#include "ADC.h"
#include "buttons.h"

#define FLOAT_CONVERSION_FACTOR 1000
#define Kp 1.0 * FLOAT_CONVERSION_FACTOR
#define Ki 1.0 * FLOAT_CONVERSION_FACTOR
#define Kd 1.0 * FLOAT_CONVERSION_FACTOR
#define Kc 0.8 * FLOAT_CONVERSION_FACTOR

volatile int32_t yaw_angle_decimal;  // Global variable to store yaw angle ticks
volatile int32_t yaw_angle_int;

//*****************************************************************************
//
//*****************************************************************************
int32_t yaw_angle_ticks_to_decimal(int32_t quad_enc_ticks)
{
    return abs(((360 * quad_enc_ticks) % 448 * 10) / 448);
}



//*****************************************************************************************************
// converts yaw angle ticks into its integer format
//*****************************************************************************************************
int32_t yaw_angle_ticks_to_int(int32_t quad_enc_ticks)
{
    return (360 * quad_enc_ticks / 448);
}



//*****************************************************************************************************
// converts yaw angle back into yaw encoder ticks - check with big sam or tutor
//*****************************************************************************************************
int32_t yaw_angle_to_ticks(int32_t angle)
{
    return (angle * 448 / 360);
}



//*****************************************************************************************************
// change yaw angle by specified amount
//*****************************************************************************************************
void change_yaw_angle(int32_t yaw_angle_change, int32_t rotor_PWM)
{
    int32_t setpoint = (quad_enc_ticks + yaw_angle_to_ticks(yaw_angle_change));
    int32_t offset;
    //account for coupling on main rotor
    offset = Kc * rotor_PWM * FLOAT_CONVERSION_FACTOR;

    //calculate control
    //send to PWM and motors
    set_tail_PWM(controller (setpoint, quad_enc_ticks, Kp, Ki, Kd, offset) / FLOAT_CONVERSION_FACTOR);//divide control action by conversion factor
}

