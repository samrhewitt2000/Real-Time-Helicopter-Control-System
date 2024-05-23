//*****************************************************************************
//
//      communications.c
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

// Information on the status of the helicopter should be transmitted via a serial link from
// UART0 at 9600 baud, with 1 stop bit and no parity bit in each transmitted byte. Status 3
// information should include the desired and actual yaw (in degrees), the desired and actual
// altitude (as a percentage of the maximum altitude), the duty cycle of each of the PWM
// signals controlling the rotors (%, with 0 meaning off) and the current operating mode.
// The information needs to be in a concise but easily readable format that is compatible
// with the scrolling display on the remote lab web page. Updates should be transmitted at
// regular intervals (no fewer than 4 updates per second).

#include "communications.h"

#define BAUD_RATE 9600

//*****************************************************************************************************
// get_ADC_val: returns the average ADC value from the values stored in the circular buffer structure
//*****************************************************************************************************
void initialise_USB_UART(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);


    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), BAUD_RATE, UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE);
    UARTFIFOEnable(UART0_BASE);
    UARTEnable(UART0_BASE);
}



//*****************************************************************************************************
//
//*****************************************************************************************************
void UART_send(char *pucBuffer)
{
    while (*pucBuffer)
    {
        UARTCharPut(UART0_BASE, *pucBuffer);
        pucBuffer++;
    }
}



//*****************************************************************************************************
// transmits all the information required for
//*****************************************************************************************************
void UART_transmit_info(int32_t yaw_setpoint, int32_t yaw_actual, int32_t alt_setpoint, int32_t alt_actual, int32_t tail_duty, int32_t main_duty, helicopter_state_t mode)
{
    //yaw_setpoint = yaw_angle_ticks_to_int(int32_t quad_enc_ticks);

    sprintf(statusStr,
            "desired yaw:  %2d\n actual yaw: %2d\n desired alt: %2d\n actual alt: %2d\n tail duty: %2d\n main duty: %2d\n mode: %c\n",
            yaw_setpoint, yaw_actual, alt_setpoint, alt_actual, tail_duty, main_duty, mode);
    //UART_Send();
}
