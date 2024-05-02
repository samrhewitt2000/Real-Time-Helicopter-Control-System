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

//volatile int32_t yaw_angle = 0;  // Global variable to store yaw angle

void PB0_IntHandler(void)
{
    // Read the current state of PB1 (channel B)
    uint8_t channel_b_state = GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_1);

    // Update yaw angle based on the state change of PB0 (channel A)
    if (channel_b_state) {
        yaw_angle++;  // Clockwise rotation
    } else {
        yaw_angle--;  // Counterclockwise rotation
    }

    // Clear the interrupt for PB0
    GPIOIntClear(GPIO_PORTB_BASE, GPIO_PIN_0);
}

void PB1_IntHandler(void)
{
    // Read the current state of PB0 (channel A)
    uint8_t channel_a_state = GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_0);

    // Update yaw angle based on the state change of PB1 (channel B)
    if (channel_a_state) {
        yaw_angle--;  // Counterclockwise rotation
    } else {
        yaw_angle++;  // Clockwise rotation
    }

    // Clear the interrupt for PB1
    GPIOIntClear(GPIO_PORTB_BASE, GPIO_PIN_1);
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
    GPIOIntRegister(GPIO_PORTB_BASE, PB0_IntHandler);
    GPIOIntRegister(GPIO_PORTB_BASE, PB1_IntHandler);

    // Enable GPIO Interrupts
    GPIOIntEnable(GPIO_PORTB_BASE, GPIO_INT_PIN_0 | GPIO_INT_PIN_1);

    // Enable Master Interrupts
    IntMasterEnable();
}
