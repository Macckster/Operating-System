#pragma once

//Compute lenth of string;
int strlen(char* pszMsg);
int strnlen(const char* pszMsg, int maxlen);
void str_reverse(char* pszMsg);

int isdigit(char c);
int strcmp(const char* cs, const char* ct);

void itoa(int n, char str[]);