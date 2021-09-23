#include "BasicRenderer.hpp"

BasicRenderer::BasicRenderer(FrameBuffer_t* _pfBuffer) 
{
	pfBuffer = _pfBuffer;
}

BasicRenderer::BasicRenderer()
{
	pfBuffer = NULL;
}

void BasicRenderer::Clear(uint colour)
{
    uint64_t fbBase = (uint64_t)pfBuffer->baseAddress;
    uint64_t bytesPerScanline = pfBuffer->pixPerScanline * 4;
    uint64_t fbHeight = pfBuffer->height;
    uint64_t fbSize = pfBuffer->bufferSize;

    for (int verticalScanline = 0; verticalScanline < fbHeight; verticalScanline++)
    {
        uint64_t pixPtrBase = fbBase + (bytesPerScanline * verticalScanline);
        for (uint32_t* pixPtr = (uint32_t*)pixPtrBase; pixPtr < (uint32_t*)(pixPtrBase + bytesPerScanline); pixPtr++)
        {
            *pixPtr = colour;
        }
    }
}