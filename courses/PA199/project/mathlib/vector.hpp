#pragma once

#include <cmath>
#include <array>

namespace mathlib {
	template<typename T, size_t N>
	struct Vector {
		T data[N];

		T& operator[](size_t index) { return data[index]; }
		const T& operator[](size_t index) const { return data[index]; }

		// ADDITION OPERATORS
		// -------------------------------

		Vector<T, N> operator+(const Vector<T, N>& other) const {
			Vector<T, N> result;
			VectorAdd<N>::compute(data, other.data, result.data);
			return result;
		}

		Vector<T, N> operator+(T scalar) const {
			Vector<T, N> result;
			VectorAddScalar<N>::compute(data, scalar, result.data);
			return result;
		}

		friend Vector<T, N> operator+(T scalar, const Vector<T, N>& v) {
			return v + scalar;
		}

		// SUBTRACTION OPERATORS
		// --------------------------------

		Vector<T, N> operator-(const Vector<T, N>& other) const {
			Vector<T, N> result;
			VectorSub<N>::compute(data, other.data, result.data);
			return result;
		}

		Vector<T, N> operator-(T scalar) const {
			Vector<T, N> result;
			VectorSubScalar<N>::compute(data, scalar, result.data);
			return result;
		}

		friend Vector<T, N> operator-(T scalar, const Vector<T, N>& v) {
			Vector<T, N> result;
			ScalarSubVector<N>::compute(scalar, v.data, result.data);
			return result;
		}

		// MULTIPLICATION OPERATORS
		// --------------------------------

		Vector<T, N> operator*(const Vector<T, N>& other) const {
			Vector<T, N> result;
			VectorMul<N>::compute(data, other.data, result.data);
			return result;
		}

		Vector<T, N> operator*(T scalar) const {
			Vector<T, N> result;
			VectorMulScalar<N>::compute(data, scalar, result.data);
			return result;
		}

		friend Vector<T, N> operator*(T scalar, const Vector<T, N>& v) {
			return v * scalar;
		}

		// DIVISION OPERATORS
		// --------------------------------

		Vector<T, N> operator/(const Vector<T, N>& other) const {
			Vector<T, N> result;
			VectorDiv<N>::compute(data, other.data, result.data);
			return result;
		}

		Vector<T, N> operator/(T scalar) const {
			Vector<T, N> result;
			VectorDivScalar<N>::compute(data, scalar, result.data);
			return result;
		}

		friend Vector<T, N> operator/(T scalar, const Vector<T, N>& v) {
			Vector<T, N> result;
			ScalarDivVector<N>::compute(scalar, v.data, result.data);
			return result;
		}

		// NEGATION OPERATOR & FUNCTION
		// -------------------------------

		Vector<T, N> operator-() const {
			Vector<T, N> result;
			VectorMulScalar<N>::compute(data, -1, result.data);
			return result;
		}

		// Returns the negated version of the vector
		Vector<T, N> negate() const {
			return -*this;
		}

		// DOT PRODUCT FUNCTION
		// -------------------------------

		// Returns the dot product of this vector with another vector
		T dot(const Vector<T, N>& other) const {
			return VectorDotProduct<N>::compute(data, other.data);
		}

		// CROSS PRODUCT FUNCTION (Only for 3D Vectors)
		// -------------------------------

		// Returns the cross product of this vector with another vector (only for 3D vectors)
		Vector<T, 3> cross(const Vector<T, 3>& other) const {
			static_assert(N == 3, "MATHLIB: Cross product is only defined for 3D vectors.");
			Vector<T, 3> result;
			result[0] = data[1] * other.data[2] - data[2] * other.data[1];
			result[1] = data[2] * other.data[0] - data[0] * other.data[2];
			result[2] = data[0] * other.data[1] - data[1] * other.data[0];
			return result;
		}

		// VECTOR FUNCTIONS
		// -------------------------------

		// Returns the magnitude (length) of the vector
		T magnitude() const {
			return std::sqrt(VectorSquareSum<N>::compute(data));
		}

		// Returns a normalized (unit length) version of the vector
		Vector<T, N> normalize() const {
			T mag = magnitude();
			if (mag == 0) return *this;

			Vector<T, N> result;
			VectorDivScalar<N>::compute(data, mag, result.data);

			return result;
		}

		size_t size() const { return N; }

	private:
		// Template for Vector Addition Cases
		template<size_t N>
		struct VectorAdd {
			static inline void compute(const T* a, const T* b, T* result) {
				VectorAdd<N - 1>::compute(a, b, result);
				result[N - 1] = a[N - 1] + b[N - 1];
			}
		};

		// Template for Vector Addition Cases (Base Case)
		template<>
		struct VectorAdd<1> {
			static inline void compute(const T* a, const T* b, T* result) {
				result[0] = a[0] + b[0];
			}
		};

		// Template for Vector and Scalar Addition Cases
		template<size_t N>
		struct VectorAddScalar {
			static inline void compute(const T* a, const T b, T* result) {
				VectorAddScalar<N - 1>::compute(a, b, result);
				result[N - 1] = a[N - 1] + b;
			}
		};

		// Template for Vector and Scalar Addition Cases (Base Case)
		template<>
		struct VectorAddScalar<1> {
			static inline void compute(const T* a, const T b, T* result) {
				result[0] = a[0] + b;
			}
		};

		// Template for Vector Subtraction Cases
		template<size_t N>
		struct VectorSub {
			static inline void compute(const T* a, const T* b, T* result) {
				VectorSub<N - 1>::compute(a, b, result);
				result[N - 1] = a[N - 1] - b[N - 1];
			}
		};
		
		// Template for Vector Subtraction Cases (Base Case)
		template<>
		struct VectorSub<1> {
			static inline void compute(const T* a, const T* b, T* result) {
				result[0] = a[0] - b[0];
			}
		};

		// Template for Vector and Scalar Subtraction Cases
		template<size_t N>
		struct VectorSubScalar {
			static inline void compute(const T* a, const T b, T* result) {
				VectorSubScalar<N - 1>::compute(a, b, result);
				result[N - 1] = a[N - 1] - b;
			}
		};

		// Template for Vector and Scalar Subtraction Cases (Base Case)
		template<>
		struct VectorSubScalar<1> {
			static inline void compute(const T* a, const T b, T* result) {
				result[0] = a[0] - b;
			}
		};

		// Template for Scalar and Vector Subtraction Cases
		template<size_t N>
		struct ScalarSubVector {
			static inline void compute(const T a, const T* b, T* result) {
				ScalarSubVector<N - 1>::compute(a, b, result);
				result[N - 1] = a - b[N - 1];
			}
		};

		// Template for Scalar and Vector Subtraction Cases (Base Case)
		template<>
		struct ScalarSubVector<1> {
			static inline void compute(const T a, const T* b, T* result) {
				result[0] = a - b[0];
			}
		};

		// Template for Vector Multiplcation Cases
		template<size_t N>
		struct VectorMul {
			static inline void compute(const T* a, const T* b, T* result) {
				VectorMul<N - 1>::compute(a, b, result);
				result[N - 1] = a[N - 1] * b[N - 1];
			}
		};

		// Template for Vector Multiplcation Cases (Base Case)
		template<>
		struct VectorMul<1> {
			static inline void compute(const T* a, const T* b, T* result) {
				result[0] = a[0] * b[0];
			}
		};

		// Template for Vector and Scalar Multiplication Cases
		template<size_t N>
		struct VectorMulScalar {
			static inline void compute(const T* a, const T b, T* result) {
				VectorMulScalar<N - 1>::compute(a, b, result);
				result[N - 1] = a[N - 1] * b;
			}
		};

		// Template for Vector and Scalar Multiplication Cases (Base Case)
		template<>
		struct VectorMulScalar<1> {
			static inline void compute(const T* a, const T b, T* result) {
				result[0] = a[0] * b;
			}
		};

		// Template for Vector Division Cases
		template<size_t N>
		struct VectorDiv {
			static inline void compute(const T* a, const T* b, T* result) {
				VectorDiv<N - 1>::compute(a, b, result);
				result[N - 1] = a[N - 1] / b[N - 1];
			}
		};

		// Template for Vector Multiplcation Cases (Base Case)
		template<>
		struct VectorDiv<1> {
			static inline void compute(const T* a, const T* b, T* result) {
				result[0] = a[0] / b[0];
			}
		};

		// Template for Vector and Scalar Division Cases
		template<size_t N>
		struct VectorDivScalar {
			static inline void compute(const T* a, const T b, T* result) {
				VectorDivScalar<N - 1>::compute(a, b, result);
				result[N - 1] = a[N - 1] / b;
			}
		};

		// Template for Vector and Scalar Division Cases (Base Case)
		template<>
		struct VectorDivScalar<1> {
			static inline void compute(const T* a, const T b, T* result) {
				result[0] = a[0] / b;
			}
		};

		// Template for Scalar and Vector Division Cases
		template<size_t N>
		struct ScalarDivVector {
			static inline void compute(const T a, const T* b, T* result) {
				ScalarDivVector<N - 1>::compute(a, b, result);
				result[N - 1] = a / b[N - 1];
			}
		};

		// Template for Scalar and Vector Division Cases (Base Case)
		template<>
		struct ScalarDivVector<1> {
			static inline void compute(const T a, const T* b, T* result) {
				result[0] = a / b[0];
			}
		};


		// Template for Vector Square Sum Cases
		template<size_t N>
		struct VectorSquareSum {
			static inline T compute(const T* a) {
				return a[N - 1] * a[N - 1] + VectorSquareSum<N - 1>::compute(a);
			}
		};

		// Template for Vector Square Sum Cases (Base Case)
		template<>
		struct VectorSquareSum<1> {
			static inline T compute(const T* a) {
				return a[0] * a[0];
			}
		};

		// Template for Vector Dot Product Cases
		template<size_t N>
		struct VectorDotProduct {
			static inline T compute(const T* a, const T* b) {
				return a[N - 1] * b[N - 1] + VectorDotProduct<N - 1>::compute(a, b);
			}
		};

		// Template for Vector Dot Product Cases (Base Case)
		template<>
		struct VectorDotProduct<1> {
			static inline T compute(const T* a, const T* b) {
				return a[0] * b[0];
			}
		};
	};
}