#pragma once

#include "mesh_renderer_component.hpp"
#include <vector>

namespace gel {

	inline std::vector<MeshRendererVAO> GenerateCylinderVertices(
		float radius,
		int segments,
		float height
	) {
		std::vector<MeshRendererVAO> vao;

		float halfHeight = height / 2.0f;

		vao.push_back({ 0.0f, -halfHeight, 0.0f, 0, -1, 0, 0.5f, 0.5f });
		vao.push_back({ 0.0f, halfHeight, 0.0f, 0, 1, 0, 0.5f, 0.5f });

		for (int i = 0; i <= segments; ++i) {
			float theta = 2.0f * static_cast<float>(M_PI) * i / segments;

			gem::Polar<float> polar(radius, theta);
			auto pos2D = polar.toCartesian();

			float x = pos2D[0];
			float z = pos2D[1];

			float u = 0.5f + 0.5f * cosf(theta);
			float v = 0.5f + 0.5f * sinf(theta);

			vao.push_back({ x, -halfHeight, z, 0, -1, 0, u, v });
			vao.push_back({ x, halfHeight, z, 0, 1, 0, u, v });

			// Side normals (point outwards)
			float nx = cosf(theta);
			float nz = sinf(theta);

			vao.push_back({ x, -halfHeight, z, nx, 0, nz, (float)i / segments, 0.0f });
			vao.push_back({ x, halfHeight, z, nx, 0, nz, (float)i / segments, 1.0f });
		}

		return vao;
	}

	inline std::vector<unsigned int> GenerateCylinderIndices(int segments) {
		std::vector<unsigned int> indices;

		int bottomCenter = 0;
		int topCenter = 1;
		int vertexOffset = 2;

		for (int i = 0; i < segments; ++i) {
			indices.push_back(bottomCenter);
			indices.push_back(vertexOffset + i * 4);      // bottom edge
			indices.push_back(vertexOffset + ((i + 1) % segments) * 4);
		}

		for (int i = 0; i < segments; ++i) {
			indices.push_back(topCenter);
			indices.push_back(vertexOffset + ((i + 1) % segments) * 4 + 1); // top edge
			indices.push_back(vertexOffset + i * 4 + 1);
		}

		for (int i = 0; i < segments; ++i) {
			int bottom1 = vertexOffset + i * 4 + 2;
			int top1 = vertexOffset + i * 4 + 3;
			int bottom2 = vertexOffset + ((i + 1) % segments) * 4 + 2;
			int top2 = vertexOffset + ((i + 1) % segments) * 4 + 3;

			indices.push_back(bottom1);
			indices.push_back(top1);
			indices.push_back(bottom2);

			indices.push_back(top1);
			indices.push_back(top2);
			indices.push_back(bottom2);
		}

		return indices;
	}

	class CylinderRendererComponent : public MeshRendererComponent {
	public:
		CylinderRendererComponent(
			float radius = 1.0f,
			int segments = 16,
			float height = 1.0f,
			GLuint texture = 0) : MeshRendererComponent(
				GenerateCylinderVertices(radius, segments, height),
				GenerateCylinderIndices(segments),
				texture
			), radius_(radius), segments_(segments), height_(height) {
		}

		float radius() const { return radius_; }
		int segments() const { return segments_; }
		float height() const { return height_; }

	private:
		float radius_;
		int segments_;
		float height_;
	};
}