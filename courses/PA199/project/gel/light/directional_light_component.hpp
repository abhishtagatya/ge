#pragma once

#include "gem.hpp"
#include "light_component.hpp"

namespace gel {
	class DirectionalLightComponent : public LightComponent {
	public:
		DirectionalLightComponent() = default;

		gem::Vector<float, 3> getDirection() const {
			return getEntity().getForwardVector();
		}
	};
}