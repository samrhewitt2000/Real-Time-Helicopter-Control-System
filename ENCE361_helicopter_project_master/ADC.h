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

extern circBuf_t g_inBuffer;

void SysTickIntHandler(void);

void ADCIntHandler(void);

void initClock (void);

void initADC (void);

#endif /*ADC_H_*/
