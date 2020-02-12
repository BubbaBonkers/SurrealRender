#include "NotLights.h"

// Called every frame.
void NotLights::Update(float DeltaTime)
{
	TickTime = DeltaTime;
}

// Initialize this light for use in the game world. Takes in a direction, color, and intensity. It also needs the DisplayAgent whose world list should have this light.
void DirectionalLight::CreateLight(const char* DebugName, XMFLOAT4 direction, XMFLOAT4 color, float intensity)
{
	Name = DebugName;
	Direction = direction;
	Color = color;
	Intensity = intensity;
}

/*
// Rotate this object in a 3D world. Return the object's new World Matrix after rotation. Set bBaseOnWorld to false if you want the rotation to be based on the current camera rotation rather than the world matrix. Set ignore DeltaTime to true to force the exact rotation and ignore the tick update.
XMFLOAT4 DirectionalLight::AddRotationInput(float X, float Y, float Z, bool bIgnoreDeltaTime)
{
	float Multiplier = 1.0f;
	if (bIgnoreDeltaTime)
	{
		Multiplier = 1.0f;
	}
	else
	{
		Multiplier = TickTime;
	}

	// Rotate the object in 3D space.
	XMFLOAT4 RotateIt = { (X * Multiplier), (Y * Multiplier), (Z * Multiplier) };
	XMFLOAT4 NewDir = { (Direction.x + RotateIt.x), (Direction.y + RotateIt.y), (Direction.z + RotateIt.z) };
	XMStoreFloat4(&Direction, XMLoadFloat4(&NewDir));

	return Direction;
}
*/

// Initialize this light for use in the game world. Takes in a direction, color, and intensity. It also needs the DisplayAgent whose world list should have this light.
void PointLight::CreateLight(const char* DebugName, XMFLOAT4 color, float intensity)
{
	Name = DebugName;
	Color = color;
	Intensity = intensity;
	XMStoreFloat4x4(&WorldMatrix, XMMatrixIdentity());
}

// Rotate this object in a 3D world. Return the object's new World Matrix after rotation. Set bBaseOnWorld to false if you want the rotation to be based on the current camera rotation rather than the world matrix. Set ignore DeltaTime to true to force the exact rotation and ignore the tick update.
XMFLOAT4X4 PointLight::AddRotationInput(float Pitch, float Yaw, float Roll, bool bIgnoreDeltaTime)
{
	float Multiplier = 1.0f;
	if (bIgnoreDeltaTime)
	{
		Multiplier = 1.0f;
	}
	else
	{
		Multiplier = TickTime;
	}

	// Rotate the object in 3D space.
	XMMATRIX RotateIt = XMMatrixRotationRollPitchYaw((Pitch * Multiplier), (Yaw * Multiplier), (Roll * Multiplier));
	XMStoreFloat4x4(&WorldMatrix, (XMMatrixMultiply(RotateIt, XMLoadFloat4x4(&WorldMatrix))));

	return WorldMatrix;
}

// Move this object in a 3D world. Returns the object's new World Matrix after movement. Set BasedInWorld to true if you want the movement to occur relative to the world x, y, and z rather than relative to this camera's current location.
XMFLOAT4X4 PointLight::AddMovementInput(float x, float y, float z, bool bIgnoreDeltaTime)
{
	float Multiplier = 1.0f;
	if (bIgnoreDeltaTime)
	{
		Multiplier = 1.0f;
	}
	else
	{
		Multiplier = TickTime;
	}

	XMMATRIX Base;
	XMMATRIX Translated;

	Base = XMLoadFloat4x4(&WorldMatrix);
	Translated = XMMatrixTranslation((x * Multiplier), (y * Multiplier), (z * Multiplier));
	Translated = XMMatrixMultiply(Base, Translated);

	// Store the new information.
	XMStoreFloat4x4(&WorldMatrix, Translated);

	return WorldMatrix;
}