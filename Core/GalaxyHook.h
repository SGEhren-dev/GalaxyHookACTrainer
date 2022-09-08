#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include "Deps/D3D_VMT_Indices.h"
#include "Deps/Shaders.h"
#include "Deps/Base.h"

#include "../proc/proc.h"

#define MAX_AMMO 0
#define GOD_MODE 1

#define MENU 49

#define VMT_PRESENT (UINT)IDXGISwapChainVMT::Present
#define PRESENT_STUB_SIZE 5

//D3D Related object pointers
using namespace DirectX;

typedef struct D3DSettings
{
public:
	ID3D11Device* pDevice = nullptr;
	IDXGISwapChain* pSwapchain = nullptr;
	ID3D11DeviceContext* pContext = nullptr;
	ID3D11RenderTargetView* pRenderTargetView = nullptr;
	ID3D11VertexShader* pVertexShader = nullptr;
	ID3D11InputLayout* pVertexLayout = nullptr;
	ID3D11PixelShader* pPixelShader = nullptr;
	ID3D11Buffer* pVertexBuffer = nullptr;
	ID3D11Buffer* pIndexBuffer = nullptr;
	ID3D11Buffer* pConstantBuffer = nullptr;
} d3dsettings_t;

typedef struct Options
{
	Options() : hProcess(0), procId(0), baseAddr(0), dynamicPtrBaseAddress(0) { memset(Toggles, FALSE, sizeof(Toggles)); }

	bool Toggles[50];

	HANDLE hProcess;
	DWORD procId;
	HWND hWnd;								//Needed for D3D hooking
	uintptr_t baseAddr;
	uintptr_t dynamicPtrBaseAddress;

} options_t;

typedef struct Addresses
{
private:
	std::vector<unsigned int> primaryAmmoOffset = { 0x374, 0x14, 0x0 };
	std::vector<unsigned int> primaryReserveOffset = { 0x374, 0x08, 0x128 };
	std::vector<unsigned int> secondaryAmmoOfset = { 0x374, 0x08, 0x13C };

	std::vector<unsigned int> healthOffsets = { 0xF8 };
	std::vector<unsigned int> armorOffsets = { 0x374, 0x08, 0xFC };

	std::vector<unsigned int> playerX = { 0x374, 0x08, 0x04 };
	std::vector<unsigned int> playerY = { 0x374, 0x08, 0x08 };
	std::vector<unsigned int> playerZ = { 0x374, 0x08, 0x0C };

public:

	Addresses(options_t* opt) 
	{
		PrimaryAmmoAddr = FindDMAAddy(opt->hProcess, opt->dynamicPtrBaseAddress, primaryAmmoOffset);
		PrimaryReserveAddr = FindDMAAddy(opt->hProcess, opt->dynamicPtrBaseAddress, primaryReserveOffset);
		HealthAddr = FindDMAAddy(opt->hProcess, opt->dynamicPtrBaseAddress, healthOffsets);
		ArmorAddr = FindDMAAddy(opt->hProcess, opt->dynamicPtrBaseAddress, armorOffsets);
		SecondaryAmmoAddr = FindDMAAddy(opt->hProcess, opt->dynamicPtrBaseAddress, secondaryAmmoOfset);
		SecondaryReserveAddr = 0;
	}

	uintptr_t PrimaryAmmoAddr;
	uintptr_t PrimaryReserveAddr;
	uintptr_t HealthAddr;
	uintptr_t ArmorAddr;
	uintptr_t SecondaryAmmoAddr;
	uintptr_t SecondaryReserveAddr;

} addy_t;

struct ConstantBuffer
{
	XMMATRIX mProjection;
};

struct Vertex
{
	XMFLOAT3 pos;
	XMFLOAT4 color;
};


class GalaxyHook
{
private:
	options_t* Options;
	addy_t* Addresses;
	d3dsettings_t* D3DSettings;
	GalaxyHook() {}

public:
	GalaxyHook(const GalaxyHook&) = delete;
	void operator=(GalaxyHook const&) = delete;

	~GalaxyHook();

	static GalaxyHook& Instance() {
		static GalaxyHook _singleton;
		return _singleton;
	}

	void Update();
	void DrawMenu();

	//DirectX hooking stuff
	bool Hook(void* pSrc, void* pDst, size_t size);
	bool HookD3D();
	bool CompileShader(const char* szShader, const char* szEntryPoint, const char* szTarget, ID3D10Blob ** pBlob);
	bool InitD3DHook(IDXGISwapChain* pSwapchain);
	void CleanupD3D();
	void Render();

	BOOL IsInGame();
};

