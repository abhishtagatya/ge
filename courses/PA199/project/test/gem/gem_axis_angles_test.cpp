#define _USE_MATH_DEFINES

#include <cmath>
#include "../../gem/vector.hpp"
#include "../../gem/quaternion.hpp"
#include "../../gem/axis_angle.hpp"
#include <gtest/gtest.h>

TEST(gem_axis_angle_test_suite, aa_toq_test) {
	gem::AxisAngle<float> aa(M_PI / 2.0f, 1.0f, 0.0f, 0.0f);
	gem::Quaternion q = aa.toQuaternion();

	EXPECT_NEAR(q.w(), std::cos(M_PI / 4), 1e-6);
	EXPECT_NEAR(q.x(), std::sin(M_PI / 4), 1e-6);
	EXPECT_NEAR(q.y(), 0, 1e-6);
	EXPECT_NEAR(q.z(), 0, 1e-6);
}

TEST(gem_axis_angle_test_suite, q_toaa_test) {
	gem::Quaternion<float> q(std::cos(M_PI / 4), std::sin(M_PI / 4), 0.0f, 0.0f);
	gem::AxisAngle<float> aa = q.toAxisAngle();

	EXPECT_NEAR(aa.angle(), M_PI / 2.0f, 1e-6);
	EXPECT_NEAR(aa.axis()[0], 1.0f, 1e-6);
	EXPECT_NEAR(aa.axis()[1], 0.0f, 1e-6);
	EXPECT_NEAR(aa.axis()[2], 0.0f, 1e-6);
}