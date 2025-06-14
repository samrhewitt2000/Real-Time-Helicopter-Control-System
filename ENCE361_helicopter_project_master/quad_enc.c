//*****************************************************************************
// 
//      quad_enc.c
//
// What does this function do? (Replace)
//
//*****************************************************************************
//
// Author:          Caleb Westbury & Sam Hewitt
// Last modified:   May 2024
//
//*****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_gpio.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"

#include "quad_enc.h"

#define MAX_ENC_TICKS 224
#define MIN_ENC_TICKS -223

volatile int32_t quad_enc_ticks = 0;  // Global variable to store yaw angle ticks
volatile int32_t yaw_angle_decimal = 0;  // Global variable to store yaw angle ticks
volatile phase_t current_phase = PHASE_4;
volatile phase_t prev_phase = PHASE_4;

// *******************************************************
// init_quad_enc: Initialise the quadrature encoder
// *******************************************************
void initYaw (void)
{
//    GPIOIntClear(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);
//    GPIOIntDisable(GPIO_PORTB_BASE, GPIO_INT_PIN_0 | GPIO_INT_PIN_1);

    // Enable Peripheral
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB))
    {
    }
    // Configure GPIO Pins (PB0 and PB1) as Inputs
    GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    // Enable Pull-up Resistors for PB0 and PB1
    GPIOPadConfigSet(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    // Configure Interrupt Type (Both Edges) for PB0 and PB1
    GPIOIntTypeSet(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1, GPIO_BOTH_EDGES);

    // Register Interrupt Handlers
    GPIOIntRegister(GPIO_PORTB_BASE, PB_IntHandler);

    // Enable GPIO Interrupts
    GPIOIntEnable(GPIO_PORTB_BASE, GPIO_INT_PIN_0 | GPIO_INT_PIN_1);

    // Enable Master Interrupts
    IntMasterEnable();
}



// **************************************************************************************************************
// quad_enc_int_handler: Initialise the interrrupt handler for the quardature encoder
// **************************************************************************************************************
void PB_IntHandler(void)
{
    //disable interrupts, no preemption
    GPIOIntDisable(GPIO_PORTB_BASE, GPIO_INT_PIN_0 | GPIO_INT_PIN_1);

    phase_t current_phase;

    //determine the phase based on sensor values
    if (GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_0)) //Sensor A high
    {
        if (GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_1))//sensor B high
        {
            current_phase = PHASE_3; // 11
        }
        else {  // sensor b low
            current_phase = PHASE_4; //10
        }
    }
    else //sensor A low
    {
        if (GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_1)) //sensor B high
        {
            current_phase = PHASE_2;//01
        }
        else {  //sensor b low
            current_phase = PHASE_1;// 00
        }
    }

    //increment yaw based on current value
    switch(current_phase)
    {
        // AB = 00
        case PHASE_1:
            if (prev_phase == PHASE_4)
            {
                quad_enc_ticks++;
            }
            else if (prev_phase == PHASE_2)
            {
                quad_enc_ticks--;
            }
            break;
        // AB = 01
        case PHASE_2:
            if (prev_phase == PHASE_1)
            {
                quad_enc_ticks++;
            }
            else if (prev_phase == PHASE_3)
            {
                quad_enc_ticks--;
            }
            break;
        // AB = 11
        case PHASE_3:
            if (prev_phase == PHASE_2)
            {
                quad_enc_ticks++;
            }
            else if (prev_phase == PHASE_4)
            {
                quad_enc_ticks--;
            }
            break;
        // AB = 10
        case PHASE_4:
            if (prev_phase == PHASE_3)
            {
                quad_enc_ticks++;
            }
            else if (prev_phase == PHASE_1)
            {
                quad_enc_ticks--;
            }
            break;
    }

    //set previous phase for next iteration
    prev_phase = current_phase;

    //reset encoder ticks upon full rotation
    if (quad_enc_ticks > MAX_ENC_TICKS)
    {
        quad_enc_ticks = MIN_ENC_TICKS;
    }
    else if (quad_enc_ticks < MIN_ENC_TICKS)
    {
        quad_enc_ticks = MAX_ENC_TICKS;
    }

    //enable gpio pins for encoder and clear interrupts
    GPIOIntEnable(GPIO_PORTB_BASE, GPIO_INT_PIN_0 | GPIO_INT_PIN_1);
    GPIOIntClear(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);
}


