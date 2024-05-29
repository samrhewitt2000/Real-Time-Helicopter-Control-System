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

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/uart.h"
#include "buttons.h"
#include "yaw_control.h"


#define MAX_STR_LEN 16




char statusStr[MAX_STR_LEN + 1];

void initialise_USB_UART(void);

void UART_send(char *pucBuffer);

void UART_transmit_info(int32_t yaw_setpoint, int32_t yaw_actual, int32_t alt_setpoint, int32_t alt_actual, int32_t tail_duty, int32_t main_duty, helicopter_state_t mode);


#endif /*COMMUNICATIONS_H_*/

