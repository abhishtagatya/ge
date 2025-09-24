#pragma once

#include <cmath>

namespace mathlib {
	template<typename T, size_t N>
	struct Vector {
		T data[N];

		T& operator[](size_t index) { return data[index]; }
		const T& operator[](size_t index) const { return data[index]; }

		Vector<T, N> operator+(const Vector<T, N>& other) const {
			Vector<T, N> result;
			VectorAdd<N>::compute(data, other.data, result.data);
			return result;
		}

		Vector<T, N> operator-(const Vector<T, N>& other) const {
			Vector<T, N> result;
			VectorSub<N>::compute(data, other.data, result.data);
			return result;
		}

		Vector<T, N> operator*(const Vector<T, N>& other) const {
			Vector<T, N> result;
			VectorMul<N>::compute(data, other.data, result.data);
			return result;
		}

		Vector<T, N> operator/(const Vector<T, N>& other) const {
			Vector<T, N> result;
			VectorDiv<N>::compute(data, other.data, result.data);
			return result;
		}

		T magnitude() const {
			return std::sqrt(VectorSquareSum<N>::compute(data));
		}

		// TODO: Unit Vector -> Normalize
		// TODO: Copy by Value -> Copy Function
		// TODO: Opposite Vector -> Negate Function
		// QUESTION: Cross Product -> Only for up to 4D Vectors? Should I make it special template for each?
		//-> Solution : Make a special template for 3D Vectors only for Cross Product and leave others without it.
		// QUESTION: How is this structure generally?

		size_t size() const { return N; }

	private:
		template<size_t N>
		struct VectorAdd {
			static inline void compute(const T* a, const T* b, T* result) {
				VectorAdd<N - 1>::compute(a, b, result);
				result[N - 1] = a[N - 1] + b[N - 1];
			}
		};

		template<>
		struct VectorAdd<1> {
			static inline void compute(const T* a, const T* b, T* result) {
				result[0] = a[0] + b[0];
			}
		};

		template<size_t N>
		struct VectorSub {
			static inline void compute(const T* a, const T* b, T* result) {
				VectorSub<N - 1>::compute(a, b, result);
				result[N - 1] = a[N - 1] - b[N - 1];
			}
		};
		
		template<>
		struct VectorSub<1> {
			static inline void compute(const T* a, const T* b, T* result) {
				result[0] = a[0] - b[0];
			}
		};

		template<size_t N>
		struct VectorMul {
			static inline void compute(const T* a, const T* b, T* result) {
				VectorMul<N - 1>::compute(a, b, result);
				result[N - 1] = a[N - 1] * b[N - 1];
			}
		};

		template<>
		struct VectorMul<1> {
			static inline void compute(const T* a, const T* b, T* result) {
				result[0] = a[0] * b[0];
			}
		};

		template<size_t N>
		struct VectorDiv {
			static inline void compute(const T* a, const T* b, T* result) {
				VectorDiv<N - 1>::compute(a, b, result);
				result[N - 1] = a[N - 1] / b[N - 1];
			}
		};

		template<>
		struct VectorDiv<1> {
			static inline void compute(const T* a, const T* b, T* result) {
				result[0] = a[0] / b[0];
			}
		};

		template<size_t N>
		struct VectorSquareSum {
			static inline T compute(const T* a) {
				return a[N - 1] * a[N - 1] + VectorSquareSum<N - 1>::compute(a);
			}
		};

		template<>
		struct VectorSquareSum<1> {
			static inline T compute(const T* a) {
				return a[0] * a[0];
			}
		};
	};
}