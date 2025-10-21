#include "game_scene.hpp"
#include "game_entity.hpp"
#include "mesh_renderer_component.hpp"

namespace gel {

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

		MeshRendererComponent* mrc = entity->getComponent<MeshRendererComponent>();
		if (mrc) {
			gem::Matrix4<float> m = entity->getWorldTransform();
			gem::Matrix4<float> v = mainCamera_->getViewMatrix();
			gem::Matrix4<float> p = mainCamera_->getProjectionMatrix();

			glUniformMatrix4fv(glGetUniformLocation(shader_program_, "model"), 1, GL_TRUE, &m[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(shader_program_, "view"), 1, GL_TRUE, &v[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(shader_program_, "proj"), 1, GL_TRUE, &p[0][0]);

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

			mrc->render();
		}

		for (auto* child : entity->getChildren()) {
			recursiveRender(child);
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
}