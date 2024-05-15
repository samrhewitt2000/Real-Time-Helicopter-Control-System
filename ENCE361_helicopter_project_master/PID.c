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

#include "circBuffer.h"
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/adc.h"
#include "driverlib/pwm.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/interrupt.h"
#include "driverlib/debug.h"
#include "utils/ustdlib.h"
#include "OrbitOLED/OrbitOLEDInterface.h"
#include "yaw.h"
#include "displays.h"
#include "ADC.h"
#include "buttons.h"
#include "inc/hw_ints.h"
#include "PID.H"

#define MAX_CONTROL_OUTPUT 98
#define MIN_CONTROL_OUPUT 2



//*****************************************************************************
// NOTE: change the variable name of sensor_reading (not always a sensor reading)
//*****************************************************************************
int32_t controller (int32_t setpoint, int32_t sensor_reading, int32_t Kp, int32_t Ki, int32_t Kd, int32_t offset, int32_t float_conversion_factor)
{
    static int32_t I = 0;
    static int32_t prev_sensor_reading = 0;
    int32_t error = setpoint - sensor_reading;
    int32_t P = (Kp * error);
    int32_t dI = Ki * error * delta_t ///delta_t = 1/systick?
    int32_t D = Kd * (prev_sensor_reading - sensor_reading) / delta_t;

    int32_t control_action = (P + (I + dI) + D + offset) / float_coversion_factor;

    //check for integral saturation
    if (control > MAX_CONTROL_OUTPUT)
    {
        control = MAX_CONTROL_OUTPUT;
    } 
    else if (control < MIN_CONTROL_OUPUT)
    {
        control = MIN_CONTROL_OUPUT;
    }
    else 
    {
        I = (I + dI);
    }

    prev_sensor_reading = sensor_reading;

    return control
}