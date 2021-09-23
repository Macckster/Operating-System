#pragma once
#include "BasicRenderer.hpp"
#include "../bootloader_types.h"
#include "../util/maths.hpp"
#include "../string.hpp"

class TextRenderer : public BasicRenderer
{
public:
	TextRenderer(FrameBuffer_t* _pfBuffer, psf1_font_t* _font);
	TextRenderer();

	void Print(const char* pszMsg);
	void SetCursor(const point& p);
	point GetCursor();
	void SetColour(uint Color);

private:
	psf1_font_t* font;
	uint textColour;
	point cursor;

	void WriteChar(uint64_t xOff, uint64_t yOff, char c);
};