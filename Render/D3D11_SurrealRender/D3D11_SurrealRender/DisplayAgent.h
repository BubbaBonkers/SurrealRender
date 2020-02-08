#include "d3d11.h"				// Include the DirectX libraries.
#include <DirectXMath.h>
#include <vector>

#include "NotReallyBasics.h"    // Some basic functions and structs for math and color type things.

using namespace NRB;
using namespace DirectX;

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

	ID3D11VertexShader* MeshVertexShader = nullptr;			// HLSL

	ID3D11Buffer* ConstantBuffer = nullptr;					// Stores shaders to send to the video card.
	ID3D11Buffer* MeshVertexBuffer = nullptr;
	ID3D11Buffer* MeshIndexBuffer = nullptr;

	Environment SpacialEnvironment;							// Contains the World, View, and Projection matrices.

	std::vector<Object> WorldObjects;

	// Graphics control options. -------------------------------------------------------------------->
	int FrameSyncControl = 0;		// Should the refresh rate be locked to the maximum on the device? "0" means no, "1" means yes. V-SYNC.
	float AspectRatio = 1.0f;		// Aspect Ratio for the view. Set automatically.
	float FieldOfViewDeg = 90.0f;	// Field of view in degrees.

	int VertexCount = 0;

	// Setup render target and present.
	void PresentFromRenderTarget(Object Obj);

	// Release the held references in memory through DirectX.
	void ReleaseInterfaces();
};

