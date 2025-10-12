#pragma once

#include <cmath>

namespace mathlib {
	template <typename T, size_t N>
	struct Vector; // Forward declaration

	template <typename T>
	struct Quaternion; // Forward declaration

	template<typename T>
	struct AxisAngle {
		Vector<T, 4> data;

		AxisAngle() : data{ 0, 0, 0, 0 } {}
		AxisAngle(T angle, T x, T y, T z) : data{ angle, x, y, z } {
			data = data.normalize();
		}

		T angle() const { return data[0]; }
		Vector<T, 3> axis() const { return Vector<T, 3>{ data[1], data[2], data[3] }; }

		Quaternion<T> toQuaternion() const {
			T half_angle = angle() * static_cast<T>(0.5);
			T s = std::sin(half_angle);
			return Quaternion<T>(
				std::cos(half_angle),
				axis()[0] * s,
				axis()[1] * s,
				axis()[2] * s
			);
		}
	};
}