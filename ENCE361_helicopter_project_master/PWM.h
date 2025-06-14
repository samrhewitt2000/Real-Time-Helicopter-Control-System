#ifndef PWM_H_
#define PWM_H_

//*****************************************************************************
// 
//      PWM.h
//
// Header file for initializing and generating PWM signals to both main and tail rotors
//
//*****************************************************************************
//
// Author:          Caleb Westbury & Sam Hewitt
// Last modified:   May 2024
//
//*****************************************************************************

#include "communications.h"
#include <stdint.h>
#include "buttons.h"


/**********************************************************
 * Constants
 **********************************************************/
// Systick configuration
#define SYSTICK_RATE_HZ    100

// PWM configuration
#define PWM_START_RATE_HZ  250
#define PWM_RATE_STEP_HZ   50
#define PWM_RATE_MIN_HZ    50
#define PWM_RATE_MAX_HZ    400
#define PWM_FIXED_DUTY     60 // increase to about 70 for faster start rate
#define PWM_MAX_DUTY       98
#define PWM_MIN_DUTY       2
#define PWM_DIVIDER_CODE   SYSCTL_PWMDIV_4
#define PWM_DIVIDER        4

//  PWM Hardware Details M0PWM7 (gen 3)
//  ---Main Rotor PWM: PC5, J4-05
#define PWM_MAIN_BASE        PWM0_BASE
#define PWM_MAIN_GEN         PWM_GEN_3
#define PWM_MAIN_OUTNUM      PWM_OUT_7
#define PWM_MAIN_OUTBIT      PWM_OUT_7_BIT
#define PWM_MAIN_PERIPH_PWM  SYSCTL_PERIPH_PWM0
#define PWM_MAIN_PERIPH_GPIO SYSCTL_PERIPH_GPIOC
#define PWM_MAIN_GPIO_BASE   GPIO_PORTC_BASE
#define PWM_MAIN_GPIO_CONFIG GPIO_PC5_M0PWM7
#define PWM_MAIN_GPIO_PIN    GPIO_PIN_5

//  PWM Hardware Details M1PWM5 (gen 2???)
//  ---Tail Rotor PWM: PF1, J3-10
#define PWM_TAIL_BASE        PWM1_BASE
#define PWM_TAIL_GEN         PWM_GEN_2
#define PWM_TAIL_OUTNUM      PWM_OUT_5
#define PWM_TAIL_OUTBIT      PWM_OUT_5_BIT
#define PWM_TAIL_PERIPH_PWM  SYSCTL_PERIPH_PWM1
#define PWM_TAIL_PERIPH_GPIO SYSCTL_PERIPH_GPIOF
#define PWM_TAIL_GPIO_BASE   GPIO_PORTF_BASE
#define PWM_TAIL_GPIO_CONFIG GPIO_PF1_M1PWM5
#define PWM_TAIL_GPIO_PIN    GPIO_PIN_1


extern volatile uint32_t *ptr_main_duty_cycle;  // Pointer to the main rotor duty cycle
extern volatile uint32_t *ptr_tail_duty_cycle;  // Pointer to the tail rotor duty cycle



void SysTickIntHandler(void);  // Interrupt handler for SysTick

void initClocks(void);  // Initialize clocks

void initSysTick(void);  // Initialize SysTick

void initialise_rotor_PWM(void);  // Initialize main rotor PWM

void initialise_tail_PWM(void);   // Initialize tail rotor PWM

void set_rotor_PWM(uint32_t ui32RotorFreq, uint32_t ui32RotorDuty);  // Set main rotor PWM

void set_tail_PWM(uint32_t ui32TailFreq, uint32_t ui32TailDuty);   // Set tail rotor PWM

void kill_motors(void);  // Function to stop both rotors

#endif /*PWM_H_*/
