#include "fifo.h"

u8 fifo_init (fifo_t *f, u8 *buffer, const u16 size)
{
    if(f == NULL) return(0);
    f->buffer = buffer;
    f->size = size;
    fifo_purge(f);
    return(1);
}

u8 fifo_put (fifo_t *f, const u8 data)
{
    if (f->count >= f->size) return(0);    // return 0 in case of FIFO overflow.

    u8 * pwrite = f->pwrite;

    *(pwrite++) = data;        // copy data byte to buffer
    if(pwrite >= f->buffer + f->size) pwrite = f->buffer; // start at the begining after reaching the end 
    f->pwrite = pwrite;
    f->count++;
    return 1;
}

u8 fifo_get (fifo_t *f, u8 *pdata)
{
    if((f == NULL) || (pdata == NULL)) return(0);
    if(!f->count) return(0);

    u8 *pread = f->pread;
    *pdata = *(pread++);
    if(pread >= f->buffer + f->size) pread = f->buffer; // start at the begining after reaching the end 
    f->pread = pread;
    f->count--;
    return(1);
}

u8 fifo_get_wait (fifo_t *f, u8 *pdata)
{
    while (!f->count);

    return fifo_get(f, pdata);
}

void fifo_purge(fifo_t* f)
{
    if((f == NULL)) return;
    f->count = 0;
    f->pread = f->buffer;
    f->pwrite = f->buffer; 
    return;    
}
