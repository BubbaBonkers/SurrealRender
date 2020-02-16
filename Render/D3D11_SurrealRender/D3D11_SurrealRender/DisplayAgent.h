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
	struct ConstBuffer
	{
		XMMATRIX WorldMatrix;
		XMMATRIX ViewMatrix;
		XMMATRIX ProjectionMatrix;
		XMMATRIX CameraWorldMatrix;
		XMFLOAT4 AmbientLightColor;
		XMFLOAT4 DirectionalLightDirections[1];
		XMFLOAT4 SpotLightDirections[1];
		XMFLOAT4 PointLightPositions[1];
		XMFLOAT4 SpotLightPositions[1];
		XMFLOAT4 DirectionalLightColors[1];
		XMFLOAT4 PointLightColors[1];
		XMFLOAT4 SpotLightColors[1];
		float SpotLightConeRatios;
		float SpotLightIntensities;
		float DirectionalLightIntensities;
		float AmbientLightIntensity;
		float PointLightIntensities;
		float BlinnPhongIntensity;
		float WorldTime;
		float DeltaTime;
		float DiscoIntensity;
	};

	ID3D11Device*				Device = nullptr;
	IDXGISwapChain*				SwapChain = nullptr;
	ID3D11DeviceContext*		Context = nullptr;
	ID3D11RenderTargetView*		RenderTargetView = nullptr;
	D3D11_VIEWPORT				Viewport;

	ID3D11InputLayout*			InputLayout = nullptr;
	ID3D11PixelShader*			PixelShader = nullptr;		// HLSL

	// Texture variables.
	ID3D11SamplerState*			LinearSamplerState = nullptr;
	ID3D11Texture2D*			DiffuseTexture = nullptr;

	ID3D11VertexShader* MeshVertexShader = nullptr;			// HLSL

	ID3D11Buffer* ConstantBuffer = nullptr;					// Stores shaders to send to the video card.

	ID3D11Texture2D* ZBuffer = nullptr;						// Z-Buffer for depth sorting.
	ID3D11DepthStencilView* ZBufferView = nullptr;

	Environment SpacialEnvironment;							// Contains the World, View, and Projection matrices.

	std::vector<Camera*> WorldCameras;
	std::vector<Object*> WorldObjects;
	std::vector<DirectionalLight*> WorldDirectionalLights;
	std::vector<SpotLight*> WorldSpotLights;
	std::vector<PointLight*> WorldPointLights;

	float G_GameTime = 0.0f;								// Global game time, in seconds, that has passed since the game started.

	// Graphics control options. -------------------------------------------------------------------->
	float RenderBackgroundColor[4] = { 0.45f, 0.45f, 0.45f, 0.45f };	// The color of the default background to set the renderer to when clearing DepthStencil and Viewport.
	int FrameSyncControl = 0;											// Should the refresh rate be locked to the maximum on the device? "0" means no, "1" means yes. V-SYNC.
	float FieldOfViewDeg = 90.0f;										// Field of view in degrees.

	// World control options. ----------------------------------------------------------------------->
	XMFLOAT4 AmbientLightCol = { 1, 1, 0, 1 };							// Color of the ambient lighting in the world.
	float AmbientLightIntense = 0.1f;									// Intensity of the ambient light on the environment.

	// Setup render target and present.
	void PresentFromRenderTarget(Camera* Cam, Object* Obj, float DeltaTime = 1.0f);

	// Create a new object.
	Object* CreateObject(const char* DebugName, const char* FileName, const char* TextureDDS, bool bHide = false);
	Object* CreateObject(const char* DebugName, const char* TextureDDS, std::vector<Vertex> VertexData, std::vector<int> IndexData, bool bHide = false);

	// Create a new camera, leave AttachTo as "nullptr" to not attach to an object.
	Camera* CreateCamera(const char* DebugName, Object* AttachTo = nullptr);

	// Create a new directional light.
	DirectionalLight* CreateDirectionalLight(const char* DebugName, XMFLOAT4 direction = { 1, 1, 1, 1 }, XMFLOAT4 color = { 1, 1, 1, 1 }, float intensity = 1.0f);
	SpotLight* CreateSpotLight(const char* DebugName, XMFLOAT4 color = { 1, 1, 1, 1 }, float intensity = 1.0f);
	PointLight* CreatePointLight(const char* DebugName, XMFLOAT4 color = { 1, 1, 1, 1 }, float intensity = 1.0f);

	// Called every frame.
	void Update(float DeltaTime);
	void ChangeAspectRatio(float InRatio);

	// Called before the window closes.
	void EndPlay();

	// Memory managent.
	void ReleaseInterfaces();													// Release the held references in memory through DirectX.
	void ReleasePointerObjects(bool bCameras = true, bool bObjects = true, bool bLights = true);		// Delete all pointers before close.
};

