#pragma once

#include "mesh_renderer_component.hpp"
#include <vector>

namespace gel {

	inline std::vector<MeshRendererVAO> GenerateCircleVertices(float radius, int segments)
	{
		std::vector<MeshRendererVAO> vao;

		vao.push_back({ 0.0f, 0.0f, 0.0f, 0,1,0, 0.5f, 0.5f });

		for (int i = 0; i <= segments; ++i) {
			float theta = (2.0f * static_cast<float>(M_PI) * i) / segments;

			gem::Polar<float> polar(radius, theta);
			auto pos2D = polar.toCartesian();

			float x = pos2D[0];
			float z = pos2D[1];

			// Map circle coordinates to UV range [0,1]
			float u = 0.5f + 0.5f * x / radius;
			float v = 0.5f + 0.5f * z / radius;

			vao.push_back({ x, 0.0f, z, 0, 1, 0, u, v });
		}

		return vao;
	}

	inline std::vector<unsigned int> GenerateCircleIndices(int segments)
	{
		std::vector<unsigned int> indices;

		for (int i = 1; i <= segments; ++i) {
			indices.push_back(0);
			indices.push_back(i % segments + 1);
			indices.push_back(i);
		}

		return indices;
	}

	class CircleRendererComponent : public MeshRendererComponent {
	public:
		CircleRendererComponent(float radius = 1.0f, int segments = 32, GLuint texture = 0) :
			MeshRendererComponent(
				GenerateCircleVertices(radius, segments),
				GenerateCircleIndices(segments),
				texture),
			radius_(radius), segments_(segments)
		{
		}
	private:
		float radius_;
		int segments_;
	};
}