#include <iostream>

#include "Core/GalaxyHook.h"

int main()
{
	GalaxyHook& cheat = GalaxyHook::Get();

	if (cheat.HookD3D())
	{
		while (!GetAsyncKeyState(VK_END) & 1)
		{
			cheat.Update();
		}
	}

	//Cleanup and unload
}