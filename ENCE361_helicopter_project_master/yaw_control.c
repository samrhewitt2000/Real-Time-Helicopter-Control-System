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
#include "yaw_control.h"
#include "PWM.h"
#include "alt_control.h"
#include "circ_buffer.h"
#include "PID.h"
#include "driverlib/PWM.h"
#include "ADC.h"
#include "buttons.h"
#include "kernel.h"

#define FLOAT_CONVERSION_FACTOR 10
#define Kp 1.0 * FLOAT_CONVERSION_FACTOR
#define Ki 1.0 * FLOAT_CONVERSION_FACTOR
#define Kd 1.0 * FLOAT_CONVERSION_FACTOR
#define Kc 0.8 * FLOAT_CONVERSION_FACTOR






void ref_yaw_int_handler(void)
{
    //disale interrupts, no preemption
    GPIOIntDisable(GPIO_PORTC_BASE, GPIO_INT_PIN_4);

    //set reference yaw to zero
    quad_enc_ticks = 0;

}



void init_ref_yaw (void)
{
//    GPIOIntClear(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);
//    GPIOIntDisable(GPIO_PORTB_BASE, GPIO_INT_PIN_0 | GPIO_INT_PIN_1);

    // Enable Peripheral
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOC))
    {
    }
    // Configure GPIO Pin PC4 as Inputs
    GPIOPinTypeGPIOInput(GPIO_PORTC_BASE, GPIO_PIN_4);

    // Enable Pull-up Resistors for
    GPIOPadConfigSet(GPIO_PORTC_BASE, GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    // Configure Interrupt Type (Both Edges) for PC4
    GPIOIntTypeSet(GPIO_PORTC_BASE, GPIO_PIN_4, GPIO_BOTH_EDGES);

    // Register Interrupt Handlers
    GPIOIntRegister(GPIO_PORTC_BASE, ref_yaw_int_handler);

    // Enable GPIO Interrupts
    GPIOIntEnable(GPIO_PORTC_BASE, GPIO_INT_PIN_4);

    // Enable Master Interrupts
    IntMasterEnable();
}



void reference_yaw_task(void)
{

}

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




//*****************************************************************************
//
//*****************************************************************************
void yaw_control_task(void)
{
    // Implement yaw control logic here

    // Example: Change yaw angle by 30 degrees clockwise
    change_yaw_angle(30, 50); // Assuming 50 as rotor PWM value

    // Indicate task completion
    pK_block_task(pK_get_current_task_id());
}
