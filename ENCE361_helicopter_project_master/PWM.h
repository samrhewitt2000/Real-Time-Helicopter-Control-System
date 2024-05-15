#ifndef PWM_H_
#define PWM_H_

//*****************************************************************************
// 
//      PWM.h
//
// What does this function do? (Replace)
//
//*****************************************************************************
//
// Author:          Caleb Westbury & Sam Hewitt
// Last modified:   May 2024
//
//*****************************************************************************


void initialise_PWM (void);

void set_rotor_PWM (uint32_t ui32Duty);

void set_tail_PWM (uint32_t ui32Duty);

#endif /*PWM_H_*/