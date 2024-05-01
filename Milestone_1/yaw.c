/*
 * quadrature.c
 *
 *  Created on: 1/05/2024
 *      Author: she108
 */


#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/adc.h"
#include "driverlib/pwm.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/interrupt.h"
#include "driverlib/debug.h"
#include "utils/ustdlib.h"




SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB); //initialize gpioa

While(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB))
{
	}

//register interrupthandler
GPIOIntRegister(GPIO_PORTB_BASE, PORTBIntHandler);

// make pins PB0 and PB1 inputs
GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, GPIO_PIN_45, GPIO_PIN_46);

// both pins interrupt triggered by high level (could be rising/falling edge instead, not sure)
GPIOIntTypeSet(GPIO_PORTB_BASE, GPIO_PIN_45 | GPIO_PIN_46, GPIO_HIGH_LEVEL);


int PinAVal = GPIOPinRead(GPIO_PORTB_BASE, (GPIO_PIN_45));

//I think there should be one for each pin??  not sure

//enable pin interrupts
GPIOIntEnable(GPIO_PORTB_BASE, GPIO_PIN_45 | GPIO_PIN_46);

void
PORTBIntHandler()
{

	//code stuff goes here


	GPIOIntClear(GPIO_PORTB_BASE. GPIO_PIN_45);
	GPIOIntClear(GPIO_PORTB_BASE. GPIO_PIN_46);
}





switch(pinState)
