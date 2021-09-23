#pragma once

#define INTERUPT_HANDLER __attribute((interrupt))

struct interrupt_frame;
INTERUPT_HANDLER void PageFaultHandler(struct interrupt_frame* frame);