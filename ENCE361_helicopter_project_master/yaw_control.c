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
#include "inc/hw_memmap.h"
#include "inc/hw_gpio.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "displays.h"


#define FLOAT_CONVERSION_FACTOR 10
#define Kp 1.0 * FLOAT_CONVERSION_FACTOR
#define Ki 1.0 * FLOAT_CONVERSION_FACTOR
#define Kd 1.0 * FLOAT_CONVERSION_FACTOR
#define Kc 0.8 * FLOAT_CONVERSION_FACTOR



extern circBuf_t g_inBuffer;


void ref_yaw_int_handler(void)
{
    //disable interrupts, no preemption
    //
    //set reference yaw to zero
    quad_enc_ticks = 0;
    pK_block_task(ref_yaw_task_ID);
    set_rotor_PWM(250, 30);
    heli_state = LANDING;
    displayYaw(0, 3);
    //test code
    //GPIOIntEnable(GPIO_PORTC_BASE, GPIO_INT_PIN_4);
    GPIOIntDisable(GPIO_PORTC_BASE, GPIO_INT_PIN_4);
}



void init_ref_yaw (void)
{
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
    int32_t offset = Kc * *ptr_main_duty_cycle;

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
}



//*****************************************************************************
//
//*****************************************************************************
void find_reference_yaw_task(void)
{
    //pK_block_task(pK_get_current_task_id());
    //change_altitude(alt_val_to_percent(initial_ADC_val, current_ADC_val), 10);
    //change_yaw_angle(360, *ptr_main_duty_cycle);
    set_rotor_PWM(250, 51);
    set_tail_PWM(250, 5);
    return;
}




