#include "memory.h"
#include "../drivers/screen.h"

void memcpy(char* pSource, char* pDest, size_t nbytes)
{
    for (int i = 0; i < nbytes; i++)
    {
        *(pDest + i) = *(pSource + i);
    }
}

void memset(uint8_t* pDest, uint8_t val, size_t len)
{
    uint8_t* temp = (uint8_t*)pDest;
    for (; len != 0; len--) *temp++ = val;
}

typedef struct chunk
{
    void* start;
    uint8_t free;
    size_t size;

}chunk_t;

chunk_t chunks[500];

void* allocptr;
int arrptr;

void* find_memory(size_t size)
{
    for (int i = 0; i < arrptr; i++)
    {
        if (chunks[i].free && chunks[i].size >= size)
        {
            chunks[i].free = 0;
            return chunks[i].start;
        }
    }

    return 0;
}

void* malloc(size_t size)
{
    void* temp = find_memory(size);

    if (temp)
        return temp;

    chunks[arrptr].free = 0;
    chunks[arrptr].size = size;
    chunks[arrptr].start = allocptr;
    arrptr++;

    temp = allocptr;
    allocptr += size;

    return temp;
}

void free(void* ptr)
{
    for (int i = 0; i < arrptr; i++)
    {
        if (chunks[i].start == ptr)
        {
            chunks[i].free = 1;
        }
    }
}

void init_memory()
{
    allocptr = (void*)0x10000;
    arrptr = 0;
}

int get_arr_ptr()
{
    return arrptr;
}