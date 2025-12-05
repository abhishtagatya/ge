#pragma once

#include "mesh_renderer_component.hpp"
#include <vector>

namespace gel {

	inline std::vector<MeshRendererVAO> GenerateArcVertices(
		float inner_radius, float outer_radius, int segments,
		float height = 1.0f, float angle = 2.0f * M_PI
	) {
		std::vector<MeshRendererVAO> vao;
		float halfHeight = height / 2.0f;

		for (int i = 0; i <= segments; ++i) {
			float theta = angle * i / segments;

			gem::Polar<float> outerPolar(outer_radius, theta);
			gem::Polar<float> innerPolar(inner_radius, theta);

			auto outerPos = outerPolar.toCartesian();
			auto innerPos = innerPolar.toCartesian();

			float xi = innerPos[0], zi = innerPos[1];
			float xo = outerPos[0], zo = outerPos[1];

			// Normals (Flats)
			float nx = cosf(theta);
			float nz = sinf(theta);

			float u = (float)i / segments;

			// Top Face
			vao.push_back(MeshRendererVAO{ xo, halfHeight, zo, 0, 1, 0, u, 1.0f });
			vao.push_back(MeshRendererVAO{ xi, halfHeight, zi, 0, 1, 0, u, 0.0f });

			// Bottom Face
			vao.push_back(MeshRendererVAO{ xo, -halfHeight, zo, 0, -1, 0, u, 1.0f });
			vao.push_back(MeshRendererVAO{ xi, -halfHeight, zi, 0, -1, 0, u, 0.0f });

			// Outer Face
			vao.push_back(MeshRendererVAO{ xo, -halfHeight, zo, nx, 0, nz, u, 0.0f });
			vao.push_back(MeshRendererVAO{ xo, halfHeight, zo, nx, 0, nz, u, 1.0f });

			// Inner Face
			vao.push_back(MeshRendererVAO{ xi, -halfHeight, zi, -nx, 0, -nz, u, 0.0f });
			vao.push_back(MeshRendererVAO{ xi, halfHeight, zi, -nx, 0, -nz, u, 1.0f });
		}

		return vao;
	}

	inline std::vector<unsigned int> GenerateArcIndices(int segments, bool closed = true) {
		std::vector<unsigned int> indices;

		const int stride = 8;

		for (int i = 0; i < segments; ++i) {
			int base = i * stride;
			int next = (i + 1) * stride;

			// Top Face
			indices.push_back(base + 0);
			indices.push_back(next + 0);
			indices.push_back(next + 1);

			indices.push_back(base + 0);
			indices.push_back(next + 1);
			indices.push_back(base + 1);

			// Bottom Face
			indices.push_back(base + 2);
			indices.push_back(next + 3);
			indices.push_back(next + 2);

			indices.push_back(base + 2);
			indices.push_back(base + 3);
			indices.push_back(next + 3);

			// Outer Face
			indices.push_back(base + 4);
			indices.push_back(next + 5);
			indices.push_back(next + 4);

			indices.push_back(base + 4);
			indices.push_back(base + 5);
			indices.push_back(next + 5);

			// Inner Face
			indices.push_back(base + 6);
			indices.push_back(next + 6);
			indices.push_back(next + 7);

			indices.push_back(base + 6);
			indices.push_back(next + 7);
			indices.push_back(base + 7);
		}

		if (!closed && segments > 0) {
			int start = 0;
			indices.push_back(start);
			indices.push_back(start + 2);
			indices.push_back(start + 3);

			indices.push_back(start);
			indices.push_back(start + 3);
			indices.push_back(start + 1);

			int end = segments * stride;
			indices.push_back(end);
			indices.push_back(end + 3);
			indices.push_back(end + 2);

			indices.push_back(end);
			indices.push_back(end + 1);
			indices.push_back(end + 3);
		}

		return indices;
	}

	class ArcRendererComponent : public MeshRendererComponent {
	public:
		ArcRendererComponent(
			float inner_radius, float outer_radius, int segments,
			float height = 1.0f, float angle = 2.0f * M_PI,
			GLuint texture = 0,
			int strength = 3
		) : MeshRendererComponent(
			GenerateArcVertices(inner_radius, outer_radius, segments, height, angle),
			GenerateArcIndices(segments, angle >= 2.0f * M_PI),
			texture, strength
		), 
			inner_radius_(inner_radius), 
			outer_radius_(outer_radius), 
			segments_(segments),
			height_(height),
			angle_(angle)
		{}

		float innerRadius() const { return inner_radius_; }
		float outerRadius() const { return outer_radius_; }
		int segments() const { return segments_; }
		float height() const { return height_; }
		float angle() const { return angle_; }
		bool breakOnCollision() {
			if (mesh_current_strength_ > 0) {
				mesh_current_strength_--;
				return false;
			}

			return true;
		}
	private:
		float inner_radius_;
		float outer_radius_;
		int segments_;
		float height_;
		float angle_;
	};
}