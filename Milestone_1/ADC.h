#ifndef ADC_H_
#define ADC_H_


//*****************************************************************************
//
// ADC.h - ADC logic for the Tiva board
//
// Author:  Caleb Westbury & Sam
// Last modified:   22/03/2024
//
//*****************************************************************************
// Based on the 'convert' series from 2016
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
update_ADC_val(circBuf_t *buffer, uint32_t buf_size);

#endif /*ADC_H_*/
