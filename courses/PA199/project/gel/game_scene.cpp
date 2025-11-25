#include "game_scene.hpp"
#include "game_entity.hpp"
#include "light/point_light_component.hpp"
#include "renderer/renderer_component.hpp"
#include "renderer/mesh_renderer_component.hpp"
#include <iostream>
#include <string>

namespace gel {

	void GameScene::setupLights() {
		// Setup Main Light
		bool isUsingLight = false;
		if (mainLight_ != nullptr) {
			glUniform3f(glGetUniformLocation(shader_program_, "mainLight.direction"), mainLight_->getDirection()[0], mainLight_->getDirection()[1], mainLight_->getDirection()[2]);
			glUniform3f(glGetUniformLocation(shader_program_, "mainLight.ambient"), mainLight_->getAmbient()[0], mainLight_->getAmbient()[1], mainLight_->getAmbient()[2]);
			glUniform3f(glGetUniformLocation(shader_program_, "mainLight.diffuse"), mainLight_->getDiffuse()[0], mainLight_->getDiffuse()[1], mainLight_->getDiffuse()[2]);
			glUniform3f(glGetUniformLocation(shader_program_, "mainLight.specular"), mainLight_->getSpecular()[0], mainLight_->getSpecular()[1], mainLight_->getSpecular()[2]);

			isUsingLight = true;
		}

		int num_point_light = 0;
		for (auto* el : extraLights_) {
			PointLightComponent* plc = dynamic_cast<PointLightComponent*>(el);
			if (plc && num_point_light < MAX_POINT_LIGHTS) {
				std::string indexStr = std::to_string(num_point_light);
				gem::Matrix4<float> lightPos = plc->getEntity()->getWorldTransform();
				float safeRange = std::max(plc->getRange(), 0.001f);
				glUniform3f(glGetUniformLocation(shader_program_, ("pointLights[" + indexStr + "].position").c_str()), lightPos[0][3], lightPos[1][3], lightPos[2][3]);
				glUniform3f(glGetUniformLocation(shader_program_, ("pointLights[" + indexStr + "].ambient").c_str()), plc->getAmbient()[0], plc->getAmbient()[1], plc->getAmbient()[2]);
				glUniform3f(glGetUniformLocation(shader_program_, ("pointLights[" + indexStr + "].diffuse").c_str()), plc->getDiffuse()[0], plc->getDiffuse()[1], plc->getDiffuse()[2]);
				glUniform3f(glGetUniformLocation(shader_program_, ("pointLights[" + indexStr + "].specular").c_str()), plc->getSpecular()[0], plc->getSpecular()[1], plc->getSpecular()[2]);
				glUniform1f(glGetUniformLocation(shader_program_, ("pointLights[" + indexStr + "].constant").c_str()), plc->getConstant());
				glUniform1f(glGetUniformLocation(shader_program_, ("pointLights[" + indexStr + "].linear").c_str()), plc->getLinear());
				glUniform1f(glGetUniformLocation(shader_program_, ("pointLights[" + indexStr + "].quadratic").c_str()), plc->getQuadratic());
				glUniform1f(glGetUniformLocation(shader_program_, ("pointLights[" + indexStr + "].range").c_str()), safeRange);
				num_point_light++;
			}
		}

		if (isUsingLight || num_point_light > 0) {
			gem::Matrix4<float> viewPos = mainCamera_->getEntity()->getWorldTransform();
			glUniform3f(glGetUniformLocation(shader_program_, "view_pos"), viewPos[0][3], viewPos[1][3], viewPos[2][3]);
			glUniform1i(glGetUniformLocation(shader_program_, "num_point_lights"), num_point_light);
		}
	}

	void GameScene::recursiveUpdate(GameEntity* entity, float delta_time) {
		for (auto* comp : entity->getComponents()) {
			comp->update(delta_time);
		}

		for (auto* child : entity->getChildren()) {
			recursiveUpdate(child, delta_time);
		}
	}

	void GameScene::recursiveRender(GameEntity* entity) {
		glUseProgram(shader_program_);

		RendererComponent* rc = entity->getComponent<RendererComponent>();
		if (rc) {
			gem::Matrix4<float> m = entity->getWorldTransform();
			gem::Matrix4<float> v = mainCamera_->getViewMatrix();
			gem::Matrix4<float> p = mainCamera_->getProjectionMatrix();

			glUniformMatrix4fv(glGetUniformLocation(shader_program_, "model"), 1, GL_TRUE, &m[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(shader_program_, "view"), 1, GL_TRUE, &v[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(shader_program_, "proj"), 1, GL_TRUE, &p[0][0]);

			// Render Meshes
			MeshRendererComponent* mrc = dynamic_cast<MeshRendererComponent*>(rc);
			if (mrc) {
				GLuint texture = mrc->getTexture();
				if (texture) {
					glActiveTexture(GL_TEXTURE0);
					assert(glGetError() == 0U);
					glBindTexture(GL_TEXTURE_2D, texture);
					glUniform1i(glGetUniformLocation(shader_program_, "use_texture"), 1);
				}
				else {
					gem::Vector<float, 3> mrc_color = mrc->getColor();
					glUniform3f(glGetUniformLocation(shader_program_, "unlit_color"), mrc_color[0], mrc_color[1], mrc_color[2]);
					glUniform1i(glGetUniformLocation(shader_program_, "use_texture"), 0);
				}
			}
			
			setupLights();
			rc->render();
		}

		for (auto* child : entity->getChildren()) {
			recursiveRender(child);
		}
	}

	void GameScene::recursiveHandleKeyPressed(GameEntity* entity, int key, int scancode, int action, int mods) {
		for (auto* comp : entity->getComponents()) {
			comp->handleKeyPressed(key, scancode, action, mods);
		}

		for (auto* child : entity->getChildren()) {
			recursiveHandleKeyPressed(child, key, scancode, action, mods);
		}
	}

	void GameScene::update(float delta_time) {
		for (auto* entity : entities_) {
			recursiveUpdate(entity, delta_time);
		}
	}

	void GameScene::render() {
		if (!mainCamera_) {
			std::cerr << "Error: No main camera set for the scene." << std::endl;
			return;
		};

		if (!shader_program_) {
			std::cerr << "Error: No shader set for the scene." << std::endl;
			return;
		}

		for (auto* entity : entities_) {
			recursiveRender(entity);
		}
	}

	void GameScene::handleKeyPressed(int key, int scancode, int action, int mods) {
		for (auto* entity : entities_) {
			recursiveHandleKeyPressed(entity, key, scancode, action, mods);
		}
	}
}