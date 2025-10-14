#include "../../gem/vector.hpp"
#include "../../gem/axis_angle.hpp"
#include "../../gem/quaternion.hpp"
#include "../../gem/interpolation.hpp"
#include <gtest/gtest.h>
#include <cmath>

// Basic tests for gem::Quaternion
TEST(gem_quaternion_test_suite, q_basic_test) {
	gem::Quaternion<float> q1;

	EXPECT_FLOAT_EQ(q1[0], 1.0f);
	EXPECT_FLOAT_EQ(q1[1], 0.0f);
	EXPECT_FLOAT_EQ(q1[2], 0.0f);
	EXPECT_FLOAT_EQ(q1[3], 0.0f);

	EXPECT_FLOAT_EQ(q1.w(), 1.0f);
	EXPECT_FLOAT_EQ(q1.x(), 0.0f);
	EXPECT_FLOAT_EQ(q1.y(), 0.0f);
	EXPECT_FLOAT_EQ(q1.z(), 0.0f);

	gem::Quaternion<float> q2(0.0f, 1.0f, 2.0f, 3.0f);
	EXPECT_FLOAT_EQ(q2[0], 0.0f);
	EXPECT_FLOAT_EQ(q2[1], 1.0f);
	EXPECT_FLOAT_EQ(q2[2], 2.0f);
	EXPECT_FLOAT_EQ(q2[3], 3.0f);

	EXPECT_FLOAT_EQ(q2.w(), 0.0f);
	EXPECT_FLOAT_EQ(q2.x(), 1.0f);
	EXPECT_FLOAT_EQ(q2.y(), 2.0f);
	EXPECT_FLOAT_EQ(q2.z(), 3.0f);

	gem::Vector<float, 4> v = { 4.0f, 5.0f, 6.0f, 7.0f };
	gem::Quaternion<float> q3(v);

	EXPECT_FLOAT_EQ(q3[0], 4.0f);
	EXPECT_FLOAT_EQ(q3[1], 5.0f);
	EXPECT_FLOAT_EQ(q3[2], 6.0f);
	EXPECT_FLOAT_EQ(q3[3], 7.0f);
}

// Test for quaternion-and-quaternon addition
TEST(gem_quaternion_test_suite, qnq_add_test) {
	gem::Quaternion<float> q1(1.0f, 2.0f, 3.0f, 4.0f);
	gem::Quaternion<float> q2(5.0f, 6.0f, 7.0f, 8.0f);

	// Quaternion + Quaternion
	auto result_q1q2 = q1 + q2;

	EXPECT_FLOAT_EQ(result_q1q2.w(), 6.0f);
	EXPECT_FLOAT_EQ(result_q1q2.x(), 8.0f);
	EXPECT_FLOAT_EQ(result_q1q2.y(), 10.0f);
	EXPECT_FLOAT_EQ(result_q1q2.z(), 12.0f);

	// Commutative check
	auto result_q2q1 = q2 + q1;

	EXPECT_FLOAT_EQ(result_q2q1.w(), result_q1q2.w());
	EXPECT_FLOAT_EQ(result_q2q1.x(), result_q1q2.x());
	EXPECT_FLOAT_EQ(result_q2q1.y(), result_q1q2.y());
	EXPECT_FLOAT_EQ(result_q2q1.z(), result_q1q2.z());

	// Associativty check
	auto result_a = (q1 + q2) + q2;
	auto result_b = q1 + (q2 + q2);

	EXPECT_FLOAT_EQ(result_a.w(), result_b.w());
	EXPECT_FLOAT_EQ(result_a.x(), result_b.x());
	EXPECT_FLOAT_EQ(result_a.y(), result_b.y());
	EXPECT_FLOAT_EQ(result_a.z(), result_b.z());
}

// Test for quaternion-and-scalar addition
TEST(gem_quaternion_test_suite, qns_add_test) {
	gem::Quaternion<float> q1(1.0f, 2.0f, 3.0f, 4.0f);
	float s = 5.0f;

	// Quaternion + Quaternion
	auto result_q1s = q1 + s;

	EXPECT_FLOAT_EQ(q1.w() + s, result_q1s.w());
	EXPECT_FLOAT_EQ(q1.x() + s, result_q1s.x());
	EXPECT_FLOAT_EQ(q1.y() + s, result_q1s.y());
	EXPECT_FLOAT_EQ(q1.z() + s, result_q1s.z());

	// Commutative check
	auto result_sq1 = s + q1;

	EXPECT_FLOAT_EQ(result_sq1.w(), result_q1s.w());
	EXPECT_FLOAT_EQ(result_sq1.x(), result_q1s.x());
	EXPECT_FLOAT_EQ(result_sq1.y(), result_q1s.y());
	EXPECT_FLOAT_EQ(result_sq1.z(), result_q1s.z());


	// Associativty check
	auto result_a = (q1 + s) + s;
	auto result_b = q1 + (s + s);

	EXPECT_FLOAT_EQ(result_a.w(), result_b.w());
	EXPECT_FLOAT_EQ(result_a.x(), result_b.x());
	EXPECT_FLOAT_EQ(result_a.y(), result_b.y());
	EXPECT_FLOAT_EQ(result_a.z(), result_b.z());
}

// Test for quaternion-and-quaternion subtraction
TEST(gem_quaternion_test_suite, qnq_sub_test) {

	gem::Quaternion<float> q1(1.0f, 2.0f, 3.0f, 4.0f);
	gem::Quaternion<float> q2(5.0f, 6.0f, 7.0f, 8.0f);
	gem::Quaternion<float> q3(9.0f, 10.0f, 11.0f, 12.0f);

	gem::Quaternion<float> expected_q1q2{ -4.0f, -4.0f, -4.0f, -4.0f };
	gem::Quaternion<float> expected_q2q1{ 4.0f, 4.0f, 4.0f, 4.0f };


	gem::Quaternion<float> expected_q1q2q3{ 5.0f, 6.0f, 7.0f, 8.0f };
	gem::Quaternion<float> expected_q1q2q3_b{ -13.0f, -14.0f, -15.0f, -16.0f };

	// Quaternion - Quaternion
	auto result_q1q2 = q1 - q2;

	EXPECT_FLOAT_EQ(result_q1q2.w(), expected_q1q2.w());
	EXPECT_FLOAT_EQ(result_q1q2.x(), expected_q1q2.x());
	EXPECT_FLOAT_EQ(result_q1q2.y(), expected_q1q2.y());
	EXPECT_FLOAT_EQ(result_q1q2.z(), expected_q1q2.z());

	// Non Commutativity check
	auto result_q2q1 = q2 - q1;

	EXPECT_FLOAT_EQ(result_q2q1.w(), expected_q2q1.w());
	EXPECT_FLOAT_EQ(result_q2q1.x(), expected_q2q1.x());
	EXPECT_FLOAT_EQ(result_q2q1.y(), expected_q2q1.y());
	EXPECT_FLOAT_EQ(result_q2q1.z(), expected_q2q1.z());

	// Non Associativity check
	auto result_a = q1 - (q2 - q3);
	auto result_b = (q1 - q2) - q3;

	EXPECT_FLOAT_EQ(result_a.w(), expected_q1q2q3.w());
	EXPECT_FLOAT_EQ(result_a.x(), expected_q1q2q3.x());
	EXPECT_FLOAT_EQ(result_a.y(), expected_q1q2q3.y());
	EXPECT_FLOAT_EQ(result_a.z(), expected_q1q2q3.z());

	EXPECT_FLOAT_EQ(result_b.w(), expected_q1q2q3_b.w());
	EXPECT_FLOAT_EQ(result_b.x(), expected_q1q2q3_b.x());
	EXPECT_FLOAT_EQ(result_b.y(), expected_q1q2q3_b.y());
	EXPECT_FLOAT_EQ(result_b.z(), expected_q1q2q3_b.z());
}

// Test for quaternion-and-scalar subtraction
TEST(gem_quaternion_test_suite, qns_sub_test) {

	gem::Quaternion<float> q1(1.0f, 2.0f, 3.0f, 4.0f);
	float s = 5.0f;

	gem::Quaternion<float> expected_q1s{ -4.0f, -3.0f, -2.0f, -1.0f };
	gem::Quaternion<float> expected_sq1{ 4.0f, 3.0f, 2.0f, 1.0f };


	gem::Quaternion<float> expected_q1ss{ 1.0f, 2.0f, 3.0f, 4.0f };
	gem::Quaternion<float> expected_q1ss_b{ -9.0f, -8.0f, -7.0f, -6.0f };

	// Quaternion - Scalar
	auto result_q1s = q1 - s;

	EXPECT_FLOAT_EQ(result_q1s.w(), expected_q1s.w());
	EXPECT_FLOAT_EQ(result_q1s.x(), expected_q1s.x());
	EXPECT_FLOAT_EQ(result_q1s.y(), expected_q1s.y());
	EXPECT_FLOAT_EQ(result_q1s.z(), expected_q1s.z());

	// Non Commutativity check
	auto result_sq1 = s - q1;

	EXPECT_FLOAT_EQ(result_sq1.w(), expected_sq1.w());
	EXPECT_FLOAT_EQ(result_sq1.x(), expected_sq1.x());
	EXPECT_FLOAT_EQ(result_sq1.y(), expected_sq1.y());
	EXPECT_FLOAT_EQ(result_sq1.z(), expected_sq1.z());

	// Non Associativity check
	auto result_a = q1 - (s - s);
	auto result_b = (q1 - s) - s;

	EXPECT_FLOAT_EQ(result_a.w(), expected_q1ss.w());
	EXPECT_FLOAT_EQ(result_a.x(), expected_q1ss.x());
	EXPECT_FLOAT_EQ(result_a.y(), expected_q1ss.y());
	EXPECT_FLOAT_EQ(result_a.z(), expected_q1ss.z());

	EXPECT_FLOAT_EQ(result_b.w(), expected_q1ss_b.w());
	EXPECT_FLOAT_EQ(result_b.x(), expected_q1ss_b.x());
	EXPECT_FLOAT_EQ(result_b.y(), expected_q1ss_b.y());
	EXPECT_FLOAT_EQ(result_b.z(), expected_q1ss_b.z());
}

// Test for quaternion-and-quaternion multiplication
TEST(gem_quaternion_test_suite, qnq_mul_test) {

	gem::Quaternion<float> q1(1.0f, 2.0f, 3.0f, 4.0f);
	gem::Quaternion<float> q2(5.0f, 6.0f, 7.0f, 8.0f);
	gem::Quaternion<float> q3(9.0f, 10.0f, 11.0f, 12.0f);

	gem::Quaternion<float> expected_q1q2{ -60.0f, 12.0f, 30.0f, 24.0f };
	gem::Quaternion<float> expected_q2q1{ -60.0f, 20.0f, 14.0f, 32.0f };


	gem::Quaternion<float> expected_q1q2q3{ 5.0f, 6.0f, 7.0f, 8.0f };
	gem::Quaternion<float> expected_q1q2q3_b{ -13.0f, -14.0f, -15.0f, -16.0f };

	// Quaternion * Quaternion
	auto result_q1q2 = q1 * q2;

	EXPECT_FLOAT_EQ(result_q1q2.w(), expected_q1q2.w());
	EXPECT_FLOAT_EQ(result_q1q2.x(), expected_q1q2.x());
	EXPECT_FLOAT_EQ(result_q1q2.y(), expected_q1q2.y());
	EXPECT_FLOAT_EQ(result_q1q2.z(), expected_q1q2.z());

	// Non Commutativity check
	auto result_q2q1 = q2 * q1;

	EXPECT_FLOAT_EQ(result_q2q1.w(), expected_q2q1.w());
	EXPECT_FLOAT_EQ(result_q2q1.x(), expected_q2q1.x());
	EXPECT_FLOAT_EQ(result_q2q1.y(), expected_q2q1.y());
	EXPECT_FLOAT_EQ(result_q2q1.z(), expected_q2q1.z());
}

// Test for quaternion-and-scalar multiplication
TEST(gem_quaternion_test_suite, qns_mul_test) {
	gem::Quaternion<float> q1(1.0f, 2.0f, 3.0f, 4.0f);
	float s = 5.0f;

	// Quaternion + Quaternion
	auto result_q1s = q1 * s;

	EXPECT_FLOAT_EQ(q1.w() * s, result_q1s.w());
	EXPECT_FLOAT_EQ(q1.x() * s, result_q1s.x());
	EXPECT_FLOAT_EQ(q1.y() * s, result_q1s.y());
	EXPECT_FLOAT_EQ(q1.z() * s, result_q1s.z());

	// Commutative check
	auto result_sq1 = s * q1;

	EXPECT_FLOAT_EQ(result_sq1.w(), result_q1s.w());
	EXPECT_FLOAT_EQ(result_sq1.x(), result_q1s.x());
	EXPECT_FLOAT_EQ(result_sq1.y(), result_q1s.y());
	EXPECT_FLOAT_EQ(result_sq1.z(), result_q1s.z());


	// Associativty check
	auto result_a = (q1 * s) * s;
	auto result_b = q1 * (s * s);

	EXPECT_FLOAT_EQ(result_a.w(), result_b.w());
	EXPECT_FLOAT_EQ(result_a.x(), result_b.x());
	EXPECT_FLOAT_EQ(result_a.y(), result_b.y());
	EXPECT_FLOAT_EQ(result_a.z(), result_b.z());
}

TEST(gem_quaternion_test_suite, q_conjugate_test) {
	gem::Quaternion<float> q1(1.0f, 2.0f, 3.0f, 4.0f);
	gem::Quaternion<float> q_conj = q1.conjugate();
	EXPECT_FLOAT_EQ(q_conj.w(), 1.0f);
	EXPECT_FLOAT_EQ(q_conj.x(), -2.0f);
	EXPECT_FLOAT_EQ(q_conj.y(), -3.0f);
	EXPECT_FLOAT_EQ(q_conj.z(), -4.0f);

	auto q_conj_conj = ~q_conj;
	EXPECT_FLOAT_EQ(q_conj_conj.w(), q1.w());
	EXPECT_FLOAT_EQ(q_conj_conj.x(), q1.x());
	EXPECT_FLOAT_EQ(q_conj_conj.y(), q1.y());
	EXPECT_FLOAT_EQ(q_conj_conj.z(), q1.z());
}

TEST(gem_quaternion_test_suite, q_magnitude_test) {
	gem::Quaternion<float> q1(1.0f, 1.0f, 1.0f, 1.0f);
	float mag = q1.magnitude();

	EXPECT_FLOAT_EQ(mag, 2.0f);

	gem::Quaternion<float> q2(0.0f, 0.0f, 0.0f, 0.0f);
	float mag2 = q2.magnitude();

	EXPECT_FLOAT_EQ(mag2, 0.0f);
}

TEST(gem_quaternion_test_suite, q_normalize_test) {
	gem::Quaternion<float> q1(1.0f, 1.0f, 1.0f, 1.0f);
	auto n = q1.normalize();
	EXPECT_FLOAT_EQ(0.5f, n[0]);
	EXPECT_FLOAT_EQ(0.5f, n[1]);
	EXPECT_FLOAT_EQ(0.5f, n[2]);
	EXPECT_FLOAT_EQ(0.5f, n[3]);
	EXPECT_FLOAT_EQ(1.0f, n.magnitude());
}

TEST(gem_quaternion_test_suite, q_inverse_test) {
	gem::Quaternion<float> q1(1.0f, 2.0f, 3.0f, 4.0f);
	auto q1_inv = q1.inverse();
	auto identity = q1 * q1_inv;
	EXPECT_FLOAT_EQ(identity.w(), 1.0f);
	EXPECT_FLOAT_EQ(identity.x(), 0.0f);
	EXPECT_FLOAT_EQ(identity.y(), 0.0f);
	EXPECT_FLOAT_EQ(identity.z(), 0.0f);
}

TEST(gem_quaternion_test_suite, q_dot_test) {
	gem::Quaternion<float> q1(1.0f, 2.0f, 3.0f, 4.0f);
	gem::Quaternion<float> q2(5.0f, 6.0f, 7.0f, 8.0f);

	float result_q1q2 = q1.dot(q2);
	float result_q2q1 = q2.dot(q1);

	EXPECT_FLOAT_EQ(result_q1q2, result_q2q1);
}

TEST(gem_quaternion_test_suite, q_lerp_test) {
	gem::Quaternion<float> q1(1.0f, 0.0f, 0.0f, 0.0f);
	gem::Quaternion<float> q2(0.0f, 1.0f, 0.0f, 0.0f);

	auto q_lerp_0 = lerp(q1, q2, 0.0f);
	EXPECT_FLOAT_EQ(q_lerp_0.w(), q1.w());
	EXPECT_FLOAT_EQ(q_lerp_0.x(), q1.x());
	EXPECT_FLOAT_EQ(q_lerp_0.y(), q1.y());
	EXPECT_FLOAT_EQ(q_lerp_0.z(), q1.z());

	auto q_lerp_1 = lerp(q1, q2, 1.0f);
	EXPECT_FLOAT_EQ(q_lerp_1.w(), q2.w());
	EXPECT_FLOAT_EQ(q_lerp_1.x(), q2.x());
	EXPECT_FLOAT_EQ(q_lerp_1.y(), q2.y());
	EXPECT_FLOAT_EQ(q_lerp_1.z(), q2.z());
}

TEST(gem_quaternion_test_suite, q_slerp_test) {
	gem::Quaternion<float> q1(1.0f, 0.0f, 0.0f, 0.0f);
	gem::Quaternion<float> q2(0.0f, 1.0f, 0.0f, 0.0f);

	auto q_slerp_0 = slerp(q1, q2, 0.0f);
	EXPECT_FLOAT_EQ(q_slerp_0.w(), q1.w());
	EXPECT_FLOAT_EQ(q_slerp_0.x(), q1.x());
	EXPECT_FLOAT_EQ(q_slerp_0.y(), q1.y());
	EXPECT_FLOAT_EQ(q_slerp_0.z(), q1.z());

	auto q_slerp_1 = slerp(q1, q2, 1.0f);
	EXPECT_FLOAT_EQ(q_slerp_1.w(), q2.w());
	EXPECT_FLOAT_EQ(q_slerp_1.x(), q2.x());
	EXPECT_FLOAT_EQ(q_slerp_1.y(), q2.y());
	EXPECT_FLOAT_EQ(q_slerp_1.z(), q2.z());

	auto q_slerp_05 = slerp(q1, q2, 0.5f);
	EXPECT_NEAR(q_slerp_05.w(), std::sqrt(2) / 2.0f, 1e-6f);
	EXPECT_NEAR(q_slerp_05.x(), std::sqrt(2) / 2.0f, 1e-6f);
	EXPECT_NEAR(q_slerp_05.y(), 0.0f, 1e-6f);
	EXPECT_NEAR(q_slerp_05.z(), 0.0f, 1e-6f);
}