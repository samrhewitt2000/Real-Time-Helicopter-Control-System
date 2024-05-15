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
#include "buttons5.h"
#include "inc/hw_ints.h"
#include "PID.H"

#define MAX_OUTPUT 1000




int32_t controller (int32_t setpoint, int32_t sensor_reading, int32_t Kp, int32_t Ki, int32_t Kd, int32_t Kc)
{
    //avoid floating point maths multiply everything by 1000

    //account for gravity ~33% duty cycle
    int32_t Kg = 333

    static int32_t I = 0;
    static int32_t prev_sensor_reading = 0;
    int32_t error = (setpoint - sensor_reading);
    int32_t P = (Kp * error);
    int32_t dI = Ki * error * delta_t ///delta_t = 1/systick?
    int32_t D = Kd * (prev_sensor_reading - sensor_reading) / delta_t;


    //no coupling if using main rotor but must account for gravity
    if (Kc = 0)
    {
        int32_t control = P + (I + dI) + D + Kg;
    } else {
        int32_t control = P + (I + dI) + D + Kc; //account for motor coupling
    }


    //check for integral saturation
    if (control > MAX_OUTPUT)
    {
        control = MAX_OUTPUT;
    }else {
        I = (I + dI);
    }

    return control //must divide by 1000 before use


}
