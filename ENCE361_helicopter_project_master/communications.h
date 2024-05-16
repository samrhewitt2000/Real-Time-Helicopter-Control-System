#ifndef COMMUNICATIONS_H_
#define COMMUNICATIONS_H_

//*****************************************************************************
// 
//      communications.h
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

typedef enum
{
    LANDED,
    LANDING,
    TAKEOFF,
    FLYING
} helicopter_state_t;

void initialise_USB_UART(void);

void UART_send(char *pucBuffer);

void UART_transmit_info(int32_t yaw_setpoint, int32_t yaw_actual, int32_t alt_setpoint, int32_t alt_actual, int32_t tail_duty, int32_t main_duty, helicopter_state_t mode);

#endif /*COMMUNICATIONS_H_*/