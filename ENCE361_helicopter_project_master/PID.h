#ifndef PID_H_
#define PID_H_

//*****************************************************************************
// 
//      PID.h
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

int32_t controller (int32_t setpoint, int32_t sensor_reading, int32_t Kp, int32_t Ki, int32_t Kd, int32_t offset, int32_t float_conversion_factor, int32_t max_output, int32_t min_output, int32_t delta_t);

#endif /*PID_H_*/
