#include <gtest/gtest.h>

#include "../../gel/test_component.hpp"
#include "../../gel/game_component.hpp"
#include "../../gel/game_entity.hpp"

TEST(gel_game_component_test_suite, gc_basic_test) {
	gel::GameEntity e;

	e.addComponent(std::make_unique<gel::TestComponent>());

	EXPECT_FLOAT_EQ(e.getComponents().size(), 1);

	auto* comp = e.getComponents().front();
	ASSERT_NE(comp, nullptr);

	auto* testComp = dynamic_cast<gel::TestComponent*>(comp);
	EXPECT_NE(testComp, nullptr) << "Component is not a TestComponent instance";

	EXPECT_EQ(testComp->getEntity(), &e);

	e.removeComponent(testComp);
	EXPECT_FLOAT_EQ(e.getComponents().size(), 0);
	EXPECT_EQ(testComp->getEntity(), nullptr);
}