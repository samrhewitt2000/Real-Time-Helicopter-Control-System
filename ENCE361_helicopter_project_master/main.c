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
#include "yaw.h"
#include "displays.h"
#include "ADC.h"
#include "buttons.h"
#include "inc/hw_ints.h"

typedef enum
{
    
    LANDED,
    LANDING,
    TAKEOFF,
    FLYING

} helicopter_state_t;



//*****************************************************************************
//
//*****************************************************************************
void init_system(void);
{
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
}



//*****************************************************************************
//
//*****************************************************************************
int main(void)
 {
    int32_t initial_ADC_val = 0;    // initialize first value
    int32_t current_ADC_val = 0;    // initialize first value
    
    init_system();

    // calculate exactly how long this needs to be
    SysCtlDelay (SysCtlClockGet() / 6); // delay so that buffer can fill
    initial_ADC_val = get_ADC_val(&g_inBuffer, BUF_SIZE);

    helicopter_state_t current_heli_state = LANDED;

    IntMasterEnable();

    while (1)
    {
        updateButtons()

        switch(current_heli_state)
        {
            case LANDED:
                if ()
                break;
            case LANDING:
                break;
            case TAKEOFF:
                break;
            case FLYING:
                break;
        }
        SysCtlDelay (SysCtlClockGet() / 24);  // Update display at ~ 2 Hz
    }
}