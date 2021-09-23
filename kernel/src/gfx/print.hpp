#pragma once

#include "TextRenderer.hpp"
#include "../types.h"

extern TextRenderer g_tRend;

void InitPrint(FrameBuffer_t* buffer, psf1_font_t* font);

void Print(const char* pszMsg);
void PrintLn(const char* pszMsg);
int Printf(const char* fmt, ...);

void SetColour(uint c);