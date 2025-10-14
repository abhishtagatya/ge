#pragma once

#include <cmath>
#include <iostream>

namespace gem
{
	template<typename T, size_t N>
	struct Vector; // Forward declaration

	template<typename T, size_t N>
	Vector<T, N> lerp(Vector<T, N> a, Vector<T, N> b, float t)
	{
		return a + (b - a) * t;
	};

	template<typename T>
	struct Quaternion; // Forward declaration
	
	template<typename T>
	Quaternion<T> lerp(Quaternion<T> a, Quaternion<T> b, float t)
	{
		return (a + (b - a) * t).normalize();
	};

	template<typename T>
	Quaternion<T> slerp(Quaternion<T> a, Quaternion<T> b, float t)
	{
		// Clamping Edges
		if (t == 0.0f) return a;
		if (t == 1.0f) return b;

		T dot = a.dot(b);
		if (dot < 0.0f)
		{
			b = -b;
			dot = -dot;
		}

		const T DOT_THRESHOLD = static_cast<T>(0.9995);
		if (dot > DOT_THRESHOLD)
		{
			// If the quaternions are very close, use lerp
			return lerp(a, b, t).normalize();
		}

		T theta_0 = std::acos(dot);
		T theta = theta_0 * t;

		T sin_theta = std::sin(theta);
		T sin_theta_0 = std::sin(theta_0);
		T s0 = std::cos(theta) - dot * sin_theta / sin_theta_0;
		T s1 = sin_theta / sin_theta_0;

		return (a * s0 + b * s1).normalize();
	};
}