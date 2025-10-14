#pragma once

#include <cmath>
#include <iostream>

namespace gem {
	template <typename T, size_t N>
	struct Vector; // Forward declaration

	template <typename T>
	struct AxisAngle; // Forward declaration

	template<typename T>
	struct Quaternion {
		Vector<T, 4> data;

		Quaternion() : data{ 1, 0, 0, 0 } {}
		Quaternion(T w, T x, T y, T z) : data{ w, x, y, z } {}
		Quaternion(const Vector<T, 4>& vec) : data(vec) {}

		T& operator[](size_t index) { return data[index]; }
		const T& operator[](size_t index) const { return data[index]; }

		T& w() { return data[0]; }
		T& x() { return data[1]; }
		T& y() { return data[2]; }
		T& z() { return data[3]; }

		// ADDITION OPERATORS
		// -------------------------------

		Quaternion<T> operator+(const Quaternion<T>& other) const {
			return Quaternion<T>(data + other.data);
		}

		Quaternion<T> operator+(T scalar) const {
			return Quaternion<T>(data + scalar);
		}

		friend Quaternion<T> operator+(T scalar, const Quaternion<T>& quat) {
			return Quaternion<T>(scalar + quat.data);
		}

		// SUBTRACTION OPERATORS
		// --------------------------------

		Quaternion<T> operator-() const {
			return Quaternion<T>(-data);
		}

		Quaternion<T> operator-(const Quaternion<T>& other) const {
			return Quaternion<T>(data - other.data);
		}

		Quaternion<T> operator-(T scalar) const {
			return Quaternion<T>(data - scalar);
		}

		friend Quaternion<T> operator-(T scalar, const Quaternion<T>& quat) {
			return Quaternion<T>(scalar - quat.data);
		}

		// MULTIPLICATION OPERATORS
		// -----------------------------------

		Quaternion<T> operator*(const Quaternion<T>& other) const {
			return Quaternion<T>(
				data[0] * other.data[0] - data[1] * other.data[1] - data[2] * other.data[2] - data[3] * other.data[3],
				data[0] * other.data[1] + data[1] * other.data[0] + data[2] * other.data[3] - data[3] * other.data[2],
				data[0] * other.data[2] - data[1] * other.data[3] + data[2] * other.data[0] + data[3] * other.data[1],
				data[0] * other.data[3] + data[1] * other.data[2] - data[2] * other.data[1] + data[3] * other.data[0]
			);
		}

		Quaternion<T> operator*(T scalar) const {
			return Quaternion<T>(data * scalar);
		}

		friend Quaternion<T> operator*(T scalar, const Quaternion<T>& quat) {
			return Quaternion<T>(scalar * quat.data);
		}

		// CONJUGATE FUNCTIONS
		// -------------------------------

		Quaternion<T> operator~() const {
			return Quaternion<T>(data[0], -data[1], -data[2], -data[3]);
		}

		Quaternion<T> conjugate() const {
			return ~(*this);
		}

		// QUATERNION FUNCTIONS
		// -------------------------------

		T magnitude() const {
			return data.magnitude();
		}

		Quaternion<T> normalize() const {
			return Quaternion<T>(data.normalize());
		}

		Quaternion<T> inverse() const {
			T mag_sq = magnitude() * magnitude();
			if (mag_sq == 0) throw std::runtime_error("Cannot invert a zero-magnitude quaternion.");
			
			return conjugate() * (1 / mag_sq);
		}

		T dot(const Quaternion<T>& other) const {
			return data.dot(other.data);
		}

		AxisAngle<T> toAxisAngle() const {
			Quaternion<T> q = normalize();

			T angle = 2 * std::acos(q.w());
			T s = std::sqrt(1 - q.w() * q.w());

			if (s < 0.001) { 
				return AxisAngle<T>();
			} else {
				return AxisAngle<T>(angle, q.x() / s, q.y() / s, q.z() / s);
			}
		}
	};
}