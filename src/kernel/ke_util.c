#include "ke_util.h"

void memcpy(char* source, char* dest, int nbytes)
{
    for (int i = 0; i < nbytes; i++)
    {
        *(dest + i) = *(source + i);
    }
}

void memset(u8* dest, u8 val, u32 len)
{
    u8* temp = (u8*)dest;
    for (; len != 0; len--) *temp++ = val;
}

/**
 * K&R implementation
 */
void itoa(int n, char str[])
{
    int i = 0, sign = 0;
    if ((sign = n) < 0) n = -n;
    i = 0;
    do
    {
        str[i++] = n % 10 + '0';
    } while ((n /= 10) > 0);

    if (sign < 0) str[i++] = '-';
    str[i] = '\0';

    ke_str_reverse(str);
}