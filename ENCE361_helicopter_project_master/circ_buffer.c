//*****************************************************************************
//
//      circ_buffer.c
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
// Based on P.J. Bones' circBufT.c code from 2017
//
//*****************************************************************************

#include "circ_buffer.h"
#include <stdint.h>
#include "stdlib.h"

int buffer_full = 0; // Flag to indicate if the buffer is full
int *ptr_buffer_full = &buffer_full; // Pointer to the buffer full flag

// *******************************************************
// initCircBuf: Initialize the circBuf instance. Reset both indices to
// the start of the buffer. Dynamically allocate and clear the memory and 
// return a pointer for the data. Return NULL if allocation fails.
// buffer: Pointer to the circular buffer structure
// size: Size of the circular buffer
// *******************************************************
uint32_t * initCircBuf (circBuf_t *buffer, uint32_t size)
{
    buffer->windex = 0; // Write index
    buffer->rindex = 0; // Read index
    buffer->size = size; // Buffer size
    buffer->data = (uint32_t *) calloc (size, sizeof(uint32_t)); // Allocate memory
    return buffer->data; // Return pointer to the data array
}

// *******************************************************
// writeCircBuf: Insert entry at the current windex location,
// advance windex, modulo (buffer size).
// buffer: Pointer to the circular buffer structure
// entry: Value to be inserted into the buffer
// *******************************************************
void writeCircBuf (circBuf_t *buffer, uint32_t entry)
{
    buffer->data[buffer->windex] = entry; // Write entry at windex
    buffer->windex++; // Increment write index
    if (buffer->windex >= buffer->size) // Wrap around if necessary
        buffer->windex = 0;
    buffer_full = 1; // Set buffer full flag
}

// *******************************************************
// readCircBuf: Return entry at the current rindex location,
// advance rindex, modulo (buffer size). The function does not check
// if reading has advanced ahead of writing.
// buffer: Pointer to the circular buffer structure
// *******************************************************
uint32_t readCircBuf (circBuf_t *buffer)
{
    uint32_t entry;
    entry = buffer->data[buffer->rindex]; // Read entry at rindex
    buffer->rindex++; // Increment read index
    if (buffer->rindex >= buffer->size) // Wrap around if necessary
        buffer->rindex = 0;
    return entry; // Return the entry
}

// *******************************************************
// freeCircBuf: Releases the memory allocated to the buffer data,
// sets pointer to NULL and other fields to 0. The buffer can
// be re-initialized by another call to initCircBuf().
// buffer: Pointer to the circular buffer structure
// *******************************************************
void freeCircBuf (circBuf_t * buffer)
{
    buffer->windex = 0; // Reset write index
    buffer->rindex = 0; // Reset read index
    buffer->size = 0; // Reset size
    free (buffer->data); // Free allocated memory
    buffer->data = NULL; // Set data pointer to NULL
}

// *******************************************************
// sum_CircBuf_vals: Sums all values in the circular buffer.
// sum: Initial sum value
// buffer: Pointer to the circular buffer structure
// buf_size: Size of the buffer to be summed
// Returns: Sum of all values in the buffer
// *******************************************************
uint32_t sum_CircBuf_vals (uint32_t sum, circBuf_t *buffer, uint32_t buf_size)
{
    uint16_t i;
    for (i = 0; i < buf_size; i++) {
        sum = sum + readCircBuf (buffer); // Add each buffer value to sum
    }
    return sum; // Return the total sum
}
