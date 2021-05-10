#include "../kernel/core.h"

void memcpy(char* source, char* dest, size_t nbytes);
void memset(uint8_t* dest, uint8_t val, size_t len);
void* malloc(size_t size);
void free(void* ptr);
void init_memory();

//----DEBUG-----//
int get_arr_ptr();