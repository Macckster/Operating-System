#include "util.h"

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

    str_reverse(str);
}