#pragma once

#include "gem.hpp"
#include "light_component.hpp"

namespace gel {
	class DirectionalLightComponent : public LightComponent {
	public:
		DirectionalLightComponent(
			gem::Vector<float, 3> ambient,
			gem::Vector<float, 3> diffuse,
			gem::Vector<float, 3> specular)
			: LightComponent(ambient, diffuse, specular) {
		};

		gem::Vector<float, 3> getDirection() const {
			return getEntity()->getForwardVector();
		}
	};
}