#include "../mathlib/matrix.hpp"
#include "../mathlib/vector.hpp"
#include <gtest/gtest.h>

// Basic tests for mathlib::Matrix4
TEST(mathlib_matrix4_test_suite, m4_basic_test) {
	mathlib::Matrix4<int> m1 = { {1,2,3,4}, {5,6,7,8}, {9,10,11,12}, {13,14,15,16} };
	mathlib::Matrix4<int> m2 = { {16,15,14,13}, {12,11,10,9}, {8,7,6,5}, {4,3,2,1} };

	mathlib::Matrix4<float> m0 = mathlib::Matrix4<float>::zero();
	mathlib::Matrix4<float> mi = mathlib::Matrix4<float>::identity();

	mathlib::Vector<float, 3> v1 = { 1.0f, 2.0f, 3.0f };
	mathlib::Matrix4<float> mt = mathlib::Matrix4<float>::translation(v1);

	EXPECT_EQ(m1[0][0], 1);
	EXPECT_EQ(m1[1][2], 7);
	EXPECT_EQ(m2[3][3], 1);
	EXPECT_EQ(m2[2][1], 7);
	EXPECT_EQ(m1[3][0], 13);
	EXPECT_EQ(m2[0][3], 13);

	// Check for zero matrix
	for (size_t i = 0; i < 4; ++i) {
		for (size_t j = 0; j < 4; ++j) {
			EXPECT_EQ(m0[i][j], 0.0f);
		}
	}

	// Check for identity matrix
	for (size_t i = 0; i < 4; ++i) {
		for (size_t j = 0; j < 4; ++j) {
			if (i == j) {
				EXPECT_EQ(mi[i][j], 1.0f);
			} else {
				EXPECT_EQ(mi[i][j], 0.0f);
			}
		}
	}

	// Check for translation matrix
	for (size_t i = 0; i < 3; ++i) {
		for (size_t j = 0; j < 3; ++j) {
			if (i == j) {
				EXPECT_EQ(mt[i][j], 1.0f);
			} else {
				if (j < 3) {
					EXPECT_EQ(mt[i][j], 0.0f);
				} else {
					EXPECT_EQ(mt[i][j], v1[i]);
				}
			}
		}
	}
}

// Test for matrix-and-matrix addition
TEST(mathlib_matrix4_test_suite, m4nm4_add_test) {
	mathlib::Matrix4<int> m1 = { {1,2,3,4}, {5,6,7,8}, {9,10,11,12}, {13,14,15,16} };
	mathlib::Matrix4<int> m2 = { {16,15,14,13}, {12,11,10,9}, {8,7,6,5}, {4,3,2,1} };

	mathlib::Matrix4<int> result_m1m2 = m1 + m2;

	mathlib::Matrix4<int> result_m1m0 = m1 + mathlib::Matrix4<int>::zero();
	for (size_t i = 0; i < 4; ++i) {
		for (size_t j = 0; j < 4; ++j) {
			EXPECT_EQ(result_m1m0[i][j], m1[i][j]);
		}
	}

	for (size_t i = 0; i < 4; ++i) {
		for (size_t j = 0; j < 4; ++j) {
			EXPECT_EQ(result_m1m2[i][j], m2[i][j] + m1[i][j]);
		}
	}

	// Associativity check
	mathlib::Matrix4<int> m3 = { {1,1,1,1}, {1,1,1,1}, {1,1,1,1}, {1,1,1,1} };
	mathlib::Matrix4<int> result_a = (m1 + m2) + m3;
	mathlib::Matrix4<int> result_b = m1 + (m2 + m3);

	for (size_t i = 0; i < 4; ++i) {
		for (size_t j = 0; j < 4; ++j) {
			EXPECT_EQ(result_a[i][j], result_b[i][j]);
		}
	}
}

// Test for matrix-and-scalar addition
TEST(mathlib_matrix4_test_suite, m4ns_add_test) {
	mathlib::Matrix4<int> m1 = { {1,2,3,4}, {5,6,7,8}, {9,10,11,12}, {13,14,15,16} };
	int s = 10;
	
	mathlib::Matrix4<int> result_m1s = m1 + s;
	for (size_t i = 0; i < 4; ++i) {
		for (size_t j = 0; j < 4; ++j) {
			EXPECT_EQ(result_m1s[i][j], m1[i][j] + s);
			EXPECT_EQ(result_m1s[i][j], s + m1[i][j]);
		}
	}

	auto result_m0s = mathlib::Matrix4<int>::zero() + s;
	for (size_t i = 0; i < 4; ++i) {
		for (size_t j = 0; j < 4; ++j) {
			EXPECT_EQ(result_m0s[i][j], s);
		}
	}

	// Associativity check
	auto result_a = (m1 + s) + s;
	auto result_b = m1 + (s + s);
	for (size_t i = 0; i < 4; ++i) {
		for (size_t j = 0; j < 4; ++j) {
			EXPECT_EQ(result_a[i][j], result_b[i][j]);
		}
	}
}

// Test for matrix-and-matrix subtraction
TEST(mathlib_matrix4_test_suite, m4nm4_sub_test) {
	mathlib::Matrix4<int> m1 = { {1,2,3,4}, {5,6,7,8}, {9,10,11,12}, {13,14,15,16} };
	mathlib::Matrix4<int> m2 = { {16,15,14,13}, {12,11,10,9}, {8,7,6,5}, {4,3,2,1} };

	mathlib::Matrix4<int> result_m1m2 = m1 - m2;
	mathlib::Matrix4<int> result_m2m1 = m2 - m1;

	mathlib::Matrix4<int> result_m1m0 = m1 - mathlib::Matrix4<int>::zero();
	mathlib::Matrix4<int> result_m0m1 = mathlib::Matrix4<int>::zero() - m1;
	for (size_t i = 0; i < 4; ++i) {
		for (size_t j = 0; j < 4; ++j) {
			EXPECT_EQ(result_m1m0[i][j], m1[i][j]);
			EXPECT_EQ(result_m0m1[i][j], -m1[i][j]);
		}
	}

	for (size_t i = 0; i < 4; ++i) {
		for (size_t j = 0; j < 4; ++j) {
			EXPECT_EQ(result_m1m2[i][j], m1[i][j] - m2[i][j]);
			EXPECT_EQ(result_m2m1[i][j], m2[i][j] - m1[i][j]);
		}
	}

	// Non Associativity check
	mathlib::Matrix4<int> m3 = { {1,1,1,1}, {1,1,1,1}, {1,1,1,1}, {1,1,1,1} };
	mathlib::Matrix4<int> result_a = (m1 - m2) - m3;
	mathlib::Matrix4<int> result_b = m1 - (m2 - m3);

	mathlib::Matrix4<int> expected_a = { {-16, -14, -12, -10}, {-8, -6, -4, -2}, {0,2,4,6}, {8,10,12,14} };
	mathlib::Matrix4<int> expected_b = { { -14, -12, -10, -8}, {-6, -4, -2, 0}, {2,4,6,8}, {10,12,14,16} };

	for (size_t i = 0; i < 4; ++i) {
		for (size_t j = 0; j < 4; ++j) {
			EXPECT_EQ(result_a[i][j], expected_a[i][j]);
			EXPECT_EQ(result_b[i][j], expected_b[i][j]);
		}
	}
}

// Test for matrix-and-scalar subtraction
TEST(mathlib_matrix4_test_suite, m4ns_sub_test) {
	mathlib::Matrix4<int> m1 = { {1,2,3,4}, {5,6,7,8}, {9,10,11,12}, {13,14,15,16} };
	int s = 10;

	mathlib::Matrix4<int> result_m1s = m1 - s;
	mathlib::Matrix4<int> result_sm1 = s - m1;


	for (size_t i = 0; i < 4; ++i) {
		for (size_t j = 0; j < 4; ++j) {
			EXPECT_EQ(result_m1s[i][j], m1[i][j] - s);
			EXPECT_EQ(result_sm1[i][j], s - m1[i][j]);
		}
	}

	auto result_m0s = mathlib::Matrix4<int>::zero() - s;
	auto result_sm0 = s - mathlib::Matrix4<int>::zero();

	for (size_t i = 0; i < 4; ++i) {
		for (size_t j = 0; j < 4; ++j) {
			EXPECT_EQ(result_m0s[i][j], -s);
			EXPECT_EQ(result_sm0[i][j], s);
		}
	}

	// Non Associativity check
	auto result_a = (m1 - s) - s;
	auto result_b = m1 - (s - s);

	mathlib::Matrix4<int> expected_a = { {-19, -18, -17, -16}, {-15, -14, -13, -12}, {-11, -10, -9, -8}, {-7, -6, -5, -4} };
	mathlib::Matrix4<int> expected_b = m1;

	for (size_t i = 0; i < 4; ++i) {
		for (size_t j = 0; j < 4; ++j) {
			EXPECT_EQ(result_a[i][j], expected_a[i][j]);
			EXPECT_EQ(result_b[i][j], expected_b[i][j]);
		}
	}
}

// Test for matrix-and-matrix multiplication
TEST(mathlib_matrix4_test_suite, m4nm4_mul_test) {
	mathlib::Matrix4<int> m1 = {{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}, {13, 14, 15, 16}};
	mathlib::Matrix4<int> m2 = {{16, 15, 14, 13}, {12, 11, 10, 9}, {8, 7, 6, 5}, {4, 3, 2, 1}};

	mathlib::Matrix4<int> result_m1m2 = m1 * m2;
	mathlib::Matrix4<int> result_m2m1 = m2 * m1;

	mathlib::Matrix4<int> expected_m1m2 = {
		{80, 70, 60, 50},
		{240, 214, 188, 162},
		{400, 358, 316, 274},
		{560, 502, 444, 386}
	};

	mathlib::Matrix4<int> expected_m2m1 = {
		{386, 444, 502, 560},
		{274, 316, 358, 400},
		{162, 188, 214, 240},
		{50, 60, 70, 80}
	};

	for (size_t i = 0; i < 4; ++i) {
		for (size_t j = 0; j < 4; ++j) {
			EXPECT_EQ(result_m1m2[i][j], expected_m1m2[i][j]);
			EXPECT_EQ(result_m2m1[i][j], expected_m2m1[i][j]);
		}
	}

	auto result_m10 = m1 * mathlib::Matrix4<int>::zero();
	for (size_t i = 0; i < 4; ++i) {
		for (size_t j = 0; j < 4; ++j) {
			EXPECT_EQ(result_m10[i][j], 0);
		}
	}

	auto result_m1i = m1 * mathlib::Matrix4<int>::identity();
	for (size_t i = 0; i < 4; ++i) {
		for (size_t j = 0; j < 4; ++j) {
			EXPECT_EQ(result_m1i[i][j], m1[i][j]);
		}
	}
}

// Test for matrix-and-scalar multiplication
TEST(mathlib_matrix4_test_suite, m4ns_mul_test) {
	mathlib::Matrix4<int> m1 = {
		{1, 2, 3, 4},
		{5, 6, 7, 8},
		{9, 10, 11, 12},
		{13, 14, 15, 16}
	};
	int s = 2;

	mathlib::Matrix4<int> result_m1s = m1 * s;
	mathlib::Matrix4<int> result_sm1 = s * m1;

	for (size_t i = 0; i < 4; ++i) {
		for (size_t j = 0; j < 4; ++j) {
			EXPECT_EQ(result_m1s[i][j], m1[i][j] * s);
			EXPECT_EQ(result_sm1[i][j], s * m1[i][j]);
			EXPECT_EQ(result_m1s[i][j], result_sm1[i][j]);
		}
	}

	mathlib::Matrix4<int> result_m10 = m1 * 0;
	for (size_t i = 0; i < 4; ++i) {
		for (size_t j = 0; j < 4; ++j) {
			EXPECT_EQ(result_m10[i][j], 0);
		}
	}

	mathlib::Matrix4<int> result_m11 = m1 * 1;
	for (size_t i = 0; i < 4; ++i) {
		for (size_t j = 0; j < 4; ++j) {
			EXPECT_EQ(result_m11[i][j], m1[i][j]);
		}
	}

	int s1 = 2, s2 = 3;
	auto result_a = (m1 * s1) * s2;
	auto result_b = m1 * (s1 * s2);

	for (size_t i = 0; i < 4; ++i) {
		for (size_t j = 0; j < 4; ++j) {
			EXPECT_EQ(result_a[i][j], result_b[i][j]);
		}
	}
}

TEST(mathlib_matrix4_test_suite, m4_negate_test) {
	mathlib::Matrix4<int> m1 = {
		{1, -2, 3, -4},
		{-5, 6, -7, 8},
		{9, -10, 11, -12},
		{-13, 14, -15, 16}
	};
	mathlib::Matrix4<int> result_neg = -m1;
	for (size_t i = 0; i < 4; ++i) {
		for (size_t j = 0; j < 4; ++j) {
			EXPECT_EQ(result_neg[i][j], -m1[i][j]);
		}
	}
	auto result_double_neg = result_neg.negate();
	for (size_t i = 0; i < 4; ++i) {
		for (size_t j = 0; j < 4; ++j) {
			EXPECT_EQ(result_double_neg[i][j], m1[i][j]);
		}
	}
}

TEST(mathlib_matrix4_test_suite, m4_transpose_test) {
	mathlib::Matrix4<int> m1 = {
		{1, -2, 3, -4},
		{-5, 6, -7, 8},
		{9, -10, 11, -12},
		{-13, 14, -15, 16}
	};
	mathlib::Matrix4<int> result_transpose = m1.transpose();
	for (size_t i = 0; i < 4; ++i) {
		for (size_t j = 0; j < 4; ++j) {
			EXPECT_EQ(result_transpose[i][j], m1[j][i]);
		}
	}
}

TEST(mathlib_matrix4_test_suite, m4_det_test) {
	mathlib::Matrix4<int> m1 = {
		{1, 2, 3, 4},
		{5, 6, 7, 8},
		{9, 10, 11, 12},
		{13, 14, 15, 16}
	};
	mathlib::Matrix4<int> m2 = {
		{1, 0, 0, 0},
		{0, 1, 0, 0},
		{0, 0, 1, 0},
		{0, 0, 0, 1}
	};
	mathlib::Matrix4<int> m3 = {
		{2, -3, 1, 5},
		{4, 2, -1, -3},
		{1, 5, 3, -2},
		{3, 4, -2, 1}
	};
	EXPECT_EQ(m1.det(), 0);
	EXPECT_EQ(m2.det(), 1);
	EXPECT_EQ(m3.det(), 608);
}

TEST(mathlib_matrix4_test_suite, m4_inv_test) {
	mathlib::Matrix4<int> m1 = {
		{1, 2, 3, 4},
		{5, 6, 7, 8},
		{9, 10, 11, 12},
		{13, 14, 15, 16}
	};
	mathlib::Matrix4<int> m2 = {
		{1, 0, 0, 0},
		{0, 1, 0, 0},
		{0, 0, 1, 0},
		{0, 0, 0, 1}
	};

	EXPECT_ANY_THROW(m1.inverse()); // Singular matrix
	
	auto m2_inv = m2.inverse();
	for (size_t i = 0; i < 4; ++i) {
		for (size_t j = 0; j < 4; ++j) {
			EXPECT_EQ(m2_inv[i][j], m2[i][j]);
		}
	}
}