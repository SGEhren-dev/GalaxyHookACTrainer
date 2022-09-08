#include <iostream>
#include "GalaxyHook.h"


//Needed things we haven't put into the class storage
#define MAINVP 0
D3D11_VIEWPORT pViewports[D3D10_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE]{ 0 };
XMMATRIX mOrtho;

//More needed definitions for D3D that have not been added
HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapchain, UINT SyncInterval, UINT Flags);
using fnPresent = HRESULT(__stdcall*)(IDXGISwapChain* pThis, UINT SyncInterval, UINT Flags);
void* ogPresent;
fnPresent ogPresentTramp;
void* pTrampoline = nullptr;
char ogBytes[PRESENT_STUB_SIZE];

GalaxyHook::GalaxyHook()
{
	this->Options = (options_t*)malloc(sizeof(options_t));
	this->Options = new options_t();

	this->D3DSettings = (d3dsettings_t*)malloc(sizeof(d3dsettings_t));
	this->D3DSettings = new d3dsettings_t();

	this->Options->procId = GetProcId(L"ac_client.exe");
	this->Options->baseAddr = 0x400000;
	this->Options->hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, Options->procId);

	this->Options->dynamicPtrBaseAddress = Options->baseAddr + 0x10F4F4;

	this->Addresses = (addy_t*)malloc(sizeof(addy_t));
	this->Addresses = new addy_t(this->Options);

	DrawMenu();
}

GalaxyHook::~GalaxyHook()
{
	safe_delete(this->Options);
	safe_delete(this->Addresses);
	safe_delete(this->D3DSettings);
}

void GalaxyHook::Update()
{
	if (GetAsyncKeyState(VK_F1) & 1)
	{
		Options->Toggles[MAX_AMMO] = !Options->Toggles[MAX_AMMO];
		DrawMenu();
		Sleep(100);
	}

	if (GetAsyncKeyState(VK_F2) & 1)
	{
		Options->Toggles[GOD_MODE] = !Options->Toggles[GOD_MODE];
		DrawMenu();
		Sleep(1000);
	}



	if(Options->Toggles[MAX_AMMO])
	{ 
		int maxAmmo = 1337;
		WriteProcessMemory(Options->hProcess, (BYTE*)Addresses->PrimaryAmmoAddr, &maxAmmo, sizeof(maxAmmo), nullptr);
		WriteProcessMemory(Options->hProcess, (BYTE*)Addresses->PrimaryReserveAddr, &maxAmmo, sizeof(maxAmmo), nullptr);
		WriteProcessMemory(Options->hProcess, (BYTE*)Addresses->SecondaryAmmoAddr, &maxAmmo, sizeof(maxAmmo), nullptr);
	}

	if (Options->Toggles[GOD_MODE])
	{
		int maxHealth = 99999;
		int maxArmor = 99999;
		WriteProcessMemory(Options->hProcess, (BYTE*)Addresses->ArmorAddr, &maxArmor, sizeof(maxArmor), nullptr);
		WriteProcessMemory(Options->hProcess, (BYTE*)Addresses->HealthAddr, &maxHealth, sizeof(maxHealth), nullptr);
	}
}

void GalaxyHook::DrawMenu()
{
	system("CLS");

	std::cout << "Dynamic Base Ptr Address: " << "0x" << std::hex << Options->dynamicPtrBaseAddress << std::endl << std::endl;
	(Options->Toggles[GOD_MODE]) ? std::cout << "God Mode [ON]" << std::endl : std::cout << "God Mode [OFF]" << std::endl;
	(Options->Toggles[MAX_AMMO]) ? std::cout << "Max Ammo [ON]" << std::endl : std::cout << "Max Ammo [OFF]" << std::endl;
}

bool GalaxyHook::HookD3D()
{
	return TRUE;
}

BOOL GalaxyHook::IsInGame()
{
	return TRUE;
}
