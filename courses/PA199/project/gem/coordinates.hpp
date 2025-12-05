#pragma once

#define _USE_MATH_DEFINES
#include <cmath>

namespace gem {
	template <typename T, size_t N>
	struct Vector;

	template <typename T>
	struct Polar {
		Vector<T, 2> data;

		Polar(T r = 0, T theta = 0) : data{ r, theta } {}
		T& r() { return data[0]; }
		T& theta() { return data[1]; }

		const T& r() const { return data[0]; }
		const T& theta() const { return data[1]; }

		Polar<T> normalize() const {
			// angle normalization
			T angle = std::fmod(theta(), 2 * M_PI);
			if (angle < 0) angle += 2 * M_PI;
			return Polar<T>(r(), angle);
		}
		
		Vector<T, 2> toCartesian() const {
			return Vector<T, 2>{
				r()* std::cos(theta()),
				r()* std::sin(theta())
			};
		}

		static Polar<T> fromCartesian(const Vector<T, 2>& v) {
			return Polar<T>(
				v.magnitude(),
				std::atan2(v[1], v[0])
			).normalize();
		}

		static Polar<T> fromCartesian(const Vector<T, 3>& v) {
			return fromCartesian(Vector<T, 2>{ {v[0], v[2]} });
		}
	};
}