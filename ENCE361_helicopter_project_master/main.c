//*****************************************************************************
// 
//      main.c
//
// What does this module do? (Replace)
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
#include "quad_enc.h"
#include "displays.h"
#include "ADC.h"
#include "buttons5.h"

#include "inc/hw_ints.h"

//*****************************************************************************
// Constants
//*****************************************************************************
#define BUF_SIZE 10

// *******************************************************
// Display state enum
// *******************************************************
typedef enum {
    STATE_PERC,             // display altitude percentages state
    STATE_MEAN_ADC_VAL,     // display mean adc values state
    STATE_OFF,              // screen off state
} display_state_t;



int main(void)
 {
    int32_t initial_ADC_val = 0;    // initialize first value
    int32_t current_ADC_val = 0;    // initialize first value
    
    // Enable interrupts to the processor.
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF))
    {
    }
    initButtons();
    initClock ();
    initADC ();
    initDisplay ();
    initYaw ();
    initCircBuf (&g_inBuffer, BUF_SIZE);

    //SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
//    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD_WPD);
//    GPIODirModeSet(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_DIR_MODE_OUT);


    // calculate exactly how long this needs to be
    SysCtlDelay (SysCtlClockGet() / 6); // delay so that buffer can fill
    initial_ADC_val = get_ADC_val(&g_inBuffer, BUF_SIZE);

    //prev_phase = get_current_phase();

    display_state_t current_state;
    current_state = STATE_PERC; //initialize display state
    //




    IntMasterEnable();

    while (1)
    {
        //
        // Background task: calculate the (approximate) mean of the values in the
        // circular buffer and display it, together with the sample number.
        current_ADC_val = get_ADC_val(&g_inBuffer, BUF_SIZE);

        if (checkButton(LEFT) == PUSHED) {
            initial_ADC_val = current_ADC_val;
        }

       switch(current_state)
       {
       case STATE_PERC:
           displayAltitudePerc(current_ADC_val, initial_ADC_val, 0, 1);
           //GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);
           //SysCtlDelay(SysCtlClockGet() / yaw_angle);
           //GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0x00);
           displayYaw(0, 2);
           break;
       case STATE_MEAN_ADC_VAL:
           // Calculate and display the rounded mean of the buffer contents
           displayADCVal (current_ADC_val, 0, 1);
           break;
       case STATE_OFF:
           displayNothing();
           break;
       }
       /*if (checkButton(UP) == PUSHED)
       {
           if (current_state != STATE_OFF) {
               current_state++;
           }
           else {
               current_state = STATE_PERC;
           }
       }*/
       SysCtlDelay (SysCtlClockGet() / 24);  // Update display at ~ 2 Hz

    }
}

