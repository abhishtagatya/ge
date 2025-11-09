#pragma once

#include "gem.hpp"
#include "light_component.hpp"

#define MAX_POINT_LIGHTS 4

namespace gel {
	class PointLightComponent : public LightComponent {
	public:
		PointLightComponent(
			gem::Vector<float, 3> ambient,
			gem::Vector<float, 3> diffuse,
			gem::Vector<float, 3> specular,
			float range = 1.0f,
			float constant = 1.0f,
			float linear = 0.09f,
			float quadratic = 0.032f)
			: LightComponent(ambient, diffuse, specular), 
			range_(range), constant_(constant), linear_(linear), quadratic_(quadratic) {
		};

		float getRange() const {
			return range_;
		}

		float getConstant() const {
			return constant_;
		}

		float getLinear() const {
			return linear_;
		}

		float getQuadratic() const {
			return quadratic_;
		}

	private:
		float range_;
		float constant_;
		float linear_;
		float quadratic_;
	};
}