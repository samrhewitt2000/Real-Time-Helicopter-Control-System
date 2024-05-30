//*****************************************************************************
//
//      communications.c
//
// This file contains functions to initialize and handle UART communications
// for transmitting helicopter status information over a serial link.
//
//*****************************************************************************
//
// Author:          Caleb Westbury & Sam Hewitt
// Last modified:   May 2024
//
//*****************************************************************************
//
// Based on P.J. Bones' UART.c code from 2017
//
//*****************************************************************************

#include "communications.h"

#define BAUD_RATE 9600  // Define the baud rate for UART communication

//*****************************************************************************************************
// initialise_USB_UART: Initializes UART0 for serial communication with specified settings.
//*****************************************************************************************************
void initialise_USB_UART(void)
{
    // Enable the peripherals for UART0 and GPIOA
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    // Configure GPIO pins for UART mode
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    // Configure UART0 settings: 9600 baud rate, 8 data bits, 1 stop bit, no parity
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), BAUD_RATE, UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE);
    
    // Enable UART FIFO and UART0
    UARTFIFOEnable(UART0_BASE);
    UARTEnable(UART0_BASE);
}

//*****************************************************************************************************
// UART_send: Sends a string via UART0.
// pucBuffer: Pointer to the string to be sent
//*****************************************************************************************************
void UART_send(char *pucBuffer)
{
    // Transmit each character in the string until null terminator is encountered
    while (*pucBuffer)
    {
        UARTCharPut(UART0_BASE, *pucBuffer);
        pucBuffer++;
    }
}

//*****************************************************************************************************
// UART_transmit_info: Transmits all the helicopter status information via UART0.
// yaw_setpoint: Desired yaw angle
// yaw_actual_int: Actual yaw angle integer part
// yaw_actual_decimal: Actual yaw angle decimal part
// alt_setpoint: Desired altitude
// alt_actual: Actual altitude
// tail_duty: Tail rotor duty cycle
// main_duty: Main rotor duty cycle
// mode: Current operating mode of the helicopter
//*****************************************************************************************************
void UART_transmit_info(int32_t yaw_setpoint, int32_t yaw_actual_int, int32_t yaw_actual_decimal, int32_t alt_setpoint, int32_t alt_actual, int32_t tail_duty, int32_t main_duty, helicopter_state_t mode)
{
    // Calculate yaw angle decimal value
    int32_t yaw_angle_decimal = abs(((360 * quad_enc_ticks) % 448 * 10) / 448);
    int32_t yaw_angle_int = abs(360 * quad_enc_ticks / 448);

    // Create the status string to be transmitted based on the current quad encoder ticks
    if (quad_enc_ticks < 0) {
        usprintf(statusStr,
                    "set yaw:  -%2d yaw: -%d.%d\n set alt: %2d alt: %2d\n tail duty: %2d main duty: %2d\n mode: %c\n", yaw_setpoint, yaw_actual_int, yaw_actual_decimal, alt_setpoint, alt_actual, tail_duty, main_duty, mode);
    } else {
        usprintf(statusStr,
                    "set yaw:  %2d yaw: %d.%d\n set alt: %2d alt: %2d\n tail duty: %2d main duty: %2d\n mode: %c\n", yaw_setpoint, yaw_actual_int, yaw_actual_decimal, alt_setpoint, alt_actual, tail_duty, main_duty, mode);
    }

    // Send the formatted status string via UART0
    UART_send(statusStr);
}
