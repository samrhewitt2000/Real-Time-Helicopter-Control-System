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

#include "circBuffer.h"

extern circBuf_t g_inBuffer;

void
SysTickIntHandler(void);

void
ADCIntHandler(void);

void
initClock (void);

void
initADC (void);

int32_t
get_ADC_val(circBuf_t *buffer, uint32_t buf_size);

#endif /*ADC_H_*/
