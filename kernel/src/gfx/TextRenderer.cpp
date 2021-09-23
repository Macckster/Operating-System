#include "TextRenderer.hpp"

TextRenderer::TextRenderer(FrameBuffer_t* _pfBuffer, psf1_font_t* _font) : BasicRenderer(_pfBuffer)
{
	font = _font;
	cursor = { 0, 0 };
	textColour = Colour(0xff, 0xff, 0xff, 0xff);
}

TextRenderer::TextRenderer()
{
	font = NULL;
	cursor = { 0, 0 };
	textColour = Colour(0xff, 0xff, 0xff, 0xff);
}

point TextRenderer::GetCursor()
{
	return cursor;
}

void TextRenderer::SetCursor(const point& p)
{
	cursor = p;
}

void TextRenderer::SetColour(uint Color)
{
	textColour = Color;
}

void TextRenderer::Clear(uint colour)
{
	BasicRenderer::Clear(colour);
	cursor = { 0, 0 };
}

void TextRenderer::Print(const char* pszMsg)
{
	char* str = (char*)pszMsg;

	int len = strlen(str);

	for (int i = 0; i < len; i++)
	{
		if (str[i] == '\n')
		{
			cursor.x = 0;
			cursor.y += 16;
		}
		else
		{
			WriteChar(cursor.x, cursor.y, str[i]);
			cursor.x += 8;

			if (cursor.x >= pfBuffer->width)
			{
				cursor.x = 0;
				cursor.y += 16;
			}
		}
}
	}

void TextRenderer::WriteChar(uint64_t xOff, uint64_t yOff, char c)
{
	uint* pixPtr = (uint*)pfBuffer->baseAddress;
	char* fontPtr = (char*)font->glyphBuffer + (c * font->header->charsize);

	for (unsigned long y = yOff; y < yOff + 16; y++)
	{
		for (unsigned long x = xOff; x < xOff + 8; x++)
		{
			if ((*fontPtr & (0b10000000 >> (x - xOff))) > 0)
				*(uint*)(pixPtr + x + (y * pfBuffer->pixPerScanline)) = textColour;
			else
				*(uint*)(pixPtr + x + (y * pfBuffer->pixPerScanline)) = Colour(0, 0, 0);
		}

		fontPtr++;
	}
}
