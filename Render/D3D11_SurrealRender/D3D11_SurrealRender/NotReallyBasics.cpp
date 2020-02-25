#include "NotReallyBasics.h"
#include "DisplayAgent.h"
#include "DDSTextureLoader.h"

// Colors

// Set the color in this structure to a new color using four floats as the RGBA values.
void NRB::RGBAColor::Color(float red, float green, float blue, float alpha)
{
	Red = red;
	Green = green;
	Blue = blue;
	Alpha = alpha;
}

// Set the color in this structure to a new color using an array of four floats as the RGBA values.
void NRB::RGBAColor::Color(float InputColor[])
{
	Red = InputColor[0];
	Green = InputColor[1];
	Blue = InputColor[2];
	Alpha = InputColor[3];
}

// Return the float values of the color as a float4 array rather than individual values. Returns as RGBA
float* NRB::RGBAColor::GetColorAsArray()
{
	float ColorsArray[4] = { Red, Green, Blue, Alpha };
	return ColorsArray;
}





// Vector2

// Set the Vector coordinates to a new X and Y using two input floats as the new values.
void NRB::Vector2::Set(float x, float y)
{
	X = x;
	Y = y;
}

// Set the Vector coordinates to a new X and Y using one float array with two input floats as the new values.
void NRB::Vector2::Set(float InputVector2[])
{
	X = InputVector2[0];
	Y = InputVector2[1];
}





// Vector3

// Set the Vector coordinates to a new X, Y, and Z using three input floats as the new values.
void NRB::Vector3::Set(float x, float y, float z)
{
	X = x;
	Y = y;
	Z = z;
}

// Set the Vector coordinates to a new X, Y, and Z using one float array with three input floats as the new values.
void NRB::Vector3::Set(float InputVector3[])
{
	X = InputVector3[0];
	Y = InputVector3[1];
	Z = InputVector3[2];
}





// Vector4

// Set the Vector coordinates to a new X, Y, Z, and W using four input floats as the new values.
void NRB::Vector4::Set(float x, float y, float z, float w)
{
	X = x;
	Y = y;
	Z = z;
	W = w;
}

// Set the Vector coordinates to a new X, Y, Z, and W using one float array with four input floats as the new values.
void NRB::Vector4::Set(float InputVector4[])
{
	X = InputVector4[0];
	Y = InputVector4[1];
	Z = InputVector4[2];
	W = InputVector4[3];
}





// Objects

void NRB::Object::BeginPlay(DisplayAgent* Display)
{
	// Load the texture.
	std::string CharPtr = TextureFilepath;
	std::wstring FullPath(CharPtr.length(), L' ');

	// Copy string to wstring.
	std::copy(CharPtr.begin(), CharPtr.end(), FullPath.begin());
	const wchar_t* WChar = FullPath.c_str();

	// Create the texture and set it to the ShaderResourceView for the graphics card.
	CreateDDSTextureFromFile(Display->Device, WChar, nullptr, &ShaderResourceView);

	// Load the Cube object data into the video card.
	D3D11_BUFFER_DESC BufferDescription;
	D3D11_SUBRESOURCE_DATA SubData;
	ZeroMemory(&BufferDescription, sizeof(BufferDescription));
	ZeroMemory(&SubData, sizeof(SubData));

	BufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	BufferDescription.ByteWidth = sizeof(Vertex) * Vertices.size();
	BufferDescription.CPUAccessFlags = 0;
	BufferDescription.MiscFlags = 0;
	BufferDescription.StructureByteStride = 0;
	BufferDescription.Usage = D3D11_USAGE_IMMUTABLE;

	SubData.pSysMem = Vertices.data();

	// Vertex Buffer.
	HRESULT hr = Display->Device->CreateBuffer(&BufferDescription, &SubData, &VertexBuffer);

	// Index Buffer.
	BufferDescription.BindFlags = D3D11_BIND_INDEX_BUFFER;
	BufferDescription.ByteWidth = sizeof(int) * Indices.size();
	SubData.pSysMem = Indices.data();

	hr = Display->Device->CreateBuffer(&BufferDescription, &SubData, &IndexBuffer);
}

XMFLOAT4X4 NRB::Object::AddMovementInput(float x, float y, float z, bool bIgnoreDeltaTime)
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

	// Translate the object in world space.
	XMMATRIX Base = XMLoadFloat4x4(&WorldMatrix);
	XMMATRIX Translated = XMMatrixTranslation((x * Multiplier), (y * Multiplier), (z * Multiplier));
	Translated = XMMatrixMultiply(Base, Translated);

	// Store the new information.
	XMStoreFloat4x4(&WorldMatrix, Translated);

	return WorldMatrix;
}

// Rotate this object in a 3D world. Return the object's new World Matrix after rotation.
XMFLOAT4X4 NRB::Object::AddRotationInput(float Pitch, float Yaw, float Roll, bool bIgnoreDeltaTime)
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

	// Rotate the object in world space.
	XMMATRIX Base = XMLoadFloat4x4(&WorldMatrix);
	XMMATRIX Rotated = XMMatrixRotationRollPitchYaw((Pitch * Multiplier), (Yaw * Multiplier), (Roll * Multiplier));
	Rotated = XMMatrixMultiply(Rotated, Base);

	// Store the new information.
	XMStoreFloat4x4(&WorldMatrix, Rotated);

	return WorldMatrix;
}

// Change the scale of this object.
XMFLOAT4X4 NRB::Object::Scale(float X, float Y, float Z)
{
	XMStoreFloat4x4(&WorldMatrix, XMMatrixMultiply(XMMatrixScaling(X, Y, Z), XMLoadFloat4x4(&WorldMatrix)));

	return WorldMatrix;
}

// Called every frame.
void NRB::Object::Update(float DeltaTime)
{
	TickTime = DeltaTime;
}

// Count how many vertices are in the Vertices array and return it as an int.
int NRB::Object::CountVertices()
{
	return Vertices.size();
}

// Count how many indices are in the Indices array and return it as an int.
int NRB::Object::CountIndices()
{
	return Indices.size();
}

// Create a primitive shape out of vertex and index data, then load it into this object.
void NRB::Object::LoadPrimitive(PRIM_TYPE Type)
{
	switch (Type)
	{
		case LINE:
		{
			
		}

		case GRID:
		{

		}
	}
}

// Create the basic information in this object by using input values. Basic object, no meshes.
void NRB::Object::CreateObject(const char* DebugName, const char* TextureDDS, std::vector<Vertex> VertexData, std::vector<int> IndexData, bool bHide)
{
	// Setup debug information.
	Name = DebugName;
	bIsVisible = bHide;

	// Setup model/mesh information.
	Vertices = VertexData;
	Indices = IndexData;

	// Setup texture information.
	TextureFilepath = TextureDDS;

	// Setup spacial cognition.
	XMStoreFloat4x4(&WorldMatrix, XMMatrixIdentity());
}

// Same as CreateObject base, but this one takes a File Name for the mesh to load into the object.
void NRB::Object::CreateObject(const char* DebugName, const char* FileName, const char* TextureDDS, bool Hide)
{
	// Set basic information about this object.
	Name = DebugName;
	bIsVisible = !Hide;

	// Setup texture information.
	TextureFilepath = TextureDDS;

	// Load mesh data into this object.
	LoadMesh(FileName);

	// Setup spacial cognition.
	XMStoreFloat4x4(&WorldMatrix, XMMatrixIdentity());
}

// Load mesh information such as Texture, Vertices, Indices, and UVs onto this object using a .mesh object file as MeshFileName.
void NRB::Object::LoadMesh(const char* MeshFileName)
{
	// Open the file (mesh) in binary input mode.
	std::fstream file{ MeshFileName, std::ios_base::in | std::ios_base::binary };

	assert(file.is_open());

	// Ensure the file opened correctly.
	if (file.is_open())
	{
		// Setup index count for the mesh.
		uint32_t Item_Index_Count;
		// Read in the nuber of Indices for the mesh.
		file.read((char*)&Item_Index_Count, sizeof(uint32_t));

		// Resize the index list to hold the exact number of indices that the mesh has for easy copying.
		Indices.resize(Item_Index_Count);

		// Read in the indices to the list we made.
		file.read((char*)Indices.data(), sizeof(uint32_t) * Item_Index_Count);

		// Setup the count of vertices in the mesh.
		uint32_t Item_Vertex_Count;
		// Read up to the values of the vertices.
		file.read((char*)&Item_Vertex_Count, sizeof(uint32_t));

		// Resize the Vertex holding list to have easy copying later.
		Vertices.resize(Item_Vertex_Count);

		// Read in the vertex data to our list.
		file.read((char*)Vertices.data(), sizeof(Vertex) * Item_Vertex_Count); // sizeof(Vertex)

		// Example mesh conditioning if needed, to fix up the flip.
		for (auto& v : Vertices)
		{
			v.Position.X = -v.Position.X;
			v.Normal.X = -v.Normal.X;
			v.Texture.Y = 1.0f - v.Texture.Y;
		}

		// Setup a count for triangles.
		int Tri_Count = (int)(Indices.size() / 3);

		// Flip information to be the proper hand system.
		for (int i = 0; i < Tri_Count; ++i)
		{
			int* Tri = Indices.data() + i * 3;

			int Temp = Tri[0];
			Tri[0] = Tri[2];
			Tri[2] = Temp;
		}

		int x = sizeof(Vertex);

		// Close the file.
		file.close();
	}
}

// Called before the window closes.
void NRB::Object::EndPlay()
{
	SAFE_RELEASE(VertexBuffer);			// HLSL
	SAFE_RELEASE(IndexBuffer);
	SAFE_RELEASE(ShaderResourceView);
}





// Cameras

XMFLOAT4X4 NRB::Camera::AddMovementInput(float x, float y, float z, bool bBaseOnWorld)
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
XMFLOAT4X4 NRB::Camera::AddRotationInput(float Pitch, float Yaw, float Roll, bool bBaseOnWorld, bool bIgnoreDeltaTime)
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

void NRB::Camera::LookAtLocation(float x, float y, float z)
{
	// Look at a specified object in the world.
	XMMATRIX LookNew = XMMatrixLookAtLH({ SpacialEnvironment.WorldMatrix._41, SpacialEnvironment.WorldMatrix._42, SpacialEnvironment.WorldMatrix._43 },   { x, y, z }, { 0, 1, 0 });
	XMStoreFloat4x4(&SpacialEnvironment.WorldMatrix, LookNew);
}

// Called every frame.
void NRB::Camera::Update(float DeltaTime)
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
			if (CameraMovementSpeed < 800.0f)
			{
				CameraMovementSpeed += (40.0f * DeltaTime);
			}
		}
		if (GetKeyState('V') & 0x8000)
		{
			if (CameraMovementSpeed > 1.0f)
			{
				CameraMovementSpeed -= (40.0f * DeltaTime);
			}
		}
	}
	else
	{
		// Look at the moving cube..
		if (GetKeyState(VK_TAB) & 0x8000)
		{
			if (LookAtTarget)
			{
				LookAtLocation(LookAtTarget->WorldMatrix._41, LookAtTarget->WorldMatrix._42, LookAtTarget->WorldMatrix._43);
			}
		}

		if (GetKeyState(VK_TAB) & 0x80)
		{
			// Move forward, backward, and side to side.
			if (GetKeyState('W') & 0x8000)
			{
				AddMovementInput(0, 0, (CameraMovementSpeed * DeltaTime), true);
			}
			if (GetKeyState('S') & 0x8000)
			{
				AddMovementInput(0, 0, -(CameraMovementSpeed * DeltaTime), true);
			}
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

	// Feature testing for turn ins. Temporary.
	// Rotate the directional light.
	if (GetKeyState(VK_SHIFT) & 0x8000)
	{
		RotateDirLight = true;
	}
	else
	{
		RotateDirLight = false;
	}

	// Get the starting position for the next frame.
	GetCursorPos(&MousePosition);
}

// Create a camera, give it a debug name, then attach it to an object. Leave AttachTo as "nullptr" to not attach this camera to an object.
void NRB::Camera::CreateCamera(const char* DebugName, Object* AttachTo)
{
	Name = DebugName;
	AttachTarget = AttachTo;
	XMStoreFloat4x4(&SpacialEnvironment.WorldMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&SpacialEnvironment.ViewMatrix, XMMatrixIdentity());
	XMMATRIX Temp = XMMatrixPerspectiveFovLH((XMConvertToRadians(FieldOfViewDeg)), AspectRatio, NearClip, FarClip);
	XMStoreFloat4x4(&SpacialEnvironment.ProjectionMatrix, Temp);
}

// Sets a new FOV and changes the projection matrix.
void NRB::Camera::RefreshCameraFOV(float FOV)
{
	FieldOfViewDeg = FOV;

	XMMATRIX Temp = XMMatrixPerspectiveFovLH((XMConvertToRadians(FieldOfViewDeg)), AspectRatio, NearClip, FarClip);
	XMStoreFloat4x4(&SpacialEnvironment.ProjectionMatrix, Temp);
}

// Sets a new Aspect Ratio and changes the projection matrix.
void NRB::Camera::RefreshCameraAspectRatio(float InRatio)
{
	AspectRatio = InRatio;

	XMMATRIX Temp = XMMatrixPerspectiveFovLH((XMConvertToRadians(FieldOfViewDeg)), AspectRatio, NearClip, FarClip);
	XMStoreFloat4x4(&SpacialEnvironment.ProjectionMatrix, Temp);
}

// Sets a new Aspect Ratio and changes the projection matrix.
void NRB::Camera::ChangeCameraClipping(float MinZ, float MaxZ)
{
	NearClip = MinZ;
	FarClip = MaxZ;
	XMMATRIX Temp = XMMatrixPerspectiveFovLH((XMConvertToRadians(FieldOfViewDeg)), AspectRatio, NearClip, FarClip);
	XMStoreFloat4x4(&SpacialEnvironment.ProjectionMatrix, Temp);
}

// Called before the window closes.
void NRB::Camera::EndPlay()
{
	if (AttachTarget != nullptr)
	{
		AttachTarget = nullptr;
		delete AttachTarget;
	}

	if (LookAtTarget != nullptr)
	{
		LookAtTarget = nullptr;
		delete LookAtTarget;
	}
}