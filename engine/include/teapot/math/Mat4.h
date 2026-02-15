#pragma once

#include <cassert>
#include <cmath>
#include <ostream>
#include <teapot/math/Vec.h>

//=============================================================================
// Mat4
//
// 4x4 column-major float matrix.
//
// Stored as float[16] in column-major order:
//   Index layout:
//     [ 0  4  8  12 ]
//     [ 1  5  9  13 ]
//     [ 2  6  10 14 ]
//     [ 3  7  11 15 ]
//
//   Column c, Row r => Data[c * 4 + r]
//
// Factory methods produce common projection and view matrices.
//=============================================================================
struct Mat4
{
	float Data[16] = {};

	// -- Construction -------------------------------------------------------

	constexpr Mat4() = default;

	// -- Element access -----------------------------------------------------

	constexpr float& At(int row, int col)
	{
		assert(row >= 0 && row < 4 && col >= 0 && col < 4);
		return Data[col * 4 + row];
	}

	constexpr const float& At(int row, int col) const
	{
		assert(row >= 0 && row < 4 && col >= 0 && col < 4);
		return Data[col * 4 + row];
	}

	// -- Arithmetic ---------------------------------------------------------

	constexpr Mat4 operator*(const Mat4& other) const
	{
		Mat4 result;
		for (int col = 0; col < 4; ++col)
		{
			for (int row = 0; row < 4; ++row)
			{
				float sum = 0.0f;
				for (int k = 0; k < 4; ++k)
					sum += At(row, k) * other.At(k, col);
				result.At(row, col) = sum;
			}
		}
		return result;
	}

	// -- Static factories ---------------------------------------------------

	static constexpr Mat4 Identity()
	{
		Mat4 m;
		m.Data[0]  = 1.0f;
		m.Data[5]  = 1.0f;
		m.Data[10] = 1.0f;
		m.Data[15] = 1.0f;
		return m;
	}

	/// Orthographic projection.
	/// Maps [left..right] x [bottom..top] x [near..far] to [-1..1] clip space.
	static constexpr Mat4 Orthographic(float left, float right,
	                                   float bottom, float top,
	                                   float near, float far)
	{
		assert(right != left && top != bottom && far != near);
		Mat4 m;
		m.At(0, 0) =  2.0f / (right - left);
		m.At(1, 1) =  2.0f / (top - bottom);
		m.At(2, 2) = -2.0f / (far - near);
		m.At(0, 3) = -(right + left) / (right - left);
		m.At(1, 3) = -(top + bottom) / (top - bottom);
		m.At(2, 3) = -(far + near)   / (far - near);
		m.At(3, 3) = 1.0f;
		return m;
	}

	/// Symmetric perspective projection.
	/// fovY in radians, aspect = width / height.
	static Mat4 Perspective(float fovY, float aspect, float near, float far)
	{
		assert(fovY > 0.0f && aspect > 0.0f && near > 0.0f && far > near);
		float tanHalf = std::tan(fovY * 0.5f);
		Mat4 m;
		m.At(0, 0) = 1.0f / (aspect * tanHalf);
		m.At(1, 1) = 1.0f / tanHalf;
		m.At(2, 2) = -(far + near) / (far - near);
		m.At(2, 3) = -(2.0f * far * near) / (far - near);
		m.At(3, 2) = -1.0f;
		return m;
	}

	/// Translation matrix from a Vec3.
	static constexpr Mat4 Translation(const Vec3& v)
	{
		Mat4 m = Identity();
		m.At(0, 3) = v.X();
		m.At(1, 3) = v.Y();
		m.At(2, 3) = v.Z();
		return m;
	}

	/// Scale matrix from a Vec3.
	static constexpr Mat4 Scaling(const Vec3& v)
	{
		Mat4 m;
		m.At(0, 0) = v.X();
		m.At(1, 1) = v.Y();
		m.At(2, 2) = v.Z();
		m.At(3, 3) = 1.0f;
		return m;
	}

	/// Rotation around the Z axis (radians). Useful for 2D rotation.
	static Mat4 RotationZ(float radians)
	{
		float c = std::cos(radians);
		float s = std::sin(radians);
		Mat4 m = Identity();
		m.At(0, 0) =  c;
		m.At(0, 1) = -s;
		m.At(1, 0) =  s;
		m.At(1, 1) =  c;
		return m;
	}

	/// Rotation around the X axis (radians).
	static Mat4 RotationX(float radians)
	{
		float c = std::cos(radians);
		float s = std::sin(radians);
		Mat4 m = Identity();
		m.At(1, 1) =  c;
		m.At(1, 2) = -s;
		m.At(2, 1) =  s;
		m.At(2, 2) =  c;
		return m;
	}

	/// Rotation around the Y axis (radians).
	static Mat4 RotationY(float radians)
	{
		float c = std::cos(radians);
		float s = std::sin(radians);
		Mat4 m = Identity();
		m.At(0, 0) =  c;
		m.At(0, 2) =  s;
		m.At(2, 0) = -s;
		m.At(2, 2) =  c;
		return m;
	}
};

// -- Stream output ----------------------------------------------------------

inline std::ostream& operator<<(std::ostream& os, const Mat4& m)
{
	for (int row = 0; row < 4; ++row)
	{
		os << "[ ";
		for (int col = 0; col < 4; ++col)
		{
			if (col > 0) os << ", ";
			os << m.At(row, col);
		}
		os << " ]\n";
	}
	return os;
}