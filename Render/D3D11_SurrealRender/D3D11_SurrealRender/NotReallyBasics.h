#include <iostream>
#include <Windows.h>
#include <vector>
#include <fstream>
#include <DirectXMath.h>

#include "d3d11.h"
#include "NotLights.h"

#pragma once

#define SAFE_RELEASE(ptr) { if(ptr) { ptr->Release(); ptr = nullptr; } }

using namespace DirectX;

class DisplayAgent;
namespace NRB
{
	// A color consisting of Red, Green, Blue, and Alpha in format RGBA. Uses arrays and floats.
	struct RGBAColor
	{
	public:
		float Red, Green, Blue, Alpha;

		// Set the color in this structure to a new color using four floats as the RGBA values.
		void Color(float red, float green, float blue, float alpha);

		// Set the color in this structure to a new color using an array of four floats as the RGBA values.
		void Color(float InputColor[]);

		// Return the float values of the color as a float4 array rather than individual values. Returns as RGBA
		float* GetColorAsArray();
	};

	// A vector position (X, Y, Z, and W) in 3D space. Uses arrays and floats.
	struct Vector4
	{
	public:
		float X, Y, Z, W;

		// Set the Vector coordinates to a new X, Y, Z, and W using four input floats as the new values.
		void Set(float x, float y, float z, float w);

		// Set the Vector coordinates to a new X, Y, Z, and W using one float array with four input floats as the new values.
		void Set(float InputVector4[]);
	};

	// A vector position (X, Y, and Z). Uses arrays and floats.
	struct Vector3
	{
	public:
		float X, Y, Z;

		// Set the Vector coordinates to a new X, Y, and Z using three input floats as the new values.
		void Set(float x, float y, float z);

		// Set the Vector coordinates to a new X, Y, and Z using one float array with three input floats as the new values.
		void Set(float InputVector3[]);
	};

	// A vector position (X and Y). Uses arrays and floats.
	struct Vector2
	{
	public:
		float X, Y;

		// Set the Vector coordinates to a new X and Y using two input floats as the new values.
		void Set(float x, float y);

		// Set the Vector coordinates to a new X and Y using one float array with two input floats as the new values.
		void Set(float InputVector2[]);
	};

	// A structure containing botha a vector position (X, Y, Z, and W) in 3D space; and a color consisting of Red, Green, Blue, and Alpha in format RGBA.
	struct Vertex
	{
	public:
		Vector3		Position;				// The position of this vertex in 3D space.
		Vector3		Normal;					// The normal data of this vertex.
		Vector2		Texture;				// Texture data for this vertex.
	};

	// Hold matrix data for the environment including World, View, Projection, and other items.
	struct Environment
	{
		XMFLOAT4X4 WorldMatrix;
		XMFLOAT4X4 ViewMatrix;
		XMFLOAT4X4 ProjectionMatrix;
	};

	// A class containing the most basic information for an object in the 3D scene. One object contains data for Vertices and their Color/Position, the number of vertices in the object, and other data for the object.
	class Object
	{
	public:
		// Spacial cognition for object class.
		XMFLOAT4X4 WorldMatrix;

		const char* Name;					// The name of this object for debugging.
		const char* TextureFilepath;		// The filepath for the DDS texture file to load.
		std::vector<Vertex> Vertices;		// The Position and Color data for each vertex.
		std::vector<int> Indices;			// The Index data for the model.

		ID3D11Buffer* VertexBuffer = nullptr;			// HLSL
		ID3D11Buffer* IndexBuffer = nullptr;

		ID3D11ShaderResourceView* ShaderResourceView = nullptr;		// Stores Loaded DDS Texture.

		// Below is extra functionality in case it is needed in the future.
		bool bIsVisible = true;

	private:
		float TickTime = 0.0f;


	public:
		void BeginPlay(DisplayAgent* Display);

		// ---------- Get information about this object. ----------------------------------------------------------------------------------------->

		// Move this object in a 3D world. Returns the object's new World Matrix after movement.
		XMFLOAT4X4 AddMovementInput(float x, float y, float z, bool bIgnoreDeltaTime = false);

		// Rotate this object in a 3D world. Return the object's new World Matrix after rotation.
		XMFLOAT4X4 AddRotationInput(float Pitch, float Yaw, float Roll, bool bIgnoreDeltaTime = false);


		// ---------- Functionality for this object. --------------------------------------------------------------------------------------------->

		// Called every frame.
		void Update(float DeltaTime);


		// ---------- Get information about this object. ----------------------------------------------------------------------------------------->

		// Count how many vertices are in the Vertices array and return it as an int.
		int CountVertices();

		// Count how many indices are in the Indices array and return it as an int.
		int CountIndices();


		// ---------- Stuff for loading meshes, setting up the Object, and initializing values.--------------------------------------------------->

		// Create the basic information in this object by using input values. Basic object, no meshes.
		void CreateObject(const char* DebugName, const char* TextureDDS, std::vector<Vertex> VertexData, std::vector<int> IndexData, bool bHide = false);

		// Same as CreateObject base, but this one takes a File Name for the mesh to load into the object.
		void CreateObject(const char* DebugName, const char* FileName, const char* TextureDDS, bool Hide = false);

		// Load mesh information such as Texture, Vertices, Indices, and UVs onto this object using a .mesh object file as MeshFileName.
		void LoadMesh(const char* MeshFileName);

		// Called before the window closes.
		void EndPlay();
	};

	class Camera
	{
	public:
		// This camera's World, View, and Projection matrices for world manipulation.
		Environment SpacialEnvironment;

		const char* Name;					// The name of this object for debugging.

		// Where the mouse cursor is at on the screen (X, Y).
		POINT MousePosition;

		// Camera controls and settings.
		float AspectRatio = 1.8667f;										// Aspect Ratio for the view. Set automatically.
		float FieldOfViewDeg = 70.0f;										// Field of view in degrees.
		Object* AttachTarget = nullptr;										// The object this camera is attached to (following). Leave as "nullptr" to disable attachment.

		// Camera physical attributes.
		float CameraMovementSpeed = 8.0f;									// Speed of camera movement in 3D space.
		float CameraRotationSpeed = 8.0f;									// Rate at which the camera rotates when you look around.

	private:
		float TickTime = 0.0f;

		float NearClip = 0.1f;
		float FarClip = 1000.0f;


	public:
		// ---------- Get information about this object. ----------------------------------------------------------------------------------------->

		// Move this object in a 3D world. Returns the object's new World Matrix after movement. Set BasedInWorld to true if you want the movement to occur relative to the world x, y, and z rather than relative to this camera's current location.
		XMFLOAT4X4 AddMovementInput(float x, float y, float z, bool bBaseOnWorld = false)
		{
			XMMATRIX Base;
			XMMATRIX Translated;

			if (bBaseOnWorld)
			{
				// Translate the object in world space.
				XMMATRIX Temp = XMMatrixIdentity();
				Translated = XMMatrixMultiply(Temp, XMMatrixTranslation(x, y, z));
				Base = XMLoadFloat4x4(&SpacialEnvironment.WorldMatrix);
				Translated = XMMatrixMultiply(Translated, Base);
			}
			else
			{
				Base = XMLoadFloat4x4(&SpacialEnvironment.WorldMatrix);
				Translated = XMMatrixTranslation(x, y, z);
				Translated = XMMatrixMultiply(Base, Translated);
			}

			// Store the new information.
			XMStoreFloat4x4(&SpacialEnvironment.WorldMatrix, Translated);

			return SpacialEnvironment.WorldMatrix;
		}

		// Rotate this object in a 3D world. Return the object's new World Matrix after rotation. Set bBaseOnWorld to false if you want the rotation to be based on the current camera rotation rather than the world matrix. Set ignore DeltaTime to true to force the exact rotation and ignore the tick update.
		XMFLOAT4X4 AddRotationInput(float Pitch, float Yaw, float Roll, bool bBaseOnWorld = true, bool bIgnoreDeltaTime = false)
		{
			XMMATRIX Base;
			XMMATRIX Rotated;

			float Multiplier = 1.0f;
			if (bIgnoreDeltaTime)
			{
				Multiplier = 1.0f;
			}
			else
			{
				Multiplier = TickTime;
			}

			if (bBaseOnWorld)
			{
				// Rotate the object in world space.
				XMFLOAT3 TempPos = *(XMFLOAT3*)&SpacialEnvironment.WorldMatrix._41;
				XMMATRIX Temp = (XMMatrixMultiply(XMLoadFloat4x4(&SpacialEnvironment.WorldMatrix), XMMatrixRotationY(-Yaw * Multiplier)));
				XMStoreFloat4x4(&SpacialEnvironment.WorldMatrix, Temp);

				*(XMFLOAT3*)&SpacialEnvironment.WorldMatrix._41 = TempPos;

				Temp = (XMMatrixMultiply(XMMatrixRotationX(-Pitch * Multiplier), XMLoadFloat4x4(&SpacialEnvironment.WorldMatrix)));
				XMStoreFloat4x4(&SpacialEnvironment.WorldMatrix, Temp);
			}
			else
			{
				// Rotate the object in world space.
				Base = XMLoadFloat4x4(&SpacialEnvironment.WorldMatrix);
				Rotated = XMMatrixRotationRollPitchYaw((Pitch * Multiplier), (Yaw * Multiplier), (Roll * Multiplier));
				Rotated = XMMatrixMultiply(Base, Rotated);
			}

			// Store the new information.
			//XMStoreFloat4x4(&SpacialEnvironment.WorldMatrix, Rotated);

			return SpacialEnvironment.WorldMatrix;
		}

		void LookAtLocation(float x, float y, float z)
		{
			// Look at a specified object in the world.
			XMMatrixLookAtLH({ SpacialEnvironment.WorldMatrix._11, SpacialEnvironment.WorldMatrix._12, SpacialEnvironment.WorldMatrix._13 }, { x, y, z }, { 0, 1, 0 });
		}

		// ---------- Functionality for this object. --------------------------------------------------------------------------------------------->

		// Called every frame.
		void Update(float DeltaTime)
		{
			TickTime = DeltaTime;

			//std::cout << DeltaTime << '\n';
			//std::cout << (1.0f / DeltaTime) << '\n' << '\n';

			// Check if there is a valid AttachTarget.
			if (AttachTarget != nullptr)
			{
				// Snap camera to the AttachTarget object.
				SpacialEnvironment.WorldMatrix = AttachTarget->WorldMatrix;
			}

			if (GetKeyState(VK_RBUTTON) & 0x80)
			{
				// Get the starting position of the mouse for this current frame.
				POINT TempMousePosition;
				GetCursorPos(&TempMousePosition);

				// Rotate the camera by the difference in mouse position between the last and current frame.
				if ((MousePosition.x != TempMousePosition.x) || (MousePosition.y != TempMousePosition.y))
				{
					POINT NewPosition = { MousePosition.x - TempMousePosition.x, MousePosition.y - TempMousePosition.y };
					AddRotationInput((NewPosition.y * (CameraRotationSpeed)), (NewPosition.x * (CameraRotationSpeed)), 0, true);
				}

				// Move forward, backward, and side to side.
				if (GetKeyState('W') & 0x8000)
				{
					AddMovementInput(0, 0, (CameraMovementSpeed * DeltaTime), true);
				}
				if (GetKeyState('S') & 0x8000)
				{
					AddMovementInput(0, 0, -(CameraMovementSpeed * DeltaTime), true);
				}
				if (GetKeyState('A') & 0x8000)
				{
					AddMovementInput(-(CameraMovementSpeed * DeltaTime), 0, 0, true);
				}
				if (GetKeyState('D') & 0x8000)
				{
					AddMovementInput((CameraMovementSpeed * DeltaTime), 0, 0, true);
				}

				// Move up or down.
				if (GetKeyState('Q') & 0x8000)
				{
					AddMovementInput(0, (CameraMovementSpeed * DeltaTime), 0, true);
				}
				if (GetKeyState('E') & 0x8000)
				{
					AddMovementInput(0, -(CameraMovementSpeed * DeltaTime), 0, true);
				}

				// Increase and decrease camera movement speed.
				if (GetKeyState('B') & 0x8000)
				{
					if (CameraMovementSpeed < 40.0f)
					{
						CameraMovementSpeed += (17.0f * DeltaTime);
					}
				}
				if (GetKeyState('V') & 0x8000)
				{
					if (CameraMovementSpeed > 1.0f)
					{
						CameraMovementSpeed -= (17.0f * DeltaTime);
					}
				}

				// Field of view controls.
				if (GetKeyState('N') & 0x8000)
				{
					if (FieldOfViewDeg < 100.0f)
					{
						RefreshCameraFOV((FieldOfViewDeg + (12.0f * DeltaTime)));
					}
				}
				if (GetKeyState('M') & 0x8000)
				{
					if (FieldOfViewDeg > 40.0f)
					{
						RefreshCameraFOV((FieldOfViewDeg - (12.0f * DeltaTime)));
					}
				}

				// Camera clipping near plane.
				if (GetKeyState('P') & 0x8000)
				{
					if (NearClip < (FarClip - 10))
					{
						ChangeCameraClipping((NearClip + (5.0f * DeltaTime)), FarClip);
					}
				}
				if (GetKeyState('O') & 0x8000)
				{
					if (NearClip > 0.1f)
					{
						ChangeCameraClipping((NearClip - (5.0f * DeltaTime)), FarClip);
					}
				}

				// Camera clipping far plane.
				if (GetKeyState('I') & 0x8000)
				{
					if (FarClip < 5000.0f)
					{
						ChangeCameraClipping(NearClip, (FarClip + (5.0f * DeltaTime)));
					}
				}
				if (GetKeyState('U') & 0x8000)
				{
					if (FarClip > (NearClip + 10))
					{
						ChangeCameraClipping(NearClip, (FarClip - (5.0f * DeltaTime)));
					}
				}
			}

			// Get the starting position for the next frame.
			GetCursorPos(&MousePosition);
		}



		// ---------- Stuff for loading meshes, setting up the Object, and initializing values.--------------------------------------------------->
		
		// Create a camera, give it a debug name, then attach it to an object. Leave AttachTo as "nullptr" to not attach this camera to an object.
		void CreateCamera(const char* DebugName, Object* AttachTo = nullptr)
		{
			Name = DebugName;
			AttachTarget = AttachTo;
			XMStoreFloat4x4(&SpacialEnvironment.WorldMatrix, XMMatrixIdentity());
			XMStoreFloat4x4(&SpacialEnvironment.ViewMatrix, XMMatrixIdentity());
			XMMATRIX Temp = XMMatrixPerspectiveFovLH((XMConvertToRadians(FieldOfViewDeg)), AspectRatio, NearClip, FarClip);
			XMStoreFloat4x4(&SpacialEnvironment.ProjectionMatrix, Temp);
		}

		// Sets a new FOV and changes the projection matrix.
		void RefreshCameraFOV(float FOV = 90.0f)
		{
			FieldOfViewDeg = FOV;

			XMMATRIX Temp = XMMatrixPerspectiveFovLH((XMConvertToRadians(FieldOfViewDeg)), AspectRatio, NearClip, FarClip);
			XMStoreFloat4x4(&SpacialEnvironment.ProjectionMatrix, Temp);
		}

		// Sets a new Aspect Ratio and changes the projection matrix.
		void RefreshCameraAspectRatio(float InRatio = 1.9f)
		{
			AspectRatio = InRatio;

			XMMATRIX Temp = XMMatrixPerspectiveFovLH((XMConvertToRadians(FieldOfViewDeg)), AspectRatio, NearClip, FarClip);
			XMStoreFloat4x4(&SpacialEnvironment.ProjectionMatrix, Temp);
		}

		// Sets a new Aspect Ratio and changes the projection matrix.
		void ChangeCameraClipping(float MinZ, float MaxZ)
		{
			NearClip = MinZ;
			FarClip = MaxZ;
			XMMATRIX Temp = XMMatrixPerspectiveFovLH((XMConvertToRadians(FieldOfViewDeg)), AspectRatio, NearClip, FarClip);
			XMStoreFloat4x4(&SpacialEnvironment.ProjectionMatrix, Temp);
		}



		// ---------- Memory management. -------------------------------------------------------------------------------------------------------->

		// Called before the window closes.
		void EndPlay()
		{
			if (AttachTarget != nullptr)
			{
				AttachTarget = nullptr;
				delete AttachTarget;
			}
		}
	};
}
