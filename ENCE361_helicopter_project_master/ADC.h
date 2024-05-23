#ifndef ADC_H_
#define ADC_H_

//*****************************************************************************
//
//      ADC.h
//
// What does this function do? (Replace)
//
// *****************************************************************************
//
// Author:          Caleb Westbury & Sam Hewitt
// Last modified:   May 2024
//
//*****************************************************************************

#include "circ_buffer.h"

void SysTickIntHandler(void);

//*****************************************************************************
// The handler for the ADC conversion complete interrupt.
// Writes to the circular buffer.
//*****************************************************************************
void ADCIntHandler(void);

//*****************************************************************************
// Initialisation functions for the clock (incl. SysTick), ADC, display
//*****************************************************************************
void initClock (void);

//*****************************************************************************
// Initializes ADC0 to take single samples from CH9 (pin PE4, AIN9)
//*****************************************************************************
void initADC (void);

//*****************************************************************************
// The code for calculating the ADC value
//*****************************************************************************
int32_t get_ADC_val(circBuf_t *buffer, uint32_t buf_size);

#endif /*ADC_H_*/
