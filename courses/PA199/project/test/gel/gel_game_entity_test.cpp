#define _USE_MATH_DEFINES
#include <cmath>
#include <gtest/gtest.h>

#include "gem.hpp"
#include "../../gel/game_entity.hpp"
#include "../../gel/test_component.hpp"


TEST(gel_game_entity_test_suite, gent_basic_test) {
	gel::GameEntity e1;

	EXPECT_FLOAT_EQ(e1.getPosition()[0], 0.0f);
	EXPECT_FLOAT_EQ(e1.getPosition()[1], 0.0f);
	EXPECT_FLOAT_EQ(e1.getPosition()[2], 0.0f);

	EXPECT_FLOAT_EQ(e1.getOrientation()[0], 0.0f);
	EXPECT_FLOAT_EQ(e1.getOrientation()[1], 0.0f);
	EXPECT_FLOAT_EQ(e1.getOrientation()[2], 0.0f);

	EXPECT_FLOAT_EQ(e1.getScale()[0], 1.0f);
	EXPECT_FLOAT_EQ(e1.getScale()[1], 1.0f);
	EXPECT_FLOAT_EQ(e1.getScale()[2], 1.0f);

	EXPECT_EQ(e1.getParent(), nullptr);
	EXPECT_EQ(e1.getChildren().size(), 0);

	gel::GameEntity e2(
		gem::Vector<float, 3>{ 1.0f, 2.0f, 3.0f },
		gem::Vector<float, 3>{ M_PI, M_PI / 2.0f, M_PI / 4.0f },
		gem::Vector<float, 3>{ 0.5f, 0.5f, 0.5f }
	);

	e1.addChild(&e2);
	e2.addComponent(new gel::TestComponent());

	EXPECT_FLOAT_EQ(e2.getPosition()[0], 1.0f);
	EXPECT_FLOAT_EQ(e2.getPosition()[1], 2.0f);
	EXPECT_FLOAT_EQ(e2.getPosition()[2], 3.0f);

	EXPECT_FLOAT_EQ(e2.getOrientation()[0], M_PI);
	EXPECT_FLOAT_EQ(e2.getOrientation()[1], M_PI / 2.0f);
	EXPECT_FLOAT_EQ(e2.getOrientation()[2], M_PI / 4.0f);

	EXPECT_FLOAT_EQ(e2.getScale()[0], 0.5f);
	EXPECT_FLOAT_EQ(e2.getScale()[1], 0.5f);
	EXPECT_FLOAT_EQ(e2.getScale()[2], 0.5f);

	EXPECT_EQ(e2.getParent(), &e1);
	EXPECT_EQ(e1.getChildren().size(), 1);
	EXPECT_EQ(e1.getChildren()[0], &e2);
	EXPECT_EQ(e2.getComponents().size(), 1);
}

TEST(gel_game_entity_test_suite, gent_ls_test) {
	gem::Vector<float, 3> position = { 1.0f, 2.0f, 3.0f };
	gem::Vector<float, 3> scale = { 2.0f, 2.0f, 2.0f };
	gem::Vector<float, 3> rotation = { M_PI / 4.0f, 0.0f, M_PI / 2.0f };

	gel::GameEntity e(
		position,
		rotation,
		scale
	);

	gem::Matrix4<float> ls = e.getLocalTransform();

	float cx = cos(rotation[0]);
	float sx = sin(rotation[0]);
	float cy = cos(rotation[1]);
	float sy = sin(rotation[1]);
	float cz = cos(rotation[2]);
	float sz = sin(rotation[2]);

	gem::Matrix4<float> expected;

	expected[0][0] = cy * cz * scale[0];
	expected[0][1] = (cz * sx * sy - cx * sz) * scale[1];
	expected[0][2] = (sx * sz + cx * cz * sy) * scale[2];
	expected[0][3] = position[0];

	expected[1][0] = cy * sz * scale[0];
	expected[1][1] = (cx * cz + sx * sy * sz) * scale[1];
	expected[1][2] = (cx * sy * sz - cz * sx) * scale[2];
	expected[1][3] = position[1];

	expected[2][0] = -sy * scale[0];
	expected[2][1] = cy * sx * scale[1];
	expected[2][2] = cx * cy * scale[2];
	expected[2][3] = position[2];

	expected[3][0] = 0.0f;
	expected[3][1] = 0.0f;
	expected[3][2] = 0.0f;
	expected[3][3] = 1.0f;

	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			EXPECT_NEAR(ls[i][j], expected[i][j], 1e-5)
				<< "Mismatch at element [" << i << "][" << j << "]";
		}
	}
}

TEST(gel_game_entity_test_suite, gent_ws_test) {
	gem::Vector<float, 3> position = { 1.0f, 2.0f, 3.0f };
	gem::Vector<float, 3> scale = { 2.0f, 2.0f, 2.0f };
	gem::Vector<float, 3> rotation = { M_PI / 4.0f, 0.0f, M_PI / 2.0f };

	gel::GameEntity e(
		position,
		rotation,
		scale
	);

	gel::GameEntity e2(
		gem::Vector<float, 3>{ 1.0f, 1.0f, 1.0f },
		gem::Vector<float, 3>{ 0.0f, 0.0f, 0.0f },
		gem::Vector<float, 3>{ 2.0f, 2.0f, 2.0f }
	);

	e2.addChild(&e);

	gem::Matrix4<float> ews = e.getWorldTransform();
	gem::Matrix4<float> pws = e2.getLocalTransform() * e.getLocalTransform();

	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			EXPECT_NEAR(ews[i][j], pws[i][j], 1e-5)
				<< "Mismatch at element [" << i << "][" << j << "]";
		}
	}
}
