#include <vector>
#include <fstream>
#include <Windows.h>

#pragma once

namespace NRB
{
	// A color consisting of Red, Green, Blue, and Alpha in format RGBA. Uses arrays and floats.
	struct RGBAColor
	{
	public:
		float Red, Green, Blue, Alpha;

		// Set the color in this structure to a new color using four floats as the RGBA values.
		void Color(float red, float green, float blue, float alpha)
		{
			Red = red;
			Green = green;
			Blue = blue;
			Alpha = alpha;
		}

		// Set the color in this structure to a new color using an array of four floats as the RGBA values.
		void Color(float InputColor[])
		{
			Red = InputColor[0];
			Green = InputColor[1];
			Blue = InputColor[2];
			Alpha = InputColor[3];
		}

		// Return the float values of the color as a float4 array rather than individual values. Returns as RGBA
		float* GetColorAsArray()
		{
			float ColorsArray[4] = { Red, Green, Blue, Alpha };
			return ColorsArray;
		}
	};

	// A vector position (X, Y, Z, and W) in 3D space. Uses arrays and floats.
	struct Vector4
	{
	public:
		float X, Y, Z, W;

		// Set the Vector coordinates to a new X, Y, Z, and W using four input floats as the new values.
		void Set(float x, float y, float z, float w)
		{
			X = x;
			Y = y;
			Z = z;
			W = w;
		}

		// Set the Vector coordinates to a new X, Y, Z, and W using one float array with four input floats as the new values.
		void Set(float InputVector4[])
		{
			X = InputVector4[0];
			Y = InputVector4[1];
			Z = InputVector4[2];
			W = InputVector4[3];
		}
	};

	// A vector position (X, Y, and Z). Uses arrays and floats.
	struct Vector3
	{
	public:
		float X, Y, Z;

		// Set the Vector coordinates to a new X, Y, and Z using three input floats as the new values.
		void Set(float x, float y, float z)
		{
			X = x;
			Y = y;
			Z = z;
		}

		// Set the Vector coordinates to a new X, Y, and Z using one float array with three input floats as the new values.
		void Set(float InputVector3[])
		{
			X = InputVector3[0];
			Y = InputVector3[1];
			Z = InputVector3[2];
		}
	};

	// A vector position (X and Y). Uses arrays and floats.
	struct Vector2
	{
	public:
		float X, Y;

		// Set the Vector coordinates to a new X and Y using two input floats as the new values.
		void Set(float x, float y)
		{
			X = x;
			Y = y;
		}

		// Set the Vector coordinates to a new X and Y using one float array with two input floats as the new values.
		void Set(float InputVector2[])
		{
			X = InputVector2[0];
			Y = InputVector2[1];
		}
	};

	// A structure containing botha a vector position (X, Y, Z, and W) in 3D space; and a color consisting of Red, Green, Blue, and Alpha in format RGBA.
	struct Vertex
	{
	public:
		Vector4		Position;				// The position of this vertex in 3D space.
		RGBAColor	Color;					// The color of this vertext position.
		Vector3		Normal;					// The normal data of this vertex.
		Vector2		Texture;				// Texture data for this vertex.
	};

	// A structure containing the most basic information for an object in the 3D scene. One object contains data for Vertices and their Color/Position, the number of vertices in the object, and other data for the object.
	struct Object
	{
	public:
		const char* Name;					// The name of this object for debugging.
		std::vector<Vertex> Vertices;		// The Position and Color data for each vertex.
		std::vector<int> Indices;			// The Index data for the model.

		// Below is extra functionality in case it is needed in the future.
		bool bIsVisible = true;

		// Create the basic information in this object by using input values. Basic object, no meshes.
		void CreateObject(const char* DebugName, std::vector<Vertex> VertexData, std::vector<int> IndexData, bool bHide = false)
		{
			Name = DebugName;
			Vertices = VertexData;
			Indices = IndexData;
			bIsVisible = bHide;
		}

		// Same as CreateObject base, but this one takes a File Name for the mesh to load into the object.
		void CreateObject(const char* DebugName, const char* FileName, bool Hide = false)
		{
			// Set basic information about this object.
			Name = DebugName;
			bIsVisible = Hide;

			// Load mesh data into this object.
			LoadMesh(FileName);
		}

		// Count how many vertices are in the Vertices array and return it as an int.
		int CountVertices()
		{
			return Vertices.size();
		}

		// Count how many indices are in the Indices array and return it as an int.
		int CountIndices()
		{
			return Indices.size();
		}

		// Load mesh information such as Texture, Vertices, Indices, and UVs onto this object using a .mesh object file as MeshFileName.
		void LoadMesh(const char* MeshFileName)
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
				file.read((char*)Vertices.data(), sizeof(Vertex) * Item_Vertex_Count);

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

				// Close the file.
				file.close();
			}
		}
	};
}
