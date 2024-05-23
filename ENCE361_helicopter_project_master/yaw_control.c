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

#include "yaw_control.h"

#define FLOAT_CONVERSION_FACTOR 10
#define Kp 1.0 * FLOAT_CONVERSION_FACTOR
#define Ki 1.0 * FLOAT_CONVERSION_FACTOR
#define Kd 1.0 * FLOAT_CONVERSION_FACTOR
#define Kc 0.8 * FLOAT_CONVERSION_FACTOR



//*****************************************************************************
//
//*****************************************************************************
int32_t yaw_angle_ticks_to_decimal(int32_t quad_enc_ticks)
{
    return abs(((360 * quad_enc_ticks) % 448 * 10) / 448);
}



//*****************************************************************************
//
//*****************************************************************************
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

    //account for coupling on main rotor
    int32_t offset = Kc * rotor_PWM;

    //calculate control
    int32_t control_action = controller (setpoint, quad_enc_ticks, Kp, Ki, Kd, offset, FLOAT_CONVERSION_FACTOR, PWM_MAX_DUTY, PWM_MIN_DUTY);

    //send to PWM and motors
    //set_yaw_PWM (control_action);
}
