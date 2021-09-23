#include "panic.hpp"
#include "../util/maths.hpp"

void err::panic(const char* pszMsg)
{
	g_tRend.Clear(Colour(0, 0, 0xff));
	g_tRend.SetCursor({ 0, 0 });

	Printf("KERNEL PANIC!\n %s", pszMsg);

	while(true) {}
}