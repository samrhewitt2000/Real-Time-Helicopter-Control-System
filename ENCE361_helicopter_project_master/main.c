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

#include "inc/hw_ints.h"

#include "circ_buffer.h"
#include "quad_enc.h"
#include "displays.h"
#include "ADC.h"
#include "buttons.h"
#include "PWM.h"
#include "kernel.h"
#include "alt_control.h"
#include "yaw_control.h"
#include "communications.h"
//*****************************************************************************
// Constants
//*****************************************************************************




// *******************************************************
// Display state enum
// *******************************************************
typedef enum {
    STATE_PERC,             // display altitude percentages state
    STATE_MEAN_ADC_VAL,     // display mean adc values state
    STATE_OFF,              // screen off state
} display_state_t;



// *******************************************************
// Global Variables
// *******************************************************
extern int32_t initial_ADC_val = 0;    // initialize first value
volatile extern int32_t current_ADC_val = 0;    // initialize first value
uint32_t ui32RotorFreq = PWM_START_RATE_HZ;
uint32_t ui32RotorDuty = PWM_FIXED_DUTY;
uint32_t ui32TailFreq = PWM_START_RATE_HZ;
uint32_t ui32TailDuty = PWM_FIXED_DUTY;

helicopter_state_t heli_state = YAW_REF; //INITIAL_REFize heli state
int32_t current_switch_state;
uint32_t counter = 0;
extern unsigned char num_tasks;
static circBuf_t g_inbuffer;



//declare task IDs globally
unsigned char ref_yaw_task_ID;
unsigned char switch_task_ID;
unsigned char push_buttons_task_ID;
unsigned char alt_control_task_ID;
unsigned char yaw_control_task_ID;





// *******************************************************
// Function prototypes
// *******************************************************
void increase_altitude_task(void);
void decrease_altitude_task(void);
void find_reference_yaw_task(void);


//********************************************************
// Function to set the freq, duty cycle of M1PWM5 (tail motor)
// ********************************************************
//void kill_motors(helicopter_state_t heli_state)
//{
//    //set main motor PWM signal to zero
//    PWMGenPeriodSet(PWM_MAIN_BASE, PWM_MAIN_GEN, 0);
//    PWMPulseWidthSet(PWM_MAIN_BASE, PWM_MAIN_OUTNUM, 0);
//
//    //set tail motor PWM signal to zero
//    PWMGenPeriodSet(PWM_TAIL_BASE, PWM_TAIL_GEN, 0);
//    PWMPulseWidthSet(PWM_TAIL_BASE, PWM_TAIL_OUTNUM, 0);
//
//    heli_state = LANDED;
//}



//********************************************************
//
// ********************************************************
void initialise_program(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF))
    {
    }
    pK_init(MAX_TASKS, SysCtlClockGet() / 100); // e.g., 10ms tick period
    initButtons();
    //initClocks ();
    initADC ();
    initDisplay ();
    initYaw ();
    init_ref_yaw();
    initialise_rotor_PWM ();
    initialise_tail_PWM ();
    PWMOutputState(PWM_MAIN_BASE, PWM_MAIN_OUTBIT, true);
    PWMOutputState(PWM_TAIL_BASE, PWM_TAIL_OUTBIT, true);

    // System initialization (e.g., clock setup, peripherals)




    //initialise buffer
    initCircBuf (&g_inBuffer, BUF_SIZE);
    SysCtlDelay (SysCtlClockGet() / 6); // delay so that buffer can fill
}

//void get_ADC_task(void)
//{
//    current_ADC_val = get_ADC_val(&g_inBuffer, BUF_SIZE);
//}


void push_buttons_task(void)
{
    updateButtons();
//    counter += 1;
//    if (checkButton(UP) == PUSHED && heli_state == FLYING)
//    {
//        set_rotor_PWM(250, *ptr_main_duty_cycle + 10);
//        //increase altitude by 10%
//        //change_altitude(alt_val_to_percent(initial_ADC_val, current_ADC_val), 10);
//    }else if (checkButton(DOWN) == PUSHED && heli_state == FLYING)
//    {
//        set_rotor_PWM(250, *ptr_main_duty_cycle - 10);
//        //decrease altitude by 10%
//        //change_altitude(alt_val_to_percent(initial_ADC_val, current_ADC_val), -10);
//    }else if (checkButton(SWITCH) == PUSHED && heli_state == LANDED)
//    {
//        heli_state = TAKEOFF;
//    }else if (checkButton(SWITCH) == RELEASED && heli_state == FLYING)
//    {
//        heli_state = LANDING;
//    }
}



void get_sensor_values(void)
{
    current_ADC_val = get_ADC_val(&g_inBuffer, BUF_SIZE);
}



//*****************************************************************************
//
//*****************************************************************************
void register_all_pk_tasks(void)
{
    ref_yaw_task_ID = pK_register_task(find_reference_yaw_task, 1);
    switch_task_ID = pK_register_task(switch_task, 200);
    push_buttons_task_ID = pK_register_task(push_buttons_task, 1);
    alt_control_task_ID = pK_register_task(alt_control_task, 2);
    yaw_control_task_ID = pK_register_task(yaw_control_task, 3);
}



int main(void)
{
    initialise_program();



    IntMasterEnable();



    register_all_pk_tasks();
    pK_block_all_tasks();
    while (1)
    {
        //current_switch_state = GPIOPinRead (SWITCH_PORT_BASE, SWITCH_PIN) == SWITCH_PIN;

        switch(heli_state)
        {
            case YAW_REF:
                pK_ready_task(ref_yaw_task_ID);
                //ready reference yaw task to use tail motor to find reference yaw
                displayYaw(0, 3);
                break;
            case LANDED:
                kill_motors();
                pK_ready_task(switch_task_ID);
                displayYaw(0, 3);
                // set rotor and tail motors to zero
                //pK_ready_task(push_buttons_task_ID);
                break;
            case TAKEOFF:
                // helicopter calibrates to reference yaw when take off switch pressed

                //change_yaw_angle(0 - quad_enc_ticks, *ptr_main_duty_cycle);
                set_rotor_PWM(250, 60);
                break;
            case FLYING:
                displayYaw(0, 3);
                pK_ready_task(push_buttons_task_ID);
                pK_ready_task(switch_task_ID);
                // helicopter doesnt spaz when both yaw and altitude pressed consecutively
                // alt in range 0 - 100 and pwm duty in range 2 - 98
                break;
            case LANDING:
                displayYaw(0, 3);
                //change_altitude(0, -100);
                // When helicopter is landing pressing buttons or switches do nothing
                // helicopter should return to reference yaw and land smoothly
                kill_motors();
                heli_state = LANDED;
                break;
        }
        pK_start();
        pK_block_all_tasks();

    }
}


