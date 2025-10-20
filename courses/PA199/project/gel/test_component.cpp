#include "test_component.hpp"

#include <iostream>

namespace gel {
	void TestComponent::update(float delta_time) {
		std::cout << "TestComponent update: " << delta_time << std::endl;
	}
}