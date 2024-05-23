//*****************************************************************************
// 
//      PID.c
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

#include "PID.H"

//*****************************************************************************
// NOTE: change the variable name of sensor_reading (not always a sensor reading)
//*****************************************************************************
int32_t controller (int32_t setpoint, int32_t sensor_reading, int32_t Kp, int32_t Ki, int32_t Kd, int32_t offset, int32_t float_conversion_factor)
{
    static int32_t I = 0;
    static int32_t prev_sensor_reading = 0;
    int32_t error = setpoint - sensor_reading;
    int32_t P = Kp * error;
    int32_t dI = Ki * error * delta_t; ///delta_t = 1/systick?
    int32_t D = Kd * (prev_sensor_reading - sensor_reading) / delta_t;

    int32_t control_action = (P + (I + dI) + D + offset) / float_conversion_factor;

    //check for integral saturation
    if (control_action > MAX_CONTROL_OUTPUT)
    {
        control_action = MAX_CONTROL_OUTPUT;
    } 
    else if (control_action < MIN_CONTROL_OUPUT)
    {
        control_action = MIN_CONTROL_OUPUT;
    }
    else 
    {
        I = (I + dI);
    }

    prev_sensor_reading = sensor_reading;

    return control_action;
}
