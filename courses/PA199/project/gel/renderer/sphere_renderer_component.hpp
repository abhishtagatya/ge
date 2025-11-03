#pragma once

#include "mesh_renderer_component.hpp"
#include <vector>

namespace gel {

	inline std::vector<MeshRendererVAO> GenerateSphereVertices(
		float radius,
		unsigned int sector,
		unsigned int stack)
	{
		std::vector<MeshRendererVAO> vao;

		for (unsigned int i = 0; i <= stack; ++i) {
			float phi = M_PI / 2.0f - i * (M_PI / stack);

			gem::Polar<float> stackPolar(radius, phi);
			auto stackPos2D = stackPolar.toCartesian(); // x = r*cos(phi), y = r*sin(phi)
			
			float xy = stackPos2D[0];
			float z = stackPos2D[1];

			for (unsigned int j = 0; j <= sector; ++j) {
				float theta = j * (2.0f * M_PI / sector);

				gem::Polar<float> sectorPolar(xy, theta);
				auto sectorPos2D = sectorPolar.toCartesian(); // x = r*cos(theta), y = r*sin(theta)

				float x = sectorPos2D[0];
				float y = sectorPos2D[1];

				float nx = x / radius;
				float ny = y / radius;
				float nz = z / radius;

				float u = (float)j / sector;
				float v = (float)i / stack;

				vao.push_back({ x, y, z, nx, ny, nz, u, v });
			}
		}

		return vao;
	}

	inline std::vector<unsigned int> GenerateSphereIndices(
		float radius,
		unsigned int sector,
		unsigned int stack) {

		std::vector<unsigned int> indices;

		for (unsigned int i = 0; i < stack; ++i) {
			unsigned int k1 = i * (sector + 1);
			unsigned int k2 = k1 + sector + 1;

			for (unsigned int j = 0; j < sector; ++j, ++k1, ++k2) {
				if (i != 0) {
					indices.push_back(k1);
					indices.push_back(k2);
					indices.push_back(k1 + 1);
				}
				if (i != (stack - 1)) {
					indices.push_back(k1 + 1);
					indices.push_back(k2);
					indices.push_back(k2 + 1);
				}
			}
		}

		return indices;
	}

	class SphereRendererComponent : public MeshRendererComponent {
	public:
		SphereRendererComponent(
			float radius = 1.0f, 
			unsigned int sector = 36, 
			unsigned int stack = 18,
			GLuint texture = 0) : MeshRendererComponent(
				GenerateSphereVertices(radius, sector, stack),
				GenerateSphereIndices(radius, sector, stack),
				texture),
			radius_(radius),
			sector_(sector),
			stack_(stack) {}

		float radius() const {
			return radius_;
		}

		unsigned int sector() const {
			return sector_;
		}

		unsigned int stack() const {
			return stack_;
		}
	private:
		float radius_;
		unsigned int sector_;
		unsigned int stack_;
	};
}