//*****************************************************************************
//
// main.c - main logic for the Tiva board
//
// Author:  Caleb Westbury & Sam
// Last modified:   22/03/2024
//
//*****************************************************************************
// Based on the 'convert' series from 2016
//*****************************************************************************


#include "circBuffer.h"


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
#include "buttons5.h"


//*****************************************************************************
// Constants
//*****************************************************************************
#define BUF_SIZE 10


int
main(void)
{
    uint32_t sum = 0;

    initClock ();
    initADC ();
    initDisplay ();
    initCircBuf (&g_inBuffer, BUF_SIZE);
    //uint16_t count = 0;

    int32_t initial_ADC_val = 0;    // initialize first value
    initial_ADC_val = loopCircBuf (sum, &g_inBuffer, BUF_SIZE);
    displayADCVal (initial_ADC_val);

    SysCtlDelay(SysCtlClockGet() /10);
    initButtons();


    typedef enum {
        STATE_PERC,             // display altitude percentages state
        STATE_MEAN_ADC_VAL,     // display mean adc values state
        STATE_OFF,              // screen off state
    } display_state_t;



    display_state_t current_state;
    current_state = STATE_PERC; //initialize display state
    //
    // Enable interrupts to the processor.
    IntMasterEnable();

    while (1)
    {
        updateButtons();
        //
        // Background task: calculate the (approximate) mean of the values in the
        // circular buffer and display it, together with the sample number.
        sum = 0;
        sum = loopCircBuf (sum, &g_inBuffer, BUF_SIZE);

       /*if (count > 20) */
       switch(current_state)
       {
       case STATE_PERC:
           displayAltitudePerc((2 * sum + BUF_SIZE) / 2 / BUF_SIZE, initial_ADC_val);
           break;
       case STATE_MEAN_ADC_VAL:
           // Calculate and display the rounded mean of the buffer contents
           displayADCVal ((2 * sum + BUF_SIZE) / 2 / BUF_SIZE);
           break;
       case STATE_OFF:
           displayNothing();
           break;
       }

       if (checkButton(UP) == PUSHED)
       {
           if (current_state < STATE_OFF) {
               current_state++;
           }

       } else if (checkButton(DOWN) == PUSHED)
       {
           if (current_state > 0) {
               current_state--;
           }
       }


        SysCtlDelay (SysCtlClockGet() / 6);  // Update display at ~ 2 Hz


        //count++;
    }
}
