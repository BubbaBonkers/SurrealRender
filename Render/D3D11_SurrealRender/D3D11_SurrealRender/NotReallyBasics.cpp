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
	XMMATRIX Translated = XMMatrixTranslation((XMConvertToRadians(x) * Multiplier), (XMConvertToRadians(y) * Multiplier), (XMConvertToRadians(z) * Multiplier));
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
	Rotated = XMMatrixMultiply(Base, Rotated);

	// Store the new information.
	XMStoreFloat4x4(&WorldMatrix, Rotated);

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