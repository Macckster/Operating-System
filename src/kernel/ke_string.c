#include "ke_string.h"
#include "../drivers/screen.h"

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
