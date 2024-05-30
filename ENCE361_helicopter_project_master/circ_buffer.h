#ifndef CIRC_BUFFER_H_
#define CIRC_BUFFER_H_

//*****************************************************************************
//
//      circ_buffer.h
//
// Support for a circular buffer of uint32_t values on the 
// Tiva processor.
//
//*****************************************************************************
//
// Author:          Caleb Westbury & Sam Hewitt
// Last modified:   May 2024
//
//*****************************************************************************
//
// Based on P.J. Bones' circBufT.h code from 2017
//
//*****************************************************************************

#include <stdint.h>

// *******************************************************
// Buffer structure: to hold the circular buffer data and indices
// *******************************************************
typedef struct {
    uint32_t size;   // Number of entries in the buffer
    uint32_t windex; // Index for writing, modulo (size)
    uint32_t rindex; // Index for reading, modulo (size)
    uint32_t *data;  // Pointer to the data array
} circBuf_t;

extern circBuf_t g_inBuffer;  // Global instance of the input buffer
extern int buffer_full;       // Flag to indicate if the buffer is full
extern int *ptr_buffer_full;  // Pointer to the buffer full flag

// *******************************************************
// initCircBuf: Initialize the circBuf instance. Reset both indices to
// the start of the buffer. Dynamically allocate and clear the memory and 
// return a pointer for the data. Return NULL if allocation fails.
// buffer: Pointer to the circular buffer structure
// size: Size of the circular buffer
// Returns: Pointer to the data array or NULL if allocation fails
// *******************************************************
uint32_t * initCircBuf (circBuf_t *buffer, uint32_t size);

// *******************************************************
// writeCircBuf: Insert entry at the current windex location,
// advance windex, modulo (buffer size).
// buffer: Pointer to the circular buffer structure
// entry: Value to be inserted into the buffer
// *******************************************************
void writeCircBuf (circBuf_t *buffer, uint32_t entry);

// *******************************************************
// readCircBuf: Return entry at the current rindex location,
// advance rindex, modulo (buffer size). The function does not check
// if reading has advanced ahead of writing.
// buffer: Pointer to the circular buffer structure
// Returns: Value read from the buffer
// *******************************************************
uint32_t readCircBuf (circBuf_t *buffer);

// *******************************************************
// freeCircBuf: Releases the memory allocated to the buffer data,
// sets pointer to NULL and other fields to 0. The buffer can be 
// re-initialized by another call to initCircBuf().
// buffer: Pointer to the circular buffer structure
// *******************************************************
void freeCircBuf (circBuf_t *buffer);

// *******************************************************
// sum_CircBuf_vals: Sums all values in the circular buffer.
// sum: Initial sum value
// buffer: Pointer to the circular buffer structure
// buf_size: Size of the buffer to be summed
// Returns: Sum of all values in the buffer
// *******************************************************
uint32_t sum_CircBuf_vals (uint32_t sum, circBuf_t *buffer, uint32_t buf_size);

#endif /* CIRC_BUFFER_H_ */
