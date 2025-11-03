#pragma once

#include "camera/camera_component.hpp"
#include <vector>
#include <glad/glad.h>

namespace gel {
	class GameEntity;

	class GameScene {
	public:
		GameScene() = default;
		virtual ~GameScene() = default;

		void update(float delta_time);
		void render();

		void addEntity(GameEntity* entity) {
			entities_.push_back(entity);
		}

		void removeEntity(GameEntity* entity) {
			entities_.erase(std::remove(entities_.begin(), entities_.end(), entity), entities_.end());
		}

		const std::vector<GameEntity*>& getEntities() const {
			return entities_;
		}

		int getEntitySize() const {
			return static_cast<int>(entities_.size());
		}

		CameraComponent* getMainCamera() {
			return mainCamera_;
		}

		void setMainCamera(CameraComponent* camera) {
			this->mainCamera_ = camera;
		}

		void setMainCamera(GameEntity* entity) {
			if (entity) {
				CameraComponent* camera = entity->getComponent<CameraComponent>();
				if (camera) {
					this->mainCamera_ = camera;
				}
			}
		}

		void setShaderProgram(GLuint shader_program) {
			shader_program_ = shader_program;
		}

	private:
		std::vector<GameEntity*> entities_;
		CameraComponent* mainCamera_ = nullptr;
		GLuint shader_program_ = 0;

		void recursiveUpdate(GameEntity* entity, float delta_time);
		void recursiveRender(GameEntity* entity);
	};
}