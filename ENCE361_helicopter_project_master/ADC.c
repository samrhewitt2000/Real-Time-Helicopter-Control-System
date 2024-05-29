//*****************************************************************************
//
//      ADC.c
//
// What does this function do? (Replace)
//
// *****************************************************************************
//
// Author:          Caleb Westbury & Sam Hewitt
// Last modified:   May 2024
//
//*****************************************************************************
//
// Based on P.J. Bones' ADCdemo1.c code from 2018
//
//*****************************************************************************

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
#include "OrbitOLED/OrbitOLEDInterface.h"

#include "displays.h"
#include "ADC.h"
#include "buttons.h"

//*****************************************************************************
// Global variables
//*****************************************************************************

#define SAMPLE_RATE_HZ 100

circBuf_t g_inBuffer;

static uint32_t g_ulSampCnt;    // Counter for the interrupts

//*****************************************************************************
//
// The interrupt handler for the for SysTick interrupt.
//
//*****************************************************************************
void SysTickIntHandler(void)
{
    //
    // Initiate a conversion
    //
    ADCProcessorTrigger(ADC0_BASE, 3); 
    updateButtons();
    g_ulSampCnt++;
}



//*****************************************************************************
// The handler for the ADC conversion complete interrupt.
// Writes to the circular buffer.
//*****************************************************************************
void ADCIntHandler(void)
{
    uint32_t ulValue;

    //
    // Get the single sample from ADC0.  ADC_BASE is defined in
    // inc/hw_memmap.h
    ADCSequenceDataGet(ADC0_BASE, 3, &ulValue);
    //
    // Place it in the circular buffer (advancing write index)
    writeCircBuf (&g_inBuffer, ulValue);

    //
    // Clean up, clearing the interrupt
    ADCIntClear(ADC0_BASE, 3);
}



//*****************************************************************************
// initClock: Initialisation functions for the clock (incl. SysTick), ADC, display
//*****************************************************************************
//void initClock (void)
//{
//    // Set the clock rate to 20 MHz
//    SysCtlClockSet (SYSCTL_SYSDIV_10 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |
//                   SYSCTL_XTAL_16MHZ);
//    //
//    // Set up the period for the SysTick timer.  The SysTick timer period is
//    // set as a function of the system clock.
//    SysTickPeriodSet(SysCtlClockGet() / SAMPLE_RATE_HZ);
//    //
//    // Register the interrupt handler
//    SysTickIntRegister(SysTickIntHandler);
//    //
//    // Enable interrupt and device
//    SysTickIntEnable();
//    SysTickEnable();
//}



//*****************************************************************************
//initADC: Initializes ADC0 to take single samples from CH9 (pin PE4, AIN9)
//*****************************************************************************
void initADC (void)
{
    //
    // The ADC0 peripheral must be enabled for configuration and use.
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_ADC0))
    {
    }
    // Enable sample sequence 3 with a processor signal trigger.  Sequence 3
    // will do a single sample when the processor sends a signal to start the
    // conversion.
    ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);
  
    //
    // Configure step 0 on sequence 3.  Sample channel 0 (ADC_CTL_CH0) in
    // single-ended mode (default) and configure the interrupt flag
    // (ADC_CTL_IE) to be set when the sample is done.  Tell the ADC logic
    // that this is the last conversion on sequence 3 (ADC_CTL_END).  Sequence
    // 3 has only one programmable step.  Sequence 1 and 2 have 4 steps, and
    // sequence 0 has 8 programmable steps.  Since we are only doing a single
    // conversion using sequence 3 we will only configure step 0.  For more
    // on the ADC sequences and steps, refer to the LM3S1968 datasheet.
    ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_CH9 | ADC_CTL_IE |
                             ADC_CTL_END);    
                             
    //
    // Since sample sequence 3 is now configured, it must be enabled.
    ADCSequenceEnable(ADC0_BASE, 3);
  
    //
    // Register the interrupt handler
    ADCIntRegister (ADC0_BASE, 3, ADCIntHandler);
  
    //
    // Enable interrupts for ADC0 sequence 3 (clears any outstanding interrupts)
    ADCIntEnable(ADC0_BASE, 3);
}



//*****************************************************************************
// get_ADC_val: The code for calculating the ADC value
//*****************************************************************************
int32_t get_ADC_val(circBuf_t *buffer, uint32_t buf_size)
{
    int32_t sum = 0;
    sum = sum_CircBuf_vals (sum, buffer, buf_size);
    return (2 * sum + buf_size) / 2 / buf_size;
}
