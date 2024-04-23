#ifndef _FIFO_H_
#define _FIFO_H_

#include <stdio.h>
#include "91x_lib.h"

// the fifo object
typedef struct
{
    u8 *buffer;                 // pointer to start of the ringbuffer
    u8 count;                // number of characters in FIFO
    u16 size;                // buffer size
    u8 *pread;               // read pointer
    u8 *pwrite;              // write pointer
} fifo_t;

/*
The initialization of the FIFO sets the read/write pointers etc..
The FIFO uses the buffer 'buf' which byte length must 'size'.
Returns 1 on success ans 0 in case of an error.
*/
u8 fifo_init (fifo_t* f, u8* buf, const u16 size);

/*
Puts a byte into the FIFO. Returns 1 on success and 0 in case of FIFO overflow.
*/
u8 fifo_put (fifo_t* f, const u8 data);

/*
Get the next byte from the FIFO. Returns 0 if the FIFO is empty.
*/
u8 fifo_get (fifo_t* f, u8* pdata);

/*
Get the next byte out of the FIFO. If the FIFO is empty the function blocks
until the next byte is put into the FIFO.
*/
u8 fifo_get_wait (fifo_t* f, u8* pdata);

/*
Purges the FIFO so that it is empty afterwards
*/
void fifo_purge (fifo_t* f);

#endif /* _FIFO_H_ */
