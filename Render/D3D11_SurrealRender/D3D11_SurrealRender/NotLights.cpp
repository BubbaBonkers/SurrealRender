#include "NotLights.h"

// Initialize this light for use in the game world. Takes in a direction, color, and intensity. It also needs the DisplayAgent whose world list should have this light.
void DirectionalLight::CreateLight(const char* DebugName, XMFLOAT4 direction, XMFLOAT4 color, float intensity)
{
	Name = DebugName;
	Direction = direction;
	Color = color;
	Intensity = intensity;
}