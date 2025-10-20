#pragma once

#include "game_component.hpp"
#include "game_entity.hpp"
#include "gem.hpp"

#include <iostream>
#include <iomanip>
#include <vector>
#include <glad/glad.h>
#include <cassert>

#pragma pack(push, 1)
struct MeshRendererVAO {
	float x, y, z;
	float nx, ny, nz;
	float u, v;
};
#pragma pack(pop)

namespace gel {
	class MeshRendererComponent : public GameComponent {
	public:
		MeshRendererComponent(
			const std::vector<MeshRendererVAO>& vertices, 
			const std::vector<unsigned int>& indices,
			GLuint shader_program)
		: vertices_(vertices), indices_(indices) , shader_program_(shader_program)
		{
			setup();
		}

		~MeshRendererComponent() override {
			glDeleteBuffers(1, &vbo_);
			glDeleteBuffers(1, &ebo_);
			glDeleteVertexArrays(1, &vao_);
			glDeleteProgram(shader_program_);
		}

		void update(float delta_time) override {}

		void render(gem::Matrix4<float> m, gem::Matrix4<float> v, gem::Matrix4<float> p) override {
			glUseProgram(shader_program_);

			glUniformMatrix4fv(glGetUniformLocation(shader_program_, "model"), 1, GL_FALSE, &m[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(shader_program_, "view"), 1, GL_FALSE, &v[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(shader_program_, "proj"), 1, GL_FALSE, &p[0][0]);

			glBindVertexArray(vao_);
			glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices_.size()), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}

	private:
		std::vector<MeshRendererVAO> vertices_;
		std::vector<unsigned int> indices_;

		GLuint vao_ = 0;
		GLuint vbo_ = 0;
		GLuint ebo_ = 0;
		GLuint shader_program_ = 0;

		void setup() {
			// VAO Generate
			glGenVertexArrays(1, &vao_);
			glBindVertexArray(vao_);

			// VBO Generate
			glGenBuffers(1, &vbo_);
			assert(glGetError() == 0U);
			glBindBuffer(GL_ARRAY_BUFFER, vbo_);
			assert(glGetError() == 0U);
			glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(MeshRendererVAO), vertices_.data(), GL_STATIC_DRAW);
			assert(glGetError() == 0U);

			// VAO (Position)
			glEnableVertexAttribArray(0);
			assert(glGetError() == 0U);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(MeshRendererVAO), (void*)offsetof(MeshRendererVAO, x));
			assert(glGetError() == 0U);

			// VAO (Normal)
			glEnableVertexAttribArray(1);
			assert(glGetError() == 0U);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(MeshRendererVAO), (void*)offsetof(MeshRendererVAO, nx));
			assert(glGetError() == 0U);

			// VAO (UV)
			glEnableVertexAttribArray(2);
			assert(glGetError() == 0U);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(MeshRendererVAO), (void*)offsetof(MeshRendererVAO, u));
			assert(glGetError() == 0U);

			// EBO Generate
			glGenBuffers(1, &ebo_);
			assert(glGetError() == 0U);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
			assert(glGetError() == 0U);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(unsigned int), indices_.data(), GL_STATIC_DRAW);
			assert(glGetError() == 0U);

			glBindVertexArray(0);
		}
	};
}