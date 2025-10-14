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