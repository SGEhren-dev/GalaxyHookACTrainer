#include <iostream>

#include "Core/GalaxyHook.h"

// Player coords { 0x374, 0x08, 0x04 | 0x08 | 0x0c }
// Primary reserve { 0x374, 0x08, 0x128 }
// player armor { 0x374, 0x08, 0xFC }
// 

int main()
{
	GalaxyHook& cheat = GalaxyHook::Instance();

	if (cheat.HookD3D())
	{
		while (!GetAsyncKeyState(VK_END) & 1)
		{
			cheat.Update();
		}
	}

	//Cleanup and unload
}