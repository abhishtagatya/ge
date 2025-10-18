#include <gtest/gtest.h>

#include "../../gel/game_entity.hpp"
#include "../../gel/game_scene.hpp"

TEST(gel_game_scene_test_suite, gs_basic_test) {
	gel::GameScene mainScene;

	gel::GameEntity e1;
	mainScene.addEntity(&e1);

	EXPECT_EQ(mainScene.getEntitySize(), 1);
	EXPECT_EQ(mainScene.getEntities()[0], &e1);

	gel::GameEntity e2;
	mainScene.addEntity(&e2);

	EXPECT_EQ(mainScene.getEntitySize(), 2);
	EXPECT_EQ(mainScene.getEntities()[1], &e2);

	mainScene.removeEntity(&e1);

	EXPECT_EQ(mainScene.getEntitySize(), 1);
	EXPECT_EQ(mainScene.getEntities()[0], &e2);
}