// Include the DirectX libraries.
#include "d3d11.h"

#pragma once

class DisplayAgent
{
public:
	ID3D11Device* Device;
	IDXGISwapChain* SwapChain;
	ID3D11DeviceContext* Context;
	ID3D11RenderTargetView* RenderTargetView;
	D3D11_VIEWPORT Viewport;

	int FrameSyncControl = 0;		// Should the refresh rate be locked to the maximum on the device? "0" means no, "1" means yes.

	// Release the held references in memory through DirectX.
	void ReleaseInterfaces();
};

