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
	ID3D11PixelShader*			PixelShader = nullptr;		// HLSL

	// Texture variables.
	ID3D11ShaderResourceView*	ShaderResourceView = nullptr;
	ID3D11SamplerState*			LinearSamplerState = nullptr;
	ID3D11Texture2D*			DiffuseTexture = nullptr;

	ID3D11VertexShader* MeshVertexShader = nullptr;			// HLSL

	ID3D11Buffer* ConstantBuffer = nullptr;					// Stores shaders to send to the video card.
	ID3D11Buffer* MeshVertexBuffer = nullptr;
	ID3D11Buffer* MeshIndexBuffer = nullptr;

	ID3D11Texture2D* ZBuffer = nullptr;								// Z-Buffer for depth sorting.
	ID3D11DepthStencilView* ZBufferView = nullptr;

	Environment SpacialEnvironment;							// Contains the World, View, and Projection matrices.

	std::vector<Camera*> WorldCameras;
	std::vector<Object*> WorldObjects;

	// Graphics control options. -------------------------------------------------------------------->
	float RenderBackgroundColor[4] = { 0.45f, 0.45f, 0.45f, 0.45f };	// The color of the default background to set the renderer to when clearing DepthStencil and Viewport.
	int FrameSyncControl = 1;											// Should the refresh rate be locked to the maximum on the device? "0" means no, "1" means yes. V-SYNC.
	float AspectRatio = 1.8667f;										// Aspect Ratio for the view. Set automatically.
	float FieldOfViewDeg = 90.0f;										// Field of view in degrees.

	// Setup render target and present.
	void PresentFromRenderTarget(Camera* Cam, Object* Obj, float DeltaTime = 1.0f);

	// Create a new object.
	Object* CreateObject(const char* DebugName, const char* FileName, bool bHide = false);
	Object* CreateObject(const char* DebugName, std::vector<Vertex> VertexData, std::vector<int> IndexData, bool bHide = false);

	// Create a new camera, leave AttachTo as "nullptr" to not attach to an object.
	Camera* CreateCamera(const char* DebugName, Object* AttachTo = nullptr);

	// Called every frame.
	void Update(float DeltaTime);

	// Called before the window closes.
	void EndPlay();

	// Memory managent.
	void ReleaseInterfaces();													// Release the held references in memory through DirectX.
	void ReleasePointerObjects(bool bCameras = true, bool bObjects = true);		// Delete all pointers before close.
};

