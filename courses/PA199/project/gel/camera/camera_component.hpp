#pragma once

#include "game_component.hpp"
#include "game_entity.hpp" // Include the header file for GameEntity
#include "gem.hpp"

namespace gel {
	class CameraComponent : public GameComponent {
	public:
		CameraComponent(float fov = 60.0f, float aspect_ratio = 16.0f / 9.0f, float near_plane = 0.1f, float far_plane = 1000.0f, bool use_ortho = false)
			: fov_(fov), aspect_ratio_(aspect_ratio), near_plane_(near_plane), far_plane_(far_plane), use_ortho_(use_ortho) {
		}

		float fov() const { return fov_; }
		float aspect_ratio() const { return aspect_ratio_; }
		float near() const { return near_plane_; }
		float far() const { return far_plane_; }
		void useOrthographic(bool use_ortho) { use_ortho_ = use_ortho; }

		void update(float delta_time) override {
			if (getEntity() != nullptr) {
				viewMatrix = gem::Matrix4<float>::lookAt(
					getEntity()->getPosition(), // Eye
					targetEntity ? targetEntity->getPosition() : getEntity()->getPosition() + getEntity()->getForwardVector(), // Center
					getEntity()->getUpVector() // Up
				);
			}

			if (use_ortho_) {
				// Treat fov_ as orthographic HEIGHT in world units.
				float halfH = fov_ * 0.5f;
				float halfW = halfH * aspect_ratio_;

				projectionMatrix = gem::Matrix4<float>::ortho(
					-halfW, halfW, -halfH, halfH, near_plane_, far_plane_
				);
				return;
			}

			projectionMatrix = gem::Matrix4<float>::perspective(fov_, aspect_ratio_, near_plane_, far_plane_);
		}

		void render() override {}

		void setAspectRatio(float aspect_ratio) { aspect_ratio_ = aspect_ratio; }

		const gem::Matrix4<float>& getViewMatrix() const { return viewMatrix; }
		const gem::Matrix4<float>& getProjectionMatrix() const { return projectionMatrix; }

		GameEntity* targetEntity = nullptr;

	private:
		float fov_;
		float aspect_ratio_;
		float near_plane_;
		float far_plane_;
		bool use_ortho_;

		gem::Matrix4<float> viewMatrix;
		gem::Matrix4<float> projectionMatrix;
	};
}