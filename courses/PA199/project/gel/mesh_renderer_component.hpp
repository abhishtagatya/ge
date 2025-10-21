#pragma once

#include "game_component.hpp"
#include "game_entity.hpp"
#include "gem.hpp"

#include <iostream>
#include <iomanip>
#include <vector>
#include <glad/glad.h>
#include <cassert>
#include <cstdlib>
#include <ctime>

#pragma pack(push, 1)
struct MeshRendererVAO {
	float x, y, z;
	float nx, ny, nz;
	float u, v;
};
#pragma pack(pop)


namespace gel {
	const std::vector<gem::Vector<float, 3>> COLOR_ = {
		{1.0f, 0.0f, 0.0f},  // Red
		{0.0f, 1.0f, 0.0f},  // Green
		{0.0f, 0.0f, 1.0f},  // Blue
		{1.0f, 1.0f, 0.0f},  // Yellow
		{1.0f, 0.0f, 1.0f},  // Magenta
		{0.0f, 1.0f, 1.0f},  // Cyan
		{1.0f, 0.5f, 0.0f}   // Orange
	};

	class MeshRendererComponent : public GameComponent {
	public:
		MeshRendererComponent(
			const std::vector<MeshRendererVAO>& vertices, 
			const std::vector<unsigned int>& indices,
			GLuint texture = 0)
		: vertices_(vertices), indices_(indices), texture_(texture)
		{
			static bool seeded = false;
			if (!seeded) {
				std::srand(static_cast<unsigned int>(std::time(nullptr)));
				seeded = true;
			}

			int color_index = std::rand() % COLOR_.size();
			color_ = COLOR_[color_index];

			setup();
		}

		~MeshRendererComponent() override {
			glDeleteBuffers(1, &vbo_);
			glDeleteBuffers(1, &ebo_);
			glDeleteVertexArrays(1, &vao_);
		}

		void update(float delta_time) override {}

		void render() override {
			glBindVertexArray(vao_);
			glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices_.size()), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}

		const gem::Vector<float, 3>& getColor() const {
			return color_;
		}

		const GLuint getTexture() const {
			return texture_;
		}

	private:
		std::vector<MeshRendererVAO> vertices_;
		std::vector<unsigned int> indices_;
		gem::Vector<float, 3> color_;

		GLuint vao_ = 0;
		GLuint vbo_ = 0;
		GLuint ebo_ = 0;
		GLuint texture_ = 0;

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