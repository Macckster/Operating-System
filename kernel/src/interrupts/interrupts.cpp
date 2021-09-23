#include "interrupts.hpp"
#include "../error/panic.hpp"

INTERUPT_HANDLER void PageFaultHandler(interrupt_frame* frame)
{
	err::panic("Page fault detected!");
}
