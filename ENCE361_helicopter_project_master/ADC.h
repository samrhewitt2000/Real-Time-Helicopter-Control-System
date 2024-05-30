#ifndef ADC_H_
#define ADC_H_

//*****************************************************************************
//
//      ADC.h
//
// 
//
// *****************************************************************************
//
// Author:          Caleb Westbury & Sam Hewitt
// Last modified:   May 2024
//
//*****************************************************************************

#include "circ_buffer.h"

#define BUF_SIZE 10

volatile extern int32_t current_ADC_val;
extern int32_t initial_ADC_val;
extern circBuf_t g_inBuffer;
volatile uint32_t g_ulSampCnt;


//*****************************************************************************
// The handler for the ADC conversion complete interrupt.
// Writes to the circular buffer.
//*****************************************************************************
void ADCIntHandler(void);


//*****************************************************************************
// Initializes ADC0 to take single samples from CH9 (pin PE4, AIN9)
//*****************************************************************************
void initADC (void);

//*****************************************************************************
// The code for calculating the ADC value
//*****************************************************************************
int32_t get_ADC_val(circBuf_t *buffer, uint32_t buf_size);

#endif /*ADC_H_*/
