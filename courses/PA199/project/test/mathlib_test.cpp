#include "../mathlib/vector.hpp"
#include <gtest/gtest.h>

// Basic tests for mathlib::Vector
TEST(mathlib_vector_test_suite, v_basic_test) {
	mathlib::Vector<float, 3> v{ 1.0f, 2.0f, 3.0f };
	mathlib::Vector<int, 3> vi{ 1, 2, 3 };

	// Access via operator[]
	EXPECT_EQ(v[0], 1.0f);
	EXPECT_EQ(v[1], 2.0f);
	EXPECT_EQ(v[2], 3.0f);

	EXPECT_EQ(vi[0], 1);
	EXPECT_EQ(vi[1], 2);
	EXPECT_EQ(vi[2], 3);

    // Access via const reference
    const auto& vc = v;
    const auto& vic = vi;
    EXPECT_EQ(vc[0], 1.0f);
    EXPECT_EQ(vc[1], 2.0f);
    EXPECT_EQ(vc[2], 3.0f);

    EXPECT_EQ(vic[0], 1);
    EXPECT_EQ(vic[1], 2);
    EXPECT_EQ(vic[2], 3);

    // Size check
    EXPECT_EQ(v.size(), 3);
    EXPECT_EQ(vi.size(), 3);

    // Type check
    EXPECT_TRUE((std::is_same_v<decltype(v[0]), float&>));
    EXPECT_TRUE((std::is_same_v<decltype(vi[0]), int&>));
    EXPECT_TRUE((std::is_same_v<decltype(vc[0]), const float&>));
    EXPECT_TRUE((std::is_same_v<decltype(vic[0]), const int&>));
}

// Test for vector-and-vector addition
TEST(mathlib_vector_test_suite, vnv_add_test) {
    mathlib::Vector<float, 3> v1{ 1.0f, 2.0f, 3.0f };
    mathlib::Vector<float, 3> v2{ 4.0f, 5.0f, 6.0f };
	mathlib::Vector<float, 3> v3{ 7.0f, 8.0f, 9.0f };

    mathlib::Vector<float, 3> expected_v1v2{ 5.0f, 7.0f, 9.0f };
	mathlib::Vector<float, 3> expected_v1v2v3{ 12.0f, 15.0f, 18.0f };
    
	// Vector + Vector
    auto result_v1v2 = v1 + v2;

    EXPECT_EQ(result_v1v2.size(), expected_v1v2.size());
    for (size_t i = 0; i < result_v1v2.size(); ++i) {
        EXPECT_FLOAT_EQ(result_v1v2[i], expected_v1v2[i]);
    }

	// Commutativity check
	auto result_v2v1 = v2 + v1;

    EXPECT_EQ(result_v2v1.size(), expected_v1v2.size());
    for (size_t i = 0; i < result_v2v1.size(); ++i) {
        EXPECT_FLOAT_EQ(result_v2v1[i], expected_v1v2[i]);
	}

	// Associativity check
	auto result_a = v1 + (v2 + v3);
	auto result_b = (v1 + v2) + v3;

	EXPECT_EQ(result_a.size(), expected_v1v2v3.size());
	EXPECT_EQ(result_b.size(), expected_v1v2v3.size());

	EXPECT_EQ(result_a.size(), expected_v1v2v3.size());
    for (size_t i = 0; i < result_a.size(); ++i) {
        EXPECT_FLOAT_EQ(result_a[i], expected_v1v2v3[i]);
    }

    EXPECT_EQ(result_a.size(), result_b.size());
    for (size_t i = 0; i < result_a.size(); ++i) {
        EXPECT_FLOAT_EQ(result_a[i], result_b[i]);
    }
}

// Test for vector-and-scalar addition
TEST(mathlib_vector_test_suite, vns_add_test) {
    mathlib::Vector<float, 3> v{ 1.0f, 2.0f, 3.0f };
	float s = 5.0f;

    mathlib::Vector<float, 3> expected_vs{ 6.0f, 7.0f, 8.0f };
	mathlib::Vector<float, 3> expected_vss{ 7.0f, 9.0f, 11.0f };

    // Vector + Vector
    auto result_vs = v + s;

    EXPECT_EQ(result_vs.size(), expected_vs.size());
    for (size_t i = 0; i < result_vs.size(); ++i) {
        EXPECT_FLOAT_EQ(result_vs[i], expected_vs[i]);
    }

    // Commutativity check
    auto result_sv = s + v;

    EXPECT_EQ(result_sv.size(), expected_vs.size());
    for (size_t i = 0; i < result_sv.size(); ++i) {
        EXPECT_FLOAT_EQ(result_sv[i], expected_vs[i]);
    }

    // Associativity check
    auto result_a = v + (s + v);
    auto result_b = (v + s) + v;

    EXPECT_EQ(result_a.size(), expected_vss.size());
    EXPECT_EQ(result_b.size(), expected_vss.size());

    EXPECT_EQ(result_a.size(), expected_vss.size());
    for (size_t i = 0; i < result_a.size(); ++i) {
        EXPECT_FLOAT_EQ(result_a[i], expected_vss[i]);
    }

    EXPECT_EQ(result_a.size(), result_b.size());
    for (size_t i = 0; i < result_a.size(); ++i) {
        EXPECT_FLOAT_EQ(result_a[i], result_b[i]);
    }
}

// Test for vector-and-vector subtraction
TEST(mathlib_vector_test_suite, vnv_sub_test) {
    mathlib::Vector<float, 3> v1{ 1.0f, 2.0f, 3.0f };
    mathlib::Vector<float, 3> v2{ 4.0f, 5.0f, 6.0f };
    mathlib::Vector<float, 3> v3{ 7.0f, 8.0f, 9.0f };

    mathlib::Vector<float, 3> expected_v1v2{ -3.0f, -3.0f, -3.0f };
    mathlib::Vector<float, 3> expected_v2v1{ 3.0f, 3.0f, 3.0f };

    mathlib::Vector<float, 3> expected_v1v2v3{ 4.0f, 5.0f, 6.0f };
    mathlib::Vector<float, 3> expected_v1v2v3_b{ -10.0f, -11.0f, -12.0f };

    // Vector + Vector
    auto result_v1v2 = v1 - v2;

    EXPECT_EQ(result_v1v2.size(), expected_v1v2.size());
    for (size_t i = 0; i < result_v1v2.size(); ++i) {
        EXPECT_FLOAT_EQ(result_v1v2[i], expected_v1v2[i]);
    }

    // Non Commutativity check
    auto result_v2v1 = v2 - v1;

    EXPECT_EQ(result_v2v1.size(), expected_v2v1.size());
    for (size_t i = 0; i < result_v2v1.size(); ++i) {
        EXPECT_FLOAT_EQ(result_v2v1[i], expected_v2v1[i]);
    }

    for (size_t i = 0; i < result_v2v1.size(); ++i) {
        EXPECT_NE(result_v2v1[i], result_v1v2[i]);
    }

    // Non Associativity check
    auto result_a = v1 - (v2 - v3);
    auto result_b = (v1 - v2) - v3;

    EXPECT_EQ(result_a.size(), expected_v1v2v3.size());
    EXPECT_EQ(result_b.size(), expected_v1v2v3.size());

    EXPECT_EQ(result_a.size(), expected_v1v2v3.size());
    for (size_t i = 0; i < result_a.size(); ++i) {
        EXPECT_FLOAT_EQ(result_a[i], expected_v1v2v3[i]);
    }

    EXPECT_EQ(result_b.size(), expected_v1v2v3_b.size());
    for (size_t i = 0; i < expected_v1v2v3_b.size(); ++i) {
        EXPECT_FLOAT_EQ(result_b[i], expected_v1v2v3_b[i]);
    }
}

// Test for vector-and-scalar subtraction
TEST(mathlib_vector_test_suite, vns_sub_test) {
    mathlib::Vector<float, 3> v{ 1.0f, 2.0f, 3.0f };
    float s = 5.0f;

    mathlib::Vector<float, 3> expected_vs{ -4.0f, -3.0f, -2.0f };
    mathlib::Vector<float, 3> expected_sv{ 4.0f, 3.0f, 2.0f };

    mathlib::Vector<float, 3> expected_vss{ -3.0f, -1.0f, 1.0f };
    mathlib::Vector<float, 3> expected_vss_b{ -5.0f, -5.0f, -5.0f };

    // Vector + Vector
    auto result_vs = v - s;

    EXPECT_EQ(result_vs.size(), expected_vs.size());
    for (size_t i = 0; i < result_vs.size(); ++i) {
        EXPECT_FLOAT_EQ(result_vs[i], expected_vs[i]);
    }

    // Non Commutativity check
    auto result_sv = s - v;

    EXPECT_EQ(result_sv.size(), expected_sv.size());
    for (size_t i = 0; i < result_sv.size(); ++i) {
        EXPECT_FLOAT_EQ(result_sv[i], expected_sv[i]);
    }

    for (size_t i = 0; i < result_sv.size(); ++i) {
        EXPECT_NE(result_vs[i], result_sv[i]);
    }

    // Non Associativity check
    auto result_a = v - (s - v);
    auto result_b = (v - s) - v;

    EXPECT_EQ(result_a.size(), expected_vss.size());
    EXPECT_EQ(result_b.size(), expected_vss.size());

    EXPECT_EQ(result_a.size(), expected_vss.size());
    for (size_t i = 0; i < result_a.size(); ++i) {
        EXPECT_FLOAT_EQ(result_a[i], expected_vss[i]);
    }

    EXPECT_EQ(result_b.size(), expected_vss_b.size());
    for (size_t i = 0; i < expected_vss_b.size(); ++i) {
        EXPECT_FLOAT_EQ(result_b[i], expected_vss_b[i]);
    }
}

// Test for vector-and-vector multiplication
TEST(mathlib_vector_test_suite, vnv_mul_test) {
    mathlib::Vector<float, 3> v1{ 1.0f, 2.0f, 3.0f };
    mathlib::Vector<float, 3> v2{ 4.0f, 5.0f, 6.0f };
    mathlib::Vector<float, 3> v3{ 7.0f, 8.0f, 9.0f };

    mathlib::Vector<float, 3> expected_v1v2{ 4.0f, 10.0f, 18.0f };
    mathlib::Vector<float, 3> expected_v1v2v3{ 28.0f, 80.0f, 162.0f };

    // Vector + Vector
    auto result_v1v2 = v1 * v2;

    EXPECT_EQ(result_v1v2.size(), expected_v1v2.size());
    for (size_t i = 0; i < result_v1v2.size(); ++i) {
        EXPECT_FLOAT_EQ(result_v1v2[i], expected_v1v2[i]);
    }

    // Commutativity check
    auto result_v2v1 = v2 * v1;

    EXPECT_EQ(result_v2v1.size(), expected_v1v2.size());
    for (size_t i = 0; i < result_v2v1.size(); ++i) {
        EXPECT_FLOAT_EQ(result_v2v1[i], expected_v1v2[i]);
    }

    // Associativity check
    auto result_a = v1 * (v2 * v3);
    auto result_b = (v1 * v2) * v3;

    EXPECT_EQ(result_a.size(), expected_v1v2v3.size());
    EXPECT_EQ(result_b.size(), expected_v1v2v3.size());

    EXPECT_EQ(result_a.size(), expected_v1v2v3.size());
    for (size_t i = 0; i < result_a.size(); ++i) {
        EXPECT_FLOAT_EQ(result_a[i], expected_v1v2v3[i]);
    }

    EXPECT_EQ(result_a.size(), result_b.size());
    for (size_t i = 0; i < result_a.size(); ++i) {
        EXPECT_FLOAT_EQ(result_a[i], result_b[i]);
    }
}

// Test for vector-and-scalar multiplication
TEST(mathlib_vector_test_suite, vns_mul_test) {
    mathlib::Vector<float, 3> v{ 1.0f, 2.0f, 3.0f };
    float s = 5.0f;

    mathlib::Vector<float, 3> expected_vs{ 5.0f, 10.0f, 15.0f };
    mathlib::Vector<float, 3> expected_vss{ 5.0f, 20.0f, 45.0f };

    // Vector + Vector
    auto result_vs = v * s;

    EXPECT_EQ(result_vs.size(), expected_vs.size());
    for (size_t i = 0; i < result_vs.size(); ++i) {
        EXPECT_FLOAT_EQ(result_vs[i], expected_vs[i]);
    }

    // Commutativity check
    auto result_sv = s * v;

    EXPECT_EQ(result_sv.size(), expected_vs.size());
    for (size_t i = 0; i < result_sv.size(); ++i) {
        EXPECT_FLOAT_EQ(result_sv[i], expected_vs[i]);
    }

    // Associativity check
    auto result_a = v * (s * v);
    auto result_b = (v * s) * v;

    EXPECT_EQ(result_a.size(), expected_vss.size());
    EXPECT_EQ(result_b.size(), expected_vss.size());

    EXPECT_EQ(result_a.size(), expected_vss.size());
    for (size_t i = 0; i < result_a.size(); ++i) {
        EXPECT_FLOAT_EQ(result_a[i], expected_vss[i]);
    }

    EXPECT_EQ(result_a.size(), result_b.size());
    for (size_t i = 0; i < result_a.size(); ++i) {
        EXPECT_FLOAT_EQ(result_a[i], result_b[i]);
    }
}

// Test for vector-and-vector division
TEST(mathlib_vector_test_suite, vnv_div_test) {
    mathlib::Vector<float, 3> v1{ 1.0f, 2.0f, 3.0f };
    mathlib::Vector<float, 3> v2{ 4.0f, 5.0f, 6.0f };
    mathlib::Vector<float, 3> v3{ 7.0f, 8.0f, 9.0f };

    mathlib::Vector<float, 3> expected_v1v2{ 0.25f, 0.4f, 0.5f };
    mathlib::Vector<float, 3> expected_v2v1{ 4.0f, 2.5f, 2.0f };

    mathlib::Vector<float, 3> expected_v1v2v3{ 1.0f / (4.0f/7.0f), 2.0f / (5.0f / 8.0f), 3.0f / (6.0f / 9.0f) };
    mathlib::Vector<float, 3> expected_v1v2v3_b{ (1.0f / 4.0f) / 7.0f, (2.0f / 5.0f) / 8.0f, (3.0f / 6.0f) / 9.0f };

    // Vector + Vector
    auto result_v1v2 = v1 / v2;

    EXPECT_EQ(result_v1v2.size(), expected_v1v2.size());
    for (size_t i = 0; i < result_v1v2.size(); ++i) {
        EXPECT_FLOAT_EQ(result_v1v2[i], expected_v1v2[i]);
    }

    // Non Commutativity check
    auto result_v2v1 = v2 / v1;

    EXPECT_EQ(result_v2v1.size(), expected_v2v1.size());
    for (size_t i = 0; i < result_v2v1.size(); ++i) {
        EXPECT_FLOAT_EQ(result_v2v1[i], expected_v2v1[i]);
    }

    for (size_t i = 0; i < result_v2v1.size(); ++i) {
        EXPECT_NE(result_v2v1[i], result_v1v2[i]);
    }

    // Non Associativity check
    auto result_a = v1 / (v2 / v3);
    auto result_b = (v1 / v2) / v3;

    EXPECT_EQ(result_a.size(), expected_v1v2v3.size());
    EXPECT_EQ(result_b.size(), expected_v1v2v3.size());

    EXPECT_EQ(result_a.size(), expected_v1v2v3.size());
    for (size_t i = 0; i < result_a.size(); ++i) {
        EXPECT_FLOAT_EQ(result_a[i], expected_v1v2v3[i]);
    }

    EXPECT_EQ(result_b.size(), expected_v1v2v3_b.size());
    for (size_t i = 0; i < expected_v1v2v3_b.size(); ++i) {
        EXPECT_FLOAT_EQ(result_b[i], expected_v1v2v3_b[i]);
    }
}

// Test for vector-and-scalar division
TEST(mathlib_vector_test_suite, vns_div_test) {
    mathlib::Vector<float, 3> v{ 1.0f, 2.0f, 3.0f };
    float s = 5.0f;

    mathlib::Vector<float, 3> expected_vs{ 0.2f, 0.4f, 0.6f };
    mathlib::Vector<float, 3> expected_sv{ 5.0f, 2.5f, 1.6666666f };

    mathlib::Vector<float, 3> expected_vss{ 1.0f / (5.0f / 1.0f), 2.0f / (5.0f / 2.0f), 3.0f / (5.0f / 3.0f) };
    mathlib::Vector<float, 3> expected_vss_b{ (1.0 / 5.0f) / 1.0f, (2.0 / 5.0f) / 2.0f, (3.0 / 5.0f) / 3.0f };

    // Vector + Vector
    auto result_vs = v / s;

    EXPECT_EQ(result_vs.size(), expected_vs.size());
    for (size_t i = 0; i < result_vs.size(); ++i) {
        EXPECT_FLOAT_EQ(result_vs[i], expected_vs[i]);
    }

    // Non Commutativity check
    auto result_sv = s / v;

    EXPECT_EQ(result_sv.size(), expected_sv.size());
    for (size_t i = 0; i < result_sv.size(); ++i) {
        EXPECT_FLOAT_EQ(result_sv[i], expected_sv[i]);
    }

    for (size_t i = 0; i < result_sv.size(); ++i) {
        EXPECT_NE(result_vs[i], result_sv[i]);
    }

    // Non Associativity check
    auto result_a = v / (s / v);
    auto result_b = (v / s) / v;

    EXPECT_EQ(result_a.size(), expected_vss.size());
    EXPECT_EQ(result_b.size(), expected_vss.size());

    EXPECT_EQ(result_a.size(), expected_vss.size());
    for (size_t i = 0; i < result_a.size(); ++i) {
        EXPECT_FLOAT_EQ(result_a[i], expected_vss[i]);
    }

    EXPECT_EQ(result_b.size(), expected_vss_b.size());
    for (size_t i = 0; i < expected_vss_b.size(); ++i) {
        EXPECT_FLOAT_EQ(result_b[i], expected_vss_b[i]);
    }
}

TEST(mathlib_vector_test_suite, v_magnitude_test) {
	mathlib::Vector<float, 1> v1{ 3.0f };
	EXPECT_FLOAT_EQ(3.0f, v1.magnitude());

	mathlib::Vector<float, 2> v2{ 3.0f, 4.0f };
	EXPECT_FLOAT_EQ(5.0f, v2.magnitude());
    
    mathlib::Vector<float, 3> v3{ 3.0f, 4.0f, 0.0f };
	EXPECT_FLOAT_EQ(5.0f, v3.magnitude());

	mathlib::Vector<int, 4> v4{ 1, 2, 2, 1 };
	EXPECT_FLOAT_EQ(3, v4.magnitude()); // Rounding due to int type (EXPECTED)
}

TEST(mathlib_vector_test_suite, v_normalize_test) {
	mathlib::Vector<float, 1> v1{ 3.0f };
	auto n1 = v1.normalize();
	EXPECT_FLOAT_EQ(1.0f, n1[0]);

    mathlib::Vector<float, 2> v2{ 3.0f, 4.0f };
    auto nv2 = v2.normalize();
    EXPECT_FLOAT_EQ(0.6f, nv2[0]);
    EXPECT_FLOAT_EQ(0.8f, nv2[1]);
    EXPECT_FLOAT_EQ(1.0f, nv2.magnitude());
    
    mathlib::Vector<float, 3> v3{ 3.0f, 4.0f, 0.0f };
    auto nv3 = v3.normalize();
    EXPECT_FLOAT_EQ(0.6f, nv3[0]);
    EXPECT_FLOAT_EQ(0.8f, nv3[1]);
    EXPECT_FLOAT_EQ(0.0f, nv3[2]);
    EXPECT_FLOAT_EQ(1.0f, nv3.magnitude());
    
    mathlib::Vector<float, 4> v4{ 1, 1, 1, 1 };
    auto nv4 = v4.normalize();
    EXPECT_FLOAT_EQ(0.5f, nv4[0]);
    EXPECT_FLOAT_EQ(0.5f, nv4[1]);
    EXPECT_FLOAT_EQ(0.5f, nv4[2]);
    EXPECT_FLOAT_EQ(0.5f, nv4[3]);
	EXPECT_FLOAT_EQ(1.0f, nv4.magnitude());
}

TEST(mathlib_vector_test_suite, v_negate_test) {
    mathlib::Vector<float, 3> v{ 1.0f, -2.0f, 3.0f };
    auto n = -v;
    EXPECT_FLOAT_EQ(-1.0f, n[0]);
    EXPECT_FLOAT_EQ(2.0f, n[1]);
	EXPECT_FLOAT_EQ(-3.0f, n[2]);

	auto nn = n.negate();
    EXPECT_FLOAT_EQ(v[0], nn[0]);
    EXPECT_FLOAT_EQ(v[1], nn[1]);
	EXPECT_FLOAT_EQ(v[2], nn[2]);
}

TEST(mathlib_vector_test_suite, v_dot_test) {
    mathlib::Vector<float, 3> v1{ 1.0f, 2.0f, 3.0f };
    mathlib::Vector<float, 3> v2{ 4.0f, 5.0f, 6.0f };

    float expected_dot = 32.0f;
    float result_dot = v1.dot(v2);
    EXPECT_FLOAT_EQ(expected_dot, result_dot);

    // Commutativity check
    float result_dot_comm = v2.dot(v1);
    EXPECT_FLOAT_EQ(expected_dot, result_dot_comm);
}

TEST(mathlib_vector_test_suite, v_cross_test) {
    mathlib::Vector<float, 3> v1{ 1.0f, 2.0f, 3.0f };
    mathlib::Vector<float, 3> v2{ 4.0f, 5.0f, 6.0f };
    mathlib::Vector<float, 3> expected_cross{ -3.0f, 6.0f, -3.0f };
    
    auto result_cross = v1.cross(v2);
    EXPECT_EQ(result_cross.size(), expected_cross.size());
    for (size_t i = 0; i < result_cross.size(); ++i) {
        EXPECT_FLOAT_EQ(result_cross[i], expected_cross[i]);
    }

    // Non Commutativity check
    auto result_cross_comm = v2.cross(v1);
    EXPECT_EQ(result_cross_comm.size(), expected_cross.size());
    for (size_t i = 0; i < result_cross_comm.size(); ++i) {
        EXPECT_FLOAT_EQ(result_cross_comm[i], -expected_cross[i]);
    }
}