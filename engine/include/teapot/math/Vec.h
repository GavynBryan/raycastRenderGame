#pragma once

#include <cassert>
#include <cmath>
#include <ostream>

//=============================================================================
// Vec<N>
//
// N-dimensional vector with float components.
// Dimension-agnostic — works for 2D, 3D, 4D, or any positive dimension.
//
// Named accessors X(), Y(), Z(), W() are available when the dimension
// count supports them (enforced at compile time via requires clauses).
//
// Cross() is restricted to 3-dimensional vectors at compile time.
//
// Common aliases:
//   Vec2, Vec3, Vec4
//=============================================================================
template <int N>
struct Vec
{
	static_assert(N > 0, "Vec dimension must be at least 1.");

	// -- Data ---------------------------------------------------------------

	static constexpr int Dimensions = N;
	float Data[N] = {};

	// -- Construction -------------------------------------------------------

	constexpr Vec() = default;

	template <typename... Args>
		requires (sizeof...(Args) == N && (std::convertible_to<Args, float> && ...))
	constexpr Vec(Args... args) : Data{ static_cast<float>(args)... } {}

	// -- Named accessors (dimension-gated) ----------------------------------

	constexpr float& X() { return Data[0]; }
	constexpr const float& X() const { return Data[0]; }

	constexpr float& Y() requires (N >= 2) { return Data[1]; }
	constexpr const float& Y() const requires (N >= 2) { return Data[1]; }

	constexpr float& Z() requires (N >= 3) { return Data[2]; }
	constexpr const float& Z() const requires (N >= 3) { return Data[2]; }

	constexpr float& W() requires (N >= 4) { return Data[3]; }
	constexpr const float& W() const requires (N >= 4) { return Data[3]; }

	// -- Element access -----------------------------------------------------

	constexpr float& operator[](int index)
	{
		assert(index >= 0 && index < N && "Vec index out of range.");
		return Data[index];
	}

	constexpr const float& operator[](int index) const
	{
		assert(index >= 0 && index < N && "Vec index out of range.");
		return Data[index];
	}

	// -- Arithmetic operators -----------------------------------------------

	constexpr Vec operator+(const Vec& other) const
	{
		Vec result;
		for (int i = 0; i < N; ++i)
			result.Data[i] = Data[i] + other.Data[i];
		return result;
	}

	constexpr Vec operator-(const Vec& other) const
	{
		Vec result;
		for (int i = 0; i < N; ++i)
			result.Data[i] = Data[i] - other.Data[i];
		return result;
	}

	constexpr Vec operator*(float scalar) const
	{
		Vec result;
		for (int i = 0; i < N; ++i)
			result.Data[i] = Data[i] * scalar;
		return result;
	}

	constexpr Vec operator/(float scalar) const
	{
		assert(scalar != 0.0f && "Vec division by zero.");
		Vec result;
		for (int i = 0; i < N; ++i)
			result.Data[i] = Data[i] / scalar;
		return result;
	}

	constexpr Vec& operator+=(const Vec& other)
	{
		for (int i = 0; i < N; ++i)
			Data[i] += other.Data[i];
		return *this;
	}

	constexpr Vec& operator-=(const Vec& other)
	{
		for (int i = 0; i < N; ++i)
			Data[i] -= other.Data[i];
		return *this;
	}

	constexpr Vec& operator*=(float scalar)
	{
		for (int i = 0; i < N; ++i)
			Data[i] *= scalar;
		return *this;
	}

	constexpr Vec& operator/=(float scalar)
	{
		assert(scalar != 0.0f && "Vec division by zero.");
		for (int i = 0; i < N; ++i)
			Data[i] /= scalar;
		return *this;
	}

	constexpr Vec operator-() const
	{
		Vec result;
		for (int i = 0; i < N; ++i)
			result.Data[i] = -Data[i];
		return result;
	}

	// -- Comparison ---------------------------------------------------------

	constexpr bool operator==(const Vec& other) const
	{
		for (int i = 0; i < N; ++i)
		{
			if (Data[i] != other.Data[i]) return false;
		}
		return true;
	}

	// -- Vector operations --------------------------------------------------

	constexpr float Dot(const Vec& other) const
	{
		float sum = 0.0f;
		for (int i = 0; i < N; ++i)
			sum += Data[i] * other.Data[i];
		return sum;
	}

	constexpr float SqrMagnitude() const
	{
		return Dot(*this);
	}

	float Magnitude() const
	{
		return std::sqrt(SqrMagnitude());
	}

	Vec Normalized() const
	{
		float mag = Magnitude();
		assert(mag > 0.0f && "Cannot normalize a zero-length Vec.");
		return *this / mag;
	}

	// -- 3D-only operations -------------------------------------------------

	constexpr Vec Cross(const Vec& other) const requires (N == 3)
	{
		return Vec{
			Data[1] * other.Data[2] - Data[2] * other.Data[1],
			Data[2] * other.Data[0] - Data[0] * other.Data[2],
			Data[0] * other.Data[1] - Data[1] * other.Data[0]
		};
	}

	// -- Static factories ---------------------------------------------------

	static constexpr Vec Zero()
	{
		return Vec{};
	}

	static constexpr Vec One()
	{
		Vec result;
		for (int i = 0; i < N; ++i)
			result.Data[i] = 1.0f;
		return result;
	}

	// -- Static utilities ---------------------------------------------------

	static constexpr Vec Lerp(const Vec& a, const Vec& b, float t)
	{
		Vec result;
		for (int i = 0; i < N; ++i)
			result.Data[i] = a.Data[i] + t * (b.Data[i] - a.Data[i]);
		return result;
	}

	static float Distance(const Vec& a, const Vec& b)
	{
		return (a - b).Magnitude();
	}

	static constexpr float SqrDistance(const Vec& a, const Vec& b)
	{
		return (a - b).SqrMagnitude();
	}
};

// -- Free function: scalar * vec --------------------------------------------

template <int N>
constexpr Vec<N> operator*(float scalar, const Vec<N>& v)
{
	return v * scalar;
}

// -- Stream output ----------------------------------------------------------

template <int N>
std::ostream& operator<<(std::ostream& os, const Vec<N>& v)
{
	os << "(";
	for (int i = 0; i < N; ++i)
	{
		if (i > 0) os << ", ";
		os << v.Data[i];
	}
	os << ")";
	return os;
}

// -- Common aliases ---------------------------------------------------------

using Vec2 = Vec<2>;
using Vec3 = Vec<3>;
using Vec4 = Vec<4>;