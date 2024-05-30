#ifndef PID_H_
#define PID_H_

//*****************************************************************************
// 
//      PID.h
//
// Header file for a PID controller function
//
//*****************************************************************************
//
// Author:          Caleb Westbury & Sam Hewitt
// Last modified:   May 2024
//
//*****************************************************************************

#include <stdint.h>

// Function prototype for the PID controller
int32_t controller(int32_t setpoint, int32_t sensor_reading, int32_t Kp, int32_t Ki, int32_t Kd, int32_t offset, int32_t float_conversion_factor, int32_t max_output, int32_t min_output);

#endif /*PID_H_*/
