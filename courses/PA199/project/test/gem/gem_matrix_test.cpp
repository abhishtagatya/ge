#define _USE_MATH_DEFINES

#include "../../gem/matrix.hpp"
#include "../../gem/vector.hpp"
#include <cmath>
#include <gtest/gtest.h>

// Basic tests for gem::Matrix4
TEST(gem_matrix4_test_suite, m4_basic_test) {
	gem::Matrix4<int> m1 = { {1,2,3,4}, {5,6,7,8}, {9,10,11,12}, {13,14,15,16} };
	gem::Matrix4<int> m2 = { {16,15,14,13}, {12,11,10,9}, {8,7,6,5}, {4,3,2,1} };

	gem::Matrix4<float> m0 = gem::Matrix4<float>::zero();
	gem::Matrix4<float> mi = gem::Matrix4<float>::identity();

	gem::Vector<float, 3> v1 = { 1.0f, 2.0f, 3.0f };
	gem::Matrix4<float> mt = gem::Matrix4<float>::translation(v1);

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

TEST(gem_matrix4_test_suite, m4_trs_test) {
	gem::Vector<float, 3> position = { 1.0f, 2.0f, 3.0f };
	gem::Vector<float, 3> scale = { 2.0f, 2.0f, 2.0f };
	gem::Vector<float, 3> rotation = { M_PI / 4.0f, 0.0f, M_PI / 2.0f };

	gem::Matrix4<float> t = gem::Matrix4<float>::translation(position);

	EXPECT_FLOAT_EQ(t[0][3], position[0]);
	EXPECT_FLOAT_EQ(t[1][3], position[1]);
	EXPECT_FLOAT_EQ(t[2][3], position[2]);
	EXPECT_FLOAT_EQ(t[3][3], 1.0f);

	gem::Matrix4<float> s = gem::Matrix4<float>::scale(scale);

	EXPECT_FLOAT_EQ(s[0][0], scale[0]);
	EXPECT_FLOAT_EQ(s[1][1], scale[1]);
	EXPECT_FLOAT_EQ(s[2][2], scale[2]);
	EXPECT_FLOAT_EQ(s[3][3], 1.0f);

	gem::Matrix4<float> rx = gem::Matrix4<float>::rotationX(rotation[0]);

	EXPECT_FLOAT_EQ(rx[0][0], 1.0f);
	EXPECT_FLOAT_EQ(rx[1][1], cos(rotation[0]));
	EXPECT_FLOAT_EQ(rx[1][2], -sin(rotation[0]));
	EXPECT_FLOAT_EQ(rx[2][1], sin(rotation[0]));
	EXPECT_FLOAT_EQ(rx[2][2], cos(rotation[0]));
	EXPECT_FLOAT_EQ(rx[3][3], 1.0f);

	gem::Matrix4<float> ry = gem::Matrix4<float>::rotationY(rotation[1]);

	EXPECT_FLOAT_EQ(ry[0][0], cos(rotation[1]));
	EXPECT_FLOAT_EQ(ry[0][2], sin(rotation[1]));
	EXPECT_FLOAT_EQ(ry[1][1], 1.0f);
	EXPECT_FLOAT_EQ(ry[2][0], -sin(rotation[1]));
	EXPECT_FLOAT_EQ(ry[2][2], cos(rotation[1]));
	EXPECT_FLOAT_EQ(ry[3][3], 1.0f);

	gem::Matrix4<float> rz = gem::Matrix4<float>::rotationZ(rotation[2]);

	EXPECT_FLOAT_EQ(rz[0][0], cos(rotation[2]));
	EXPECT_FLOAT_EQ(rz[0][1], -sin(rotation[2]));
	EXPECT_FLOAT_EQ(rz[1][0], sin(rotation[2]));
	EXPECT_FLOAT_EQ(rz[1][1], cos(rotation[2]));
	EXPECT_FLOAT_EQ(rz[2][2], 1.0f);
	EXPECT_FLOAT_EQ(rz[3][3], 1.0f);
}

// Test for matrix-and-matrix addition
TEST(gem_matrix4_test_suite, m4nm4_add_test) {
	gem::Matrix4<int> m1 = { {1,2,3,4}, {5,6,7,8}, {9,10,11,12}, {13,14,15,16} };
	gem::Matrix4<int> m2 = { {16,15,14,13}, {12,11,10,9}, {8,7,6,5}, {4,3,2,1} };

	gem::Matrix4<int> result_m1m2 = m1 + m2;

	gem::Matrix4<int> result_m1m0 = m1 + gem::Matrix4<int>::zero();
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
	gem::Matrix4<int> m3 = { {1,1,1,1}, {1,1,1,1}, {1,1,1,1}, {1,1,1,1} };
	gem::Matrix4<int> result_a = (m1 + m2) + m3;
	gem::Matrix4<int> result_b = m1 + (m2 + m3);

	for (size_t i = 0; i < 4; ++i) {
		for (size_t j = 0; j < 4; ++j) {
			EXPECT_EQ(result_a[i][j], result_b[i][j]);
		}
	}
}

// Test for matrix-and-scalar addition
TEST(gem_matrix4_test_suite, m4ns_add_test) {
	gem::Matrix4<int> m1 = { {1,2,3,4}, {5,6,7,8}, {9,10,11,12}, {13,14,15,16} };
	int s = 10;
	
	gem::Matrix4<int> result_m1s = m1 + s;
	for (size_t i = 0; i < 4; ++i) {
		for (size_t j = 0; j < 4; ++j) {
			EXPECT_EQ(result_m1s[i][j], m1[i][j] + s);
			EXPECT_EQ(result_m1s[i][j], s + m1[i][j]);
		}
	}

	auto result_m0s = gem::Matrix4<int>::zero() + s;
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
TEST(gem_matrix4_test_suite, m4nm4_sub_test) {
	gem::Matrix4<int> m1 = { {1,2,3,4}, {5,6,7,8}, {9,10,11,12}, {13,14,15,16} };
	gem::Matrix4<int> m2 = { {16,15,14,13}, {12,11,10,9}, {8,7,6,5}, {4,3,2,1} };

	gem::Matrix4<int> result_m1m2 = m1 - m2;
	gem::Matrix4<int> result_m2m1 = m2 - m1;

	gem::Matrix4<int> result_m1m0 = m1 - gem::Matrix4<int>::zero();
	gem::Matrix4<int> result_m0m1 = gem::Matrix4<int>::zero() - m1;
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
	gem::Matrix4<int> m3 = { {1,1,1,1}, {1,1,1,1}, {1,1,1,1}, {1,1,1,1} };
	gem::Matrix4<int> result_a = (m1 - m2) - m3;
	gem::Matrix4<int> result_b = m1 - (m2 - m3);

	gem::Matrix4<int> expected_a = { {-16, -14, -12, -10}, {-8, -6, -4, -2}, {0,2,4,6}, {8,10,12,14} };
	gem::Matrix4<int> expected_b = { { -14, -12, -10, -8}, {-6, -4, -2, 0}, {2,4,6,8}, {10,12,14,16} };

	for (size_t i = 0; i < 4; ++i) {
		for (size_t j = 0; j < 4; ++j) {
			EXPECT_EQ(result_a[i][j], expected_a[i][j]);
			EXPECT_EQ(result_b[i][j], expected_b[i][j]);
		}
	}
}

// Test for matrix-and-scalar subtraction
TEST(gem_matrix4_test_suite, m4ns_sub_test) {
	gem::Matrix4<int> m1 = { {1,2,3,4}, {5,6,7,8}, {9,10,11,12}, {13,14,15,16} };
	int s = 10;

	gem::Matrix4<int> result_m1s = m1 - s;
	gem::Matrix4<int> result_sm1 = s - m1;


	for (size_t i = 0; i < 4; ++i) {
		for (size_t j = 0; j < 4; ++j) {
			EXPECT_EQ(result_m1s[i][j], m1[i][j] - s);
			EXPECT_EQ(result_sm1[i][j], s - m1[i][j]);
		}
	}

	auto result_m0s = gem::Matrix4<int>::zero() - s;
	auto result_sm0 = s - gem::Matrix4<int>::zero();

	for (size_t i = 0; i < 4; ++i) {
		for (size_t j = 0; j < 4; ++j) {
			EXPECT_EQ(result_m0s[i][j], -s);
			EXPECT_EQ(result_sm0[i][j], s);
		}
	}

	// Non Associativity check
	auto result_a = (m1 - s) - s;
	auto result_b = m1 - (s - s);

	gem::Matrix4<int> expected_a = { {-19, -18, -17, -16}, {-15, -14, -13, -12}, {-11, -10, -9, -8}, {-7, -6, -5, -4} };
	gem::Matrix4<int> expected_b = m1;

	for (size_t i = 0; i < 4; ++i) {
		for (size_t j = 0; j < 4; ++j) {
			EXPECT_EQ(result_a[i][j], expected_a[i][j]);
			EXPECT_EQ(result_b[i][j], expected_b[i][j]);
		}
	}
}

// Test for matrix-and-matrix multiplication
TEST(gem_matrix4_test_suite, m4nm4_mul_test) {
	gem::Matrix4<int> m1 = {{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}, {13, 14, 15, 16}};
	gem::Matrix4<int> m2 = {{16, 15, 14, 13}, {12, 11, 10, 9}, {8, 7, 6, 5}, {4, 3, 2, 1}};

	gem::Matrix4<int> result_m1m2 = m1 * m2;
	gem::Matrix4<int> result_m2m1 = m2 * m1;

	gem::Matrix4<int> expected_m1m2 = {
		{80, 70, 60, 50},
		{240, 214, 188, 162},
		{400, 358, 316, 274},
		{560, 502, 444, 386}
	};

	gem::Matrix4<int> expected_m2m1 = {
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

	auto result_m10 = m1 * gem::Matrix4<int>::zero();
	for (size_t i = 0; i < 4; ++i) {
		for (size_t j = 0; j < 4; ++j) {
			EXPECT_EQ(result_m10[i][j], 0);
		}
	}

	auto result_m1i = m1 * gem::Matrix4<int>::identity();
	for (size_t i = 0; i < 4; ++i) {
		for (size_t j = 0; j < 4; ++j) {
			EXPECT_EQ(result_m1i[i][j], m1[i][j]);
		}
	}
}

// Test for matrix-and-scalar multiplication
TEST(gem_matrix4_test_suite, m4ns_mul_test) {
	gem::Matrix4<int> m1 = {
		{1, 2, 3, 4},
		{5, 6, 7, 8},
		{9, 10, 11, 12},
		{13, 14, 15, 16}
	};
	int s = 2;

	gem::Matrix4<int> result_m1s = m1 * s;
	gem::Matrix4<int> result_sm1 = s * m1;

	for (size_t i = 0; i < 4; ++i) {
		for (size_t j = 0; j < 4; ++j) {
			EXPECT_EQ(result_m1s[i][j], m1[i][j] * s);
			EXPECT_EQ(result_sm1[i][j], s * m1[i][j]);
			EXPECT_EQ(result_m1s[i][j], result_sm1[i][j]);
		}
	}

	gem::Matrix4<int> result_m10 = m1 * 0;
	for (size_t i = 0; i < 4; ++i) {
		for (size_t j = 0; j < 4; ++j) {
			EXPECT_EQ(result_m10[i][j], 0);
		}
	}

	gem::Matrix4<int> result_m11 = m1 * 1;
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

TEST(gem_matrix4_test_suite, m4_negate_test) {
	gem::Matrix4<int> m1 = {
		{1, -2, 3, -4},
		{-5, 6, -7, 8},
		{9, -10, 11, -12},
		{-13, 14, -15, 16}
	};
	gem::Matrix4<int> result_neg = -m1;
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

TEST(gem_matrix4_test_suite, m4_transpose_test) {
	gem::Matrix4<int> m1 = {
		{1, -2, 3, -4},
		{-5, 6, -7, 8},
		{9, -10, 11, -12},
		{-13, 14, -15, 16}
	};
	gem::Matrix4<int> result_transpose = m1.transpose();
	for (size_t i = 0; i < 4; ++i) {
		for (size_t j = 0; j < 4; ++j) {
			EXPECT_EQ(result_transpose[i][j], m1[j][i]);
		}
	}
}

TEST(gem_matrix4_test_suite, m4_det_test) {
	gem::Matrix4<int> m1 = {
		{1, 2, 3, 4},
		{5, 6, 7, 8},
		{9, 10, 11, 12},
		{13, 14, 15, 16}
	};
	gem::Matrix4<int> m2 = {
		{1, 0, 0, 0},
		{0, 1, 0, 0},
		{0, 0, 1, 0},
		{0, 0, 0, 1}
	};
	gem::Matrix4<int> m3 = {
		{2, -3, 1, 5},
		{4, 2, -1, -3},
		{1, 5, 3, -2},
		{3, 4, -2, 1}
	};
	EXPECT_EQ(m1.det(), 0);
	EXPECT_EQ(m2.det(), 1);
	EXPECT_EQ(m3.det(), 608);
}

TEST(gem_matrix4_test_suite, m4_inv_test) {
	gem::Matrix4<int> m1 = {
		{1, 2, 3, 4},
		{5, 6, 7, 8},
		{9, 10, 11, 12},
		{13, 14, 15, 16}
	};
	gem::Matrix4<int> m2 = {
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