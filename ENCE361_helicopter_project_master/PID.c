//*****************************************************************************
// 
//      PID.c
//
// Implementation of a PID controller
//
//*****************************************************************************
//
// Author:          Caleb Westbury & Sam Hewitt
// Last modified:   May 2024
//
//*****************************************************************************

#include "PID.h"                // PID header file
#include "displays.h"           // Display functions

int32_t controller(int32_t setpoint, int32_t sensor_reading, int32_t Kp, int32_t Ki, int32_t Kd, int32_t offset, int32_t float_conversion_factor, int32_t max_output, int32_t min_output)
{
    static int32_t delta_t = (SysCtlClockGet() / 100); // Sampling period (global variable)
    static int32_t I = 0;                              // Integral term
    static int32_t prev_sensor_reading = 0;             // Previous sensor reading
    int32_t error = setpoint - sensor_reading;          // Error calculation
    int32_t P = Kp * error;                             // Proportional term
    int32_t dI = Ki * error / delta_t;                  // Integral term change
    int32_t D = Kd * (prev_sensor_reading - sensor_reading) * delta_t; // Derivative term

    // Calculate control action
    int32_t control_action = (P + (I + dI) + D + offset) / float_conversion_factor;

    // Check for integral saturation
    if (control_action > max_output)
    {
        control_action = max_output;
    } 
    else if (control_action < min_output)
    {
        control_action = min_output;
    }
    else 
    {
        I = (I + dI);   // Update integral term
    }

    prev_sensor_reading = sensor_reading; // Update previous sensor reading

    return control_action;  // Return control action
}
