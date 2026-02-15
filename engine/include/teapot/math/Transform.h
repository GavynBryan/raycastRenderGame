#pragma once

#include <teapot/math/Vec.h>
#include <type_traits>

//=============================================================================
// Transform<N>
//
// N-dimensional transform with float components.
//
// Bundles position, scale, and rotation into a single value type.
//
// Rotation type varies by dimension:
//   - 2D: float (a single angle in radians)
//   - 3D+: Vec<N> (e.g. Euler angles)
//
// Common aliases:
//   Transform2D, Transform3D
//=============================================================================
template <int N>
struct Transform
{
	static_assert(N >= 2, "Transform dimension must be at least 2.");

	using RotationType = std::conditional_t<N == 2, float, Vec<N>>;

	Vec<N> Position{};
	Vec<N> Scale = Vec<N>::One();
	RotationType Rotation{};
};

// -- Common aliases ---------------------------------------------------------

using Transform2D = Transform<2>;
using Transform3D = Transform<3>;