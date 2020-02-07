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

	// A structure containing botha a vector position (X, Y, Z, and W) in 3D space; and a color consisting of Red, Green, Blue, and Alpha in format RGBA.
	struct Vertex
	{
	public:
		Vector4		Position;				// The position of this vertex in 3D space.
		RGBAColor	Color;					// The color of this vertext position.
	};

	// A structure containing the most basic information for an object in the 3D scene. One object contains data for Vertices and their Color/Position, the number of vertices in the object, and other data for the object.
	struct Object
	{
		char* Name;							// The name of this object for debugging.
		Vertex Vertices[1];					// The Position and Color data for each vertex.

		// Count how many vertices are in the Vertices array and return it as an int.
		int CountVertices()
		{
			return (sizeof(Vertices) / sizeof(Vertices[0]));
		}
	};
}
