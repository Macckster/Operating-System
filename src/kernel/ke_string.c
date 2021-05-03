#include "ke_string.h"
#include "../drivers/screen.h"

//Compute length of string
int ke_strlen(char* pszMsg)
{
	int counter = 0;
	while (*pszMsg)
	{
		counter++;
		pszMsg++;
	}

	return counter;
}