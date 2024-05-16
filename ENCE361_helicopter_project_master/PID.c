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


#include "PWM.h"
#include "PID.h"
#include "circ_buffer.h"
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
#include "yaw_control.h"
#include "displays.h"
#include "ADC.h"
#include "buttons.h"
#include "inc/hw_ints.h"

#define MAX_OUTPUT 980   //max output * FLOAT_CONVERSION_FACTOR
#define MIN_OUTPUT 20   //min output * FLOAT_CONVERSION_FACTOR

int32_t controller (int32_t setpoint, int32_t sensor_reading, int32_t Kp, int32_t Ki, int32_t Kd, int32_t offset)
{
    //avoid floating point maths multiply everything by 1000

    //account for gravity ~33% duty cycle
    int32_t Kg = (333 * main_rotor_duty / 100);

    int32_t delta_t = 48 * SysCtlClockGet();
    static int32_t I = 0;
    static int32_t prev_sensor_reading = 0;
    int32_t error = (setpoint - sensor_reading);
    int32_t P = (Kp * error);
    int32_t dI = (Ki * error * delta_t); ///delta_t = 1/systick?
    int32_t D = Kd * (prev_sensor_reading - sensor_reading) / delta_t;

    //no coupling if using main rotor but must account for gravity
    int32_t control = P + (I + dI) + D + offset; //account for motor coupling

    //check for integral saturation
    if (control > MAX_OUTPUT)
    {
        control = MAX_OUTPUT;
    } else if (control < MIN_OUTPUT)
    {
        control = MIN_OUTPUT;
    }else {
        I = (I + dI);
    }

    prev_sensor_reading = sensor_reading;


    return control; //control signal * 1000


}
