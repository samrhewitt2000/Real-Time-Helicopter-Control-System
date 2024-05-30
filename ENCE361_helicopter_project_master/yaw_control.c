//*****************************************************************************
// 
//      yaw_control.c
//
// Implements yaw control logic and tasks
//
//*****************************************************************************

//
// Author:          Caleb Westbury & Sam Hewitt
// Last modified:   May 2024
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
    // Disable interrupts, no preemption
    GPIOIntDisable(GPIO_PORTC_BASE, GPIO_INT_PIN_4);
    
    // Set reference yaw to zero
    quad_enc_ticks = 0;
    pK_block_task(ref_yaw_task_ID);
    set_rotor_PWM(250, 30);
    heli_state = LANDING;
    
    // Test code
    //GPIOIntEnable(GPIO_PORTC_BASE, GPIO_INT_PIN_4);
    GPIOIntDisable(GPIO_PORTC_BASE, GPIO_INT_PIN_4);
}

void init_ref_yaw(void)
{
    // Enable Peripheral
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOC)) {}
    
    // Configure GPIO Pin PC4 as Inputs
    GPIOPinTypeGPIOInput(GPIO_PORTC_BASE, GPIO_PIN_4);
    
    // Enable Pull-up Resistors
    GPIOPadConfigSet(GPIO_PORTC_BASE, GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
    
    // Configure Interrupt Type (Both Edges) for PC4
    GPIOIntTypeSet(GPIO_PORTC_BASE, GPIO_PIN_4, GPIO_BOTH_EDGES);
    
    // Register Interrupt Handler
    GPIOIntRegister(GPIO_PORTC_BASE, ref_yaw_int_handler);
    
    // Enable GPIO Interrupts
    GPIOIntEnable(GPIO_PORTC_BASE, GPIO_INT_PIN_4);
}

int32_t yaw_angle_ticks_to_decimal(int32_t quad_enc_ticks)
{
    return abs(((360 * quad_enc_ticks) % 448 * 10) / 448);
}

int32_t yaw_angle_ticks_to_int(int32_t quad_enc_ticks)
{
    return (360 * quad_enc_ticks / 448);
}

int32_t yaw_angle_to_ticks(int32_t angle)
{
    return (angle * 448 / 360);
}

void change_yaw_angle(int32_t yaw_angle_change, int32_t rotor_PWM)
{
    int32_t setpoint = (quad_enc_ticks + yaw_angle_to_ticks(yaw_angle_change));
    int32_t offset = Kc * *ptr_main_duty_cycle;
    int32_t control_action = controller (setpoint, quad_enc_ticks, Kp, Ki, Kd, offset, FLOAT_CONVERSION_FACTOR, PWM_MAX_DUTY, PWM_MIN_DUTY);
    // Set PWM and motors
    // set_yaw_PWM(control_action);
}

void yaw_control_task(void)
{
    // Implement yaw control logic here
    // Example: Change yaw angle by 30 degrees clockwise
    change_yaw_angle(30, 50); // Assuming 50 as rotor PWM value
    // Indicate task completion
}

void find_reference_yaw_task(void)
{
    // Change altitude and set tail PWM
    change_altitude(alt_val_to_percent(initial_ADC_val, get_alt_val(&g_inBuffer)), 10);
    set_tail_PWM(250, 50);
    PWMOutputState(PWM_MAIN_BASE, PWM_MAIN_OUTBIT, true);
    PWMOutputState(PWM_TAIL_BASE, PWM_TAIL_OUTBIT, true);
    return;
}
