#pragma once

#include "gem.hpp"
#include "game_component.hpp"

namespace gel {
	class LightComponent : public GameComponent {
	public:
		LightComponent(
			gem::Vector<float, 3> ambient, 
			gem::Vector<float, 3> diffuse, 
			gem::Vector<float, 3> specular)
			: ambient_(ambient), 
			diffuse_(diffuse),
			specular_(specular) {
		}

		void update(float delta_time) override {}

		void render() override {}

		gem::Vector<float, 3> getAmbient() const {
			return ambient_;
		}

		gem::Vector<float, 3> getDiffuse() const {
			return diffuse_;
		}

		gem::Vector<float, 3> getSpecular() const {
			return specular_;
		}

	private:
		gem::Vector<float, 3> ambient_;
		gem::Vector<float, 3> diffuse_;
		gem::Vector<float, 3> specular_;
	};
}