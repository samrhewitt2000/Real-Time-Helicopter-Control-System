/*
 * quadrature.c
 *
 *  Created on: 1/05/2024
 *      Author: she108 and Caleb Westbury
 */


#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_gpio.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "yaw.h"

phase_t get_current_phase(void)
{

    phase_t current_phase;

    uint8_t channel_a_state = GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_0);
    uint8_t channel_b_state = GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_1);

    if (channel_a_state)
    {
        if (channel_b_state)
        {
            current_phase = PHASE_3;
        }
        else {
            current_phase = PHASE_4;
        }
    }
    else
    {
        if (channel_b_state)
        {
            current_phase = PHASE_2;
        }
        else {
            current_phase = PHASE_1;
        }
    }
    return current_phase;
}


void PB_IntHandler(void)
{

    current_phase = get_current_phase();

    switch(current_phase)
           {
           case PHASE_1:
               if (prev_phase == PHASE_4)
               {
                   yaw_angle++;
               } else {
                   yaw_angle--;
               }
               break;
           case PHASE_2:
               if (current_phase > prev_phase)
               {
                   yaw_angle++;
               } else {
                   yaw_angle--;
               }
               break;
           case PHASE_3:
               if (current_phase > prev_phase)
               {
                   yaw_angle++;
               } else {
                   yaw_angle--;
               }
               break;
           case PHASE_4:
               if (prev_phase == PHASE_3)
               {
                   yaw_angle++;
               } else {
                   yaw_angle--;
               }
               break;
           }

    prev_phase = current_phase;
    GPIOIntClear(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);
}

void
initYaw (void)
{
    // Enable Peripheral Clocks
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

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
