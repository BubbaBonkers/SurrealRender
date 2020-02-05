#include "d3d11.h"				// Include the DirectX libraries.

#include "NotReallyBasics.h"    // Some basic functions and structs for math and color type things.

using namespace NRB;

#pragma once

class DisplayAgent
{
public:
	ID3D11Device*				Device = nullptr;
	IDXGISwapChain*				SwapChain = nullptr;
	ID3D11DeviceContext*		Context = nullptr;
	ID3D11RenderTargetView*		RenderTargetView = nullptr;
	D3D11_VIEWPORT				Viewport;

	ID3D11Buffer*				VertexBuffer = nullptr;
	ID3D11InputLayout*			InputLayout = nullptr;
	ID3D11VertexShader*			VertexShader = nullptr;		// HLSL
	ID3D11PixelShader*			PixelShader = nullptr;		// HLSL

	int FrameSyncControl = 0;		// Should the refresh rate be locked to the maximum on the device? "0" means no, "1" means yes.

	// Setup render target and present.
	void PresentFromRenderTarget();

	// Release the held references in memory through DirectX.
	void ReleaseInterfaces();
};

