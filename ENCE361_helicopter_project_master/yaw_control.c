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
#include "yaw_control.h"

//k values need changing
#define Kp 100
#define Ki 100
#define Kd 100
#define Kc 333




volatile int32_t yaw_angle_decimal = 0;  // Global variable to store yaw angle ticks

int32_t yaw_angle_ticks_to_decimal(int32_t quad_enc_ticks)
{
    return abs(((360 * quad_enc_ticks) % 448 * 10) / 448);
}

int32_t yaw_angle_ticks_to_int(int32_t quad_enc_ticks)
{
    return (360 * yaw_ticks / 448);
}





void increase_yaw(int32_t yaw_angle)
{

    //get altitude
    int32_t current_alt_val = get_alt_val(*buffer, buf_size);
    int32_t current_alt_percent = alt_vals_to_percent(current_alt_val);

    int32_t setpoint = (current alt _percent + 1500); //change 15 degrees
    if setpoint > 100 //check not greater than 100
    {
        setpoint = 100;
    }

    //calculate control
    control = controller (setpoint, int32_t sensor_reading, Kp, Ki, Kd, Kc);

    //send to motors
    set_main_PWM (control/10);//control is divided by 100 within PWM function so divide by 10

}

void decrease_yaw(int32_t yaw_angle)
{


    //get altitude
    int32_t current_alt_val = get_alt_val(*buffer, buf_size);
    int32_t current_alt_percent = alt_vals_to_percent(current_alt_val);

    int32_t setpoint = (current alt _percent - 1500); // change 15 degrees
    if setpoint < 0
    {
        setpoint = 0;
    }

    //calculate control
    control = controller (setpoint, int32_t sensor_reading, Kp, Ki, Kd, Kc);

    //send to PWM and motors
    set_main_PWM (control / 10); //control is divided by 100 within PWM function so divide by 10
}
