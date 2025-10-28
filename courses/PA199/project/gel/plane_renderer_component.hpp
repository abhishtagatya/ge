#pragma once

#include "mesh_renderer_component.hpp"
#include <vector>

namespace gel {

	inline std::vector<MeshRendererVAO> GeneratePlaneVertices(
		float size = 1.0f, int divisions = 1) 
	{
		std::vector<MeshRendererVAO> vao;

		for (int i = 0; i <= divisions; ++i) {
			for (int j = 0; j <= divisions; ++j) {
				float x = -size / 2 + (size / divisions) * j;
				float z = -size / 2 + (size / divisions) * i;
				float u = (float)j / divisions;
				float v = (float)i / divisions;

				vao.push_back({ x, 0.0f, z, 0.0f, 1.0f, 0.0f, u, v });
			}
		}

		return vao;
	}

	inline std::vector<unsigned int> GeneratePlaneIndices(
		float size = 1.0f, int divisions = 1) {
		std::vector<unsigned int> indices;

		for (int i = 0; i < divisions; ++i) {
			for (int j = 0; j < divisions; ++j) {
				unsigned int topLeft = i * (divisions + 1) + j;
				unsigned int topRight = topLeft + 1;
				unsigned int bottomLeft = (i + 1) * (divisions + 1) + j;
				unsigned int bottomRight = bottomLeft + 1;

				indices.push_back(topLeft);
				indices.push_back(bottomLeft);
				indices.push_back(topRight);

				indices.push_back(topRight);
				indices.push_back(bottomLeft);
				indices.push_back(bottomRight);
			}
		}

		return indices;
	}

	class PlaneRendererComponent : public MeshRendererComponent {
	public:
		PlaneRendererComponent(
			float size = 1.0f, int divisions = 1, GLuint texture = 0) : MeshRendererComponent(
				GeneratePlaneVertices(size, divisions),
				GeneratePlaneIndices(size, divisions),
				texture
			), size_(size), divisions_(divisions)
		{}
	private:
		float size_;
		int divisions_;
	};
}