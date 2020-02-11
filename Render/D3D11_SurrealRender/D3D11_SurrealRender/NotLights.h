#include "DirectXMath.h"

#pragma once

using namespace DirectX;

class NotLights
{
public:
	const char* Name;
	float Intensity = 1.0f;								// Intensity of the light emitted from this Not Light.
	XMFLOAT4 Color = { 1.0f, 1.0f, 1.0f, 1.0f };		// Color of light emitted from this Not Light.
};

class DirectionalLight : NotLights
{
public:
	const char* Name;
	float Intensity = 1.0f;								// Intensity of the light emitted from this Not Light.
	XMFLOAT4 Color = { 1.0f, 1.0f, 1.0f, 1.0f };		// Color of light emitted from this Not Light.
	XMFLOAT4 Direction;									// Direction in 3D space.

	// Initialize this light for use in the game world. Takes in a direction, color, and intensity.
	void CreateLight(const char* DebugName, XMFLOAT4 direction, XMFLOAT4 color, float intensity = 1.0f);
};

class PointLight : NotLights
{
public:
	const char* Name;
	float Intensity = 1.0f;								// Intensity of the light emitted from this Not Light.
	XMFLOAT4 Color = { 1.0f, 1.0f, 1.0f, 1.0f };		// Color of light emitted from this Not Light.
	XMFLOAT4X4 WorldMatrix;								// Location in 3D space.
};

