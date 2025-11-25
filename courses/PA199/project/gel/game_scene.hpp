#pragma once

#include "camera/camera_component.hpp"
#include "light/directional_light_component.hpp"
#include "util/shader_resource.hpp"

#include <vector>
#include <map>
#include <glad/glad.h>

namespace gel {
	class GameEntity; // Forward declaration

	class GameScene {
	public:
		GameScene() = default;
		virtual ~GameScene() {
			for (auto entity : entities_) {
				delete entity;
			}
			entities_.clear();

			for (auto& [name, val] : shader_resources_) {
				glDeleteShader(val.vtx_shader);
				glDeleteShader(val.frag_shader);
				glDeleteProgram(val.program);
			}
			shader_resources_.clear();
		}

		void update(float delta_time);
		void setupLights();
		void render();
		void handleKeyPressed(int key, int scancode, int action, int mods);

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

		DirectionalLightComponent* getMainLight() {
			return mainLight_;
		}

		void setMainLight(DirectionalLightComponent* light) {
			this->mainLight_ = light;
		}

		void addExtraLight(LightComponent* light) {
			extraLights_.push_back(light);
		}

		const std::vector<LightComponent*>& getExtraLights() const {
			return extraLights_;
		}

		void setMainCamera(GameEntity* entity) {
			if (entity) {
				CameraComponent* camera = entity->getComponent<CameraComponent>();
				if (camera) {
					this->mainCamera_ = camera;
				}
			}
		}

		void addShaderResource(const std::string& name, const GLuint vertex_shader, const GLuint fragment_shader, const GLuint shader_program) {
			shader_resources_[name] = ShaderResource{ vertex_shader, fragment_shader, shader_program };
			shader_program_ = shader_program;
		}

		ShaderResource* getShaderResource(const std::string& name) {
			auto it = shader_resources_.find(name);
			if (it != shader_resources_.end()) {
				return &it->second;
			}
			return nullptr;
		}

		void useShaderProgram(const std::string& name) {
			ShaderResource* resource = getShaderResource(name);
			if (resource) {
				shader_program_ = resource->program;
			}
		}

	private:
		std::vector<GameEntity*> entities_;

		CameraComponent* mainCamera_ = nullptr;

		DirectionalLightComponent* mainLight_ = nullptr;
		std::vector<LightComponent*> extraLights_;

		GLuint shader_program_ = 0;
		std::map<std::string, ShaderResource> shader_resources_;

		void recursiveUpdate(GameEntity* entity, float delta_time);
		void recursiveRender(GameEntity* entity);
		void recursiveHandleKeyPressed(GameEntity* entity, int key, int scancode, int action, int mods);
	};
}