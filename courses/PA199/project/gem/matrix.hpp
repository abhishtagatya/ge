#pragma once

#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>
#include <array>

namespace gem {
	template <typename T, size_t N>
	struct Vector; // Forward declaration

	template <typename T>
	struct Matrix4 {
		std::array<std::array<T, 4>, 4> data;

		T& operator()(int row, int col) {
			return data[row][col];
		}

		const T& operator()(int row, int col) const {
			return data[row][col];
		}

		T* operator[](int row) {
			return data[row].data();
		}

		Matrix4() = default;

		Matrix4(std::initializer_list<std::initializer_list<T>> list) {
			size_t i = 0;
			for (auto& row : list) {
				size_t j = 0;
				for (auto& val : row) {
					data[i][j++] = val;
				}
				++i;
			}
		}

		// MATRIX DEFINITIONS
		// -------------------------------

		static Matrix4 identity() {
			Matrix4<T> result = {};

			result.data[0][0] = 1; result.data[0][1] = 0; result.data[0][2] = 0; result.data[0][3] = 0;
			result.data[1][0] = 0; result.data[1][1] = 1; result.data[1][2] = 0; result.data[1][3] = 0;
			result.data[2][0] = 0; result.data[2][1] = 0; result.data[2][2] = 1; result.data[2][3] = 0;
			result.data[3][0] = 0; result.data[3][1] = 0; result.data[3][2] = 0; result.data[3][3] = 1;

			return result;
		}

		static Matrix4 zero() {
			Matrix4<T> result = {};
			result.data[0][0] = 0; result.data[0][1] = 0; result.data[0][2] = 0; result.data[0][3] = 0;
			result.data[1][0] = 0; result.data[1][1] = 0; result.data[1][2] = 0; result.data[1][3] = 0;
			result.data[2][0] = 0; result.data[2][1] = 0; result.data[2][2] = 0; result.data[2][3] = 0;
			result.data[3][0] = 0; result.data[3][1] = 0; result.data[3][2] = 0; result.data[3][3] = 0;
			return result;
		}

		static Matrix4 translation(const Vector<T, 3>& v) {
			Matrix4<T> result = identity();
			result.data[0][3] = v[0];
			result.data[1][3] = v[1];
			result.data[2][3] = v[2];
			return result;
		}

		static Matrix4 scale(const Vector<T, 3>& v) {
			Matrix4<T> result = identity();
			result.data[0][0] = v[0];
			result.data[1][1] = v[1];
			result.data[2][2] = v[2];
			return result;
		}

		static Matrix4 rotationX(T angle) {
			Matrix4<T> result = identity();
			T c = std::cos(angle);
			T s = std::sin(angle);
			result.data[1][1] = c;  result.data[1][2] = -s;
			result.data[2][1] = s;  result.data[2][2] = c;
			return result;
		}

		static Matrix4 rotationY(T angle) {
			Matrix4<T> result = identity();
			T c = std::cos(angle);
			T s = std::sin(angle);
			result.data[0][0] = c;  result.data[0][2] = s;
			result.data[2][0] = -s; result.data[2][2] = c;
			return result;
		}

		static Matrix4 rotationZ(T angle) {
			Matrix4<T> result = identity();
			T c = std::cos(angle);
			T s = std::sin(angle);
			result.data[0][0] = c;  result.data[0][1] = -s;
			result.data[1][0] = s;  result.data[1][1] = c;
			return result;
		}

		// ADDITION OPERATORS
		// -------------------------------

		Matrix4<T> operator+(const Matrix4<T>& other) const {
			Matrix4<T> result;

			result.data[0][0] = data[0][0] + other.data[0][0];
			result.data[0][1] = data[0][1] + other.data[0][1];
			result.data[0][2] = data[0][2] + other.data[0][2];
			result.data[0][3] = data[0][3] + other.data[0][3];

			result.data[1][0] = data[1][0] + other.data[1][0];
			result.data[1][1] = data[1][1] + other.data[1][1];
			result.data[1][2] = data[1][2] + other.data[1][2];
			result.data[1][3] = data[1][3] + other.data[1][3];

			result.data[2][0] = data[2][0] + other.data[2][0];
			result.data[2][1] = data[2][1] + other.data[2][1];
			result.data[2][2] = data[2][2] + other.data[2][2];
			result.data[2][3] = data[2][3] + other.data[2][3];

			result.data[3][0] = data[3][0] + other.data[3][0];
			result.data[3][1] = data[3][1] + other.data[3][1];
			result.data[3][2] = data[3][2] + other.data[3][2];
			result.data[3][3] = data[3][3] + other.data[3][3];

			return result;
		}

		Matrix4<T> operator+(T scalar) const {
			Matrix4<T> result;

			result.data[0][0] = data[0][0] + scalar;
			result.data[0][1] = data[0][1] + scalar;
			result.data[0][2] = data[0][2] + scalar;
			result.data[0][3] = data[0][3] + scalar;

			result.data[1][0] = data[1][0] + scalar;
			result.data[1][1] = data[1][1] + scalar;
			result.data[1][2] = data[1][2] + scalar;
			result.data[1][3] = data[1][3] + scalar;

			result.data[2][0] = data[2][0] + scalar;
			result.data[2][1] = data[2][1] + scalar;
			result.data[2][2] = data[2][2] + scalar;
			result.data[2][3] = data[2][3] + scalar;

			result.data[3][0] = data[3][0] + scalar;
			result.data[3][1] = data[3][1] + scalar;
			result.data[3][2] = data[3][2] + scalar;
			result.data[3][3] = data[3][3] + scalar;

			return result;
		}

		friend Matrix4<T> operator+(T scalar, const Matrix4<T>& mat) {
			return mat + scalar;
		}

		// SUBTRACTION OPERATORS
		// --------------------------------

		Matrix4<T> operator-(const Matrix4<T>& other) const {
			Matrix4<T> result;

			result.data[0][0] = data[0][0] - other.data[0][0];
			result.data[0][1] = data[0][1] - other.data[0][1];
			result.data[0][2] = data[0][2] - other.data[0][2];
			result.data[0][3] = data[0][3] - other.data[0][3];

			result.data[1][0] = data[1][0] - other.data[1][0];
			result.data[1][1] = data[1][1] - other.data[1][1];
			result.data[1][2] = data[1][2] - other.data[1][2];
			result.data[1][3] = data[1][3] - other.data[1][3];

			result.data[2][0] = data[2][0] - other.data[2][0];
			result.data[2][1] = data[2][1] - other.data[2][1];
			result.data[2][2] = data[2][2] - other.data[2][2];
			result.data[2][3] = data[2][3] - other.data[2][3];

			result.data[3][0] = data[3][0] - other.data[3][0];
			result.data[3][1] = data[3][1] - other.data[3][1];
			result.data[3][2] = data[3][2] - other.data[3][2];
			result.data[3][3] = data[3][3] - other.data[3][3];

			return result;
		}

		Matrix4<T> operator-(T scalar) const {
			Matrix4<T> result;

			result.data[0][0] = data[0][0] - scalar;
			result.data[0][1] = data[0][1] - scalar;
			result.data[0][2] = data[0][2] - scalar;
			result.data[0][3] = data[0][3] - scalar;

			result.data[1][0] = data[1][0] - scalar;
			result.data[1][1] = data[1][1] - scalar;
			result.data[1][2] = data[1][2] - scalar;
			result.data[1][3] = data[1][3] - scalar;

			result.data[2][0] = data[2][0] - scalar;
			result.data[2][1] = data[2][1] - scalar;
			result.data[2][2] = data[2][2] - scalar;
			result.data[2][3] = data[2][3] - scalar;

			result.data[3][0] = data[3][0] - scalar;
			result.data[3][1] = data[3][1] - scalar;
			result.data[3][2] = data[3][2] - scalar;
			result.data[3][3] = data[3][3] - scalar;

			return result;
		}

		friend Matrix4<T> operator-(T scalar, const Matrix4<T>& other) {
			Matrix4<T> result;

			result.data[0][0] = scalar - other.data[0][0];
			result.data[0][1] = scalar - other.data[0][1];
			result.data[0][2] = scalar - other.data[0][2];
			result.data[0][3] = scalar - other.data[0][3];

			result.data[1][0] = scalar - other.data[1][0];
			result.data[1][1] = scalar - other.data[1][1];
			result.data[1][2] = scalar - other.data[1][2];
			result.data[1][3] = scalar - other.data[1][3];

			result.data[2][0] = scalar - other.data[2][0];
			result.data[2][1] = scalar - other.data[2][1];
			result.data[2][2] = scalar - other.data[2][2];
			result.data[2][3] = scalar - other.data[2][3];

			result.data[3][0] = scalar - other.data[3][0];
			result.data[3][1] = scalar - other.data[3][1];
			result.data[3][2] = scalar - other.data[3][2];
			result.data[3][3] = scalar - other.data[3][3];

			return result;
		}

		// MULTIPLICATION OPERATORS
		// --------------------------------

		Matrix4<T> operator*(const Matrix4<T>& other) const {
			Matrix4<T> result;

			result.data[0][0] = data[0][0] * other.data[0][0] + data[0][1] * other.data[1][0] + data[0][2] * other.data[2][0] + data[0][3] * other.data[3][0];
			result.data[0][1] = data[0][0] * other.data[0][1] + data[0][1] * other.data[1][1] + data[0][2] * other.data[2][1] + data[0][3] * other.data[3][1];
			result.data[0][2] = data[0][0] * other.data[0][2] + data[0][1] * other.data[1][2] + data[0][2] * other.data[2][2] + data[0][3] * other.data[3][2];
			result.data[0][3] = data[0][0] * other.data[0][3] + data[0][1] * other.data[1][3] + data[0][2] * other.data[2][3] + data[0][3] * other.data[3][3];

			result.data[1][0] = data[1][0] * other.data[0][0] + data[1][1] * other.data[1][0] + data[1][2] * other.data[2][0] + data[1][3] * other.data[3][0];
			result.data[1][1] = data[1][0] * other.data[0][1] + data[1][1] * other.data[1][1] + data[1][2] * other.data[2][1] + data[1][3] * other.data[3][1];
			result.data[1][2] = data[1][0] * other.data[0][2] + data[1][1] * other.data[1][2] + data[1][2] * other.data[2][2] + data[1][3] * other.data[3][2];
			result.data[1][3] = data[1][0] * other.data[0][3] + data[1][1] * other.data[1][3] + data[1][2] * other.data[2][3] + data[1][3] * other.data[3][3];

			result.data[2][0] = data[2][0] * other.data[0][0] + data[2][1] * other.data[1][0] + data[2][2] * other.data[2][0] + data[2][3] * other.data[3][0];
			result.data[2][1] = data[2][0] * other.data[0][1] + data[2][1] * other.data[1][1] + data[2][2] * other.data[2][1] + data[2][3] * other.data[3][1];
			result.data[2][2] = data[2][0] * other.data[0][2] + data[2][1] * other.data[1][2] + data[2][2] * other.data[2][2] + data[2][3] * other.data[3][2];
			result.data[2][3] = data[2][0] * other.data[0][3] + data[2][1] * other.data[1][3] + data[2][2] * other.data[2][3] + data[2][3] * other.data[3][3];

			result.data[3][0] = data[3][0] * other.data[0][0] + data[3][1] * other.data[1][0] + data[3][2] * other.data[2][0] + data[3][3] * other.data[3][0];
			result.data[3][1] = data[3][0] * other.data[0][1] + data[3][1] * other.data[1][1] + data[3][2] * other.data[2][1] + data[3][3] * other.data[3][1];
			result.data[3][2] = data[3][0] * other.data[0][2] + data[3][1] * other.data[1][2] + data[3][2] * other.data[2][2] + data[3][3] * other.data[3][2];
			result.data[3][3] = data[3][0] * other.data[0][3] + data[3][1] * other.data[1][3] + data[3][2] * other.data[2][3] + data[3][3] * other.data[3][3];

			return result;
		}

		Matrix4<T> operator*(T scalar) const {
			Matrix4<T> result;

			result.data[0][0] = data[0][0] * scalar;
			result.data[0][1] = data[0][1] * scalar;
			result.data[0][2] = data[0][2] * scalar;
			result.data[0][3] = data[0][3] * scalar;

			result.data[1][0] = data[1][0] * scalar;
			result.data[1][1] = data[1][1] * scalar;
			result.data[1][2] = data[1][2] * scalar;
			result.data[1][3] = data[1][3] * scalar;

			result.data[2][0] = data[2][0] * scalar;
			result.data[2][1] = data[2][1] * scalar;
			result.data[2][2] = data[2][2] * scalar;
			result.data[2][3] = data[2][3] * scalar;

			result.data[3][0] = data[3][0] * scalar;
			result.data[3][1] = data[3][1] * scalar;
			result.data[3][2] = data[3][2] * scalar;
			result.data[3][3] = data[3][3] * scalar;

			return result;
		}

		friend Matrix4<T> operator*(T scalar, const Matrix4<T>& mat) {
			return mat * scalar;
		}

		Vector<T, 4> operator*(const Vector<T, 4>& v) const {
			Vector<T, 4> result;

			result.data[0] = data[0][0] * v[0] + data[0][1] * v[1] + data[0][2] * v[2] + data[0][3] * v[3];
			result.data[1] = data[1][0] * v[0] + data[1][1] * v[1] + data[1][2] * v[2] + data[1][3] * v[3];
			result.data[2] = data[2][0] * v[0] + data[2][1] * v[1] + data[2][2] * v[2] + data[2][3] * v[3];
			result.data[3] = data[3][0] * v[0] + data[3][1] * v[1] + data[3][2] * v[2] + data[3][3] * v[3];
			
			return result;
		}

		friend Vector<T, 4> operator*(const Vector<T, 4>& v, const Matrix4<T>& mat) {
			Vector<T, 4> result;
			result.data[0] = v[0] * mat.data[0][0] + v[1] * mat.data[1][0] + v[2] * mat.data[2][0] + v[3] * mat.data[3][0];
			result.data[1] = v[0] * mat.data[0][1] + v[1] * mat.data[1][1] + v[2] * mat.data[2][1] + v[3] * mat.data[3][1];
			result.data[2] = v[0] * mat.data[0][2] + v[1] * mat.data[1][2] + v[2] * mat.data[2][2] + v[3] * mat.data[3][2];
			result.data[3] = v[0] * mat.data[0][3] + v[1] * mat.data[1][3] + v[2] * mat.data[2][3] + v[3] * mat.data[3][3];
			return result;
		}

		// NEGATION OPERATOR & FUNCTION
		// -------------------------------

		Matrix4<T> operator-() const {
			Matrix4<T> result;

			result.data[0][0] = -data[0][0];
			result.data[0][1] = -data[0][1];
			result.data[0][2] = -data[0][2];
			result.data[0][3] = -data[0][3];

			result.data[1][0] = -data[1][0];
			result.data[1][1] = -data[1][1];
			result.data[1][2] = -data[1][2];
			result.data[1][3] = -data[1][3];

			result.data[2][0] = -data[2][0];
			result.data[2][1] = -data[2][1];
			result.data[2][2] = -data[2][2];
			result.data[2][3] = -data[2][3];

			result.data[3][0] = -data[3][0];
			result.data[3][1] = -data[3][1];
			result.data[3][2] = -data[3][2];
			result.data[3][3] = -data[3][3];

			return result;
		}

		Matrix4<T> negate() const {
			return -*this;
		}

		// TRANSPOSITION FUNCTION
		// -------------------------------

		Matrix4<T> transpose() const {
			Matrix4<T> result;

			result.data[0][0] = data[0][0];
			result.data[0][1] = data[1][0];
			result.data[0][2] = data[2][0];
			result.data[0][3] = data[3][0];

			result.data[1][0] = data[0][1];
			result.data[1][1] = data[1][1];
			result.data[1][2] = data[2][1];
			result.data[1][3] = data[3][1];

			result.data[2][0] = data[0][2];
			result.data[2][1] = data[1][2];
			result.data[2][2] = data[2][2];
			result.data[2][3] = data[3][2];

			result.data[3][0] = data[0][3];
			result.data[3][1] = data[1][3];
			result.data[3][2] = data[2][3];
			result.data[3][3] = data[3][3];

			return result;
		}

		// DETERMINANT FUNCTION
		// -------------------------------

		T det() const {
			const T& a = data[0][0], b = data[0][1], c = data[0][2], d = data[0][3];
			const T& e = data[1][0], f = data[1][1], g = data[1][2], h = data[1][3];
			const T& i = data[2][0], j = data[2][1], k = data[2][2], l = data[2][3];
			const T& m = data[3][0], n = data[3][1], o = data[3][2], p = data[3][3];

			return
				a * (f * (k * p - l * o) - g * (j * p - l * n) + h * (j * o - k * n)) -
				b * (e * (k * p - l * o) - g * (i * p - l * m) + h * (i * o - k * m)) +
				c * (e * (j * p - l * n) - f * (i * p - l * m) + h * (i * n - j * m)) -
				d * (e * (j * o - k * n) - f * (i * o - k * m) + g * (i * n - j * m));
		}

		// INVERSE FUNCTION
		// -------------------------------

		Matrix4<T> inverse() const {
			T determinant = det();
			if (determinant == 0) {
				throw std::runtime_error("Matrix is not invertible");
			}

			T invDet = 1 / determinant;
			Matrix4<T> result;

			const T& a = data[0][0], b = data[0][1], c = data[0][2], d = data[0][3];
			const T& e = data[1][0], f = data[1][1], g = data[1][2], h = data[1][3];
			const T& i = data[2][0], j = data[2][1], k = data[2][2], l = data[2][3];
			const T& m = data[3][0], n = data[3][1], o = data[3][2], p = data[3][3];

			result.data[0][0] = (f * (k * p - l * o) - g * (j * p - l * n) + h * (j * o - k * n)) * invDet;
			result.data[0][1] = (c * (j * p - l * n) - b * (k * p - l * o) - d * (j * o - k * n)) * invDet;
			result.data[0][2] = (b * (g * p - h * o) - c * (f * p - h * n) + d * (f * o - g * n)) * invDet;
			result.data[0][3] = (c * (f * l - g * k) - b * (g * l - h * k) + d * (f * k - g * j)) * invDet;

			result.data[1][0] = (g * (i * p - l * m) - e * (k * p - l * o) + h * (i * o - k * m)) * invDet;
			result.data[1][1] = (a * (k * p - l * o) - c * (i * p - l * m) - d * (i * o - k * m)) * invDet;
			result.data[1][2] = (c * (e * p - h * m) - a * (g * p - h * o) + d * (e * o - g * m)) * invDet;
			result.data[1][3] = (a * (g * l - h * k) - c * (e * l - h * i) + d * (e * k - g * i)) * invDet;

			result.data[2][0] = (e * (j * p - l * n) - f * (i * p - l * m) + h * (i * n - j * m)) * invDet;
			result.data[2][1] = (b * (i * p - l * m) - a * (j * p - l * n) - d * (i * n - j * m)) * invDet;
			result.data[2][2] = (a * (f * p - h * n) - b * (e * p - h * m) + d * (e * n - f * m)) * invDet;
			result.data[2][3] = (b * (e * l - h * i) - a * (f * l - h * k) + d * (e * k - f * j)) * invDet;

			result.data[3][0] = (f * (i * o - k * m) - e * (j * o - k * n) + g * (i * n - j * m)) * invDet;
			result.data[3][1] = (a * (j * o - k * n) - b * (i * o - k * m) - c * (i * n - j * m)) * invDet;
			result.data[3][2] = (b * (e * o - g * m) - a * (f * o - g * n) + c * (e * n - f * m)) * invDet;
			result.data[3][3] = (a * (f * k - g * j) - b * (e * k - g * i) + c * (e * j - f * i)) * invDet;

			return result;
		}

		// LOOKAT FUNCTION
		// -------------------------------

		static Matrix4<T> lookAt(const Vector<T, 3>& eye,
			const Vector<T, 3>& center,
			const Vector<T, 3>& up)
		{
			Vector<T, 3> nup = up;
			Vector<T, 3> f = (center - eye).normalize();
			if (f.cross(up).magnitude() < 0.001) {
				nup = (fabs(f[1]) > 0.99f) ? Vector<T, 3>{ 0, 0, 1 } : Vector<T, 3>{ 0, 1, 0 };
			}
			Vector<T, 3> r = f.cross(nup).normalize();
			Vector<T, 3> u = r.cross(f);

			Matrix4<T> result = identity();

			result[0][0] = r[0]; 
			result[0][1] = r[1]; 
			result[0][2] = r[2]; 
			result[0][3] = -r.dot(eye);

			result[1][0] = u[0]; 
			result[1][1] = u[1]; 
			result[1][2] = u[2]; 
			result[1][3] = -u.dot(eye);

			result[2][0] = -f[0]; 
			result[2][1] = -f[1]; 
			result[2][2] = -f[2]; 
			result[2][3] = f.dot(eye);

			result[3][0] = 0; 
			result[3][1] = 0; 
			result[3][1] = 0; 
			result[3][3] = 1;

			/*
			std::cout << "world up: " << up[0] << "," << up[1] << "," << up[2] << "\n";
			std::cout << "front: " << f[0] << "," << f[1] << "," << f[2] << "\n";
			std::cout << "right: " << r[0] << "," << r[1] << "," << r[2] << "\n";
			std::cout << "up: " << u[0] << "," << u[1] << "," << u[2] << "\n";
			*/

			return result;
		}

		static Matrix4<T> perspective(T fov, T aspect, T near, T far) {
			Matrix4<T> result = zero();

			T tanHalfFov = std::tan(fov * (M_PI / 180) / 2);

			result[0][0] = 1 / (aspect * tanHalfFov);
			result[1][1] = 1 / (tanHalfFov);
			result[2][2] = -(far + near) / (far - near);
			result[2][3] = -(2 * far * near) / (far - near);
			result[3][2] = -1;
			result[3][3] = 0;

			return result;
		}

		static Matrix4<T> ortho(T left, T right, T bottom, T top, T near, T far) {
			Matrix4<T> result = Matrix4<T>::identity();

			result[0][0] = 2 / (right - left);
			result[1][1] = 2 / (top - bottom);
			result[2][2] = -2 / (far - near);
			result[3][0] = -(right + left) / (right - left);
			result[3][1] = -(top + bottom) / (top - bottom);
			result[3][2] = -(far + near) / (far - near);
			result[3][3] = 1;

			return result;
		}
	};
}