#pragma once

#include "../types.h"
#include "../bootloader_types.h"

class BasicRenderer
{

public:
	BasicRenderer(FrameBuffer_t* _pfBuffer);
	BasicRenderer();

	void Clear(uint colour);

	FrameBuffer_t* pfBuffer;
	uint32_t BBP = 4;

private:
};