#include "DirectXMath.h"

#pragma once

using namespace DirectX;

class NotLights
{
public:
	const char* Name = "Debug Light";
	float Intensity = 1.0f;								// Intensity of the light emitted from this Not Light.
	XMFLOAT4 Color = { 1.0f, 1.0f, 1.0f, 1.0f };		// Color of light emitted from this Not Light.

protected:
	float TickTime = 0.0f;								// Time in seconds since the last frame update.

public:
	void Update(float DeltaTime);
};

class DirectionalLight : public NotLights
{
public:
	XMFLOAT4 Direction;									// Direction in 3D space.

	// Initialize this light for use in the game world. Takes in a direction, color, and intensity.
	void CreateLight(const char* DebugName, XMFLOAT4 direction, XMFLOAT4 color, float intensity = 1.0f);

	XMFLOAT4 AddRotationInput(float Pitch, float Yaw, float Roll, bool bIgnoreDeltaTime = false);
};

class SpotLight : public NotLights
{
public:
	const char* Name;
	float Intensity = 1.0f;								// Intensity of the light emitted from this Not Light.
	XMFLOAT4 Color = { 1.0f, 1.0f, 1.0f, 1.0f };		// Color of light emitted from this Not Light.
	XMFLOAT4X4 WorldMatrix;								// Location in 3D space.

	// Directional Light specific stuffs.
	float ConeRatio = 0.8f;							// The angle that the cone opens to.
	float ConeLength = 300.0f;							// The farthest reach of the light emitted from this cone.

	// Initialize this light for use in the game world. Takes in a direction, color, and intensity.
	void CreateLight(const char* DebugName, XMFLOAT4 color, float intensity = 1.0f);

	// Rotate this object in a 3D world. Return the object's new World Matrix after rotation. Set bBaseOnWorld to false if you want the rotation to be based on the current camera rotation rather than the world matrix. Set ignore DeltaTime to true to force the exact rotation and ignore the tick update.
	XMFLOAT4X4 AddRotationInput(float Pitch, float Yaw, float Roll, bool bIgnoreDeltaTime = false);

	// Move this object in a 3D world. Returns the object's new World Matrix after movement. Set BasedInWorld to true if you want the movement to occur relative to the world x, y, and z rather than relative to this camera's current location.
	XMFLOAT4X4 AddMovementInput(float x, float y, float z, bool bIgnoreDeltaTime = false);
};

class PointLight : public NotLights
{
public:
	const char* Name;
	float Intensity = 1.0f;								// Intensity of the light emitted from this Not Light.
	XMFLOAT4 Color = { 1.0f, 1.0f, 1.0f, 1.0f };		// Color of light emitted from this Not Light.
	XMFLOAT4X4 WorldMatrix;								// Location in 3D space.

	// Initialize this light for use in the game world. Takes in a direction, color, and intensity.
	void CreateLight(const char* DebugName, XMFLOAT4 color, float intensity = 1.0f);

	// Rotate this object in a 3D world. Return the object's new World Matrix after rotation. Set bBaseOnWorld to false if you want the rotation to be based on the current camera rotation rather than the world matrix. Set ignore DeltaTime to true to force the exact rotation and ignore the tick update.
	XMFLOAT4X4 AddRotationInput(float Pitch, float Yaw, float Roll, bool bIgnoreDeltaTime = false);

	// Move this object in a 3D world. Returns the object's new World Matrix after movement. Set BasedInWorld to true if you want the movement to occur relative to the world x, y, and z rather than relative to this camera's current location.
	XMFLOAT4X4 AddMovementInput(float x, float y, float z, bool bIgnoreDeltaTime = false);

};

