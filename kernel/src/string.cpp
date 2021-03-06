#include "string.hpp"

//Compute length of string
int strlen(char* pszMsg)
{
	int counter = 0;
	while (*pszMsg)
	{
		counter++;
		pszMsg++;
	}

	return counter;
}

int strnlen(const char* pszMsg, int maxlen)
{
	int counter = 0;
	while (*pszMsg)
	{
		counter++;
		pszMsg++;

		if (counter == maxlen)
			return maxlen;
	}

	return counter;
}

void str_reverse(char* pszMsg)
{
	int start = 0;
	int end = strlen(pszMsg) - 1;

	while (start < end)
	{
		char temp = pszMsg[start];
		pszMsg[start] = pszMsg[end];
		pszMsg[end] = temp;
		start++;
		end--;
	}
}

const char* const digits = "0123456789";

int isdigit(char c)
{
	for (int i = 0; i < 10; i++)
	{
		if (c == digits[i])
			return 1;
	}

	return 0;
}

int strcmp(const char* cs, const char* ct)
{
	unsigned char c1, c2;

	while (1)
	{
		c1 = *cs++;
		c2 = *ct++;
		if (c1 != c2)
			return c1 < c2 ? -1 : 1;
		if (!c1)
			break;
	}
	return 0;
}

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