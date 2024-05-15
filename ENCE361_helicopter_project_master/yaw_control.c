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
#include "PID.h"
#include "quad_enc.h"
#include "yaw_control.h"
#include "ADC.h"

#define KP_TAIL 0.5 * FLOAT_CONVERSION_FACTOR //k values need changing
#define KI_TAIL 0.5 * FLOAT_CONVERSION_FACTOR
#define KD_TAIL 0.5 * FLOAT_CONVERSION_FACTOR
#define KC_TAIL 0.8 * FLOAT_CONVERSION_FACTOR
#define FLOAT_CONVERSION_FACTOR 10

volatile int32_t yaw_angle_decimal = 0;  // Global variable to store yaw angle ticks

//*****************************************************************************************************
//converts yaw angle ticks into its decimal part for display
//*****************************************************************************************************
int32_t yaw_angle_ticks_to_decimal(int32_t quad_enc_ticks)
{
    return abs(((360 * quad_enc_ticks) % 448 * 10) / 448);
}



//*****************************************************************************************************
//converts yaw angle ticks into its integer format
//*****************************************************************************************************
int32_t yaw_angle_ticks_to_int(int32_t quad_enc_ticks)
{
    return (360 * yaw_ticks / 448);
}



//*****************************************************************************************************
//converts yaw angle back into yaw encoder ticks
//*****************************************************************************************************
int32_t yaw_angle_to_ticks(int32_t angle)
{
    return (angle * 448 / 360);
}



//*****************************************************************************************************
//change yaw angle by specified amount
//*****************************************************************************************************
void change_yaw_angle(int32_t current_yaw_ticks, int32_t yaw_angle_change, FLOAT_CONVERSION_FACTOR)
{
    //get yaw ticks from yaw angle
    int32_t yaw_angle_change_ticks = (angle * 448 / 360);

    int32_t setpoint = (current_yaw_ticks + yaw_angle_change_ticks);

    //account for coupling on main rotor
    offset = main_duty_cycle * KC_TAIL;

    //calculate control
    control = controller (setpoint, int32_t sensor_reading, Kp, Ki, Kd, offset) / FLOAT_CONVERSION_FACTOR;

    //send to PWM and motors
    set_main_PWM (control); //control is out of 100 within PWM function so divide by 10
}

