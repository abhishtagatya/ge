#pragma once

#include "mesh_renderer_component.hpp"
#include <vector>

namespace gel {

	inline std::vector<MeshRendererVAO> GenerateCubeVertices() {
		std::vector<MeshRendererVAO> vao = {
            // Front face (z+)
            { -0.5f, -0.5f,  0.5f, 0,0,1, 0,0 },
            {  0.5f, -0.5f,  0.5f, 0,0,1, 1,0 },
            {  0.5f,  0.5f,  0.5f, 0,0,1, 1,1 },
            { -0.5f,  0.5f,  0.5f, 0,0,1, 0,1 },

            // Back face (z-)
            {  0.5f, -0.5f, -0.5f, 0,0,-1, 0,0 },
            { -0.5f, -0.5f, -0.5f, 0,0,-1, 1,0 },
            { -0.5f,  0.5f, -0.5f, 0,0,-1, 1,1 },
            {  0.5f,  0.5f, -0.5f, 0,0,-1, 0,1 },

            // Left face (x-)
            { -0.5f, -0.5f, -0.5f, -1,0,0, 0,0 },
            { -0.5f, -0.5f,  0.5f, -1,0,0, 1,0 },
            { -0.5f,  0.5f,  0.5f, -1,0,0, 1,1 },
            { -0.5f,  0.5f, -0.5f, -1,0,0, 0,1 },

            // Right face (x+)
            {  0.5f, -0.5f,  0.5f, 1,0,0, 0,0 },
            {  0.5f, -0.5f, -0.5f, 1,0,0, 1,0 },
            {  0.5f,  0.5f, -0.5f, 1,0,0, 1,1 },
            {  0.5f,  0.5f,  0.5f, 1,0,0, 0,1 },

            // Top face (y+)
            { -0.5f,  0.5f,  0.5f, 0,1,0, 0,0 },
            {  0.5f,  0.5f,  0.5f, 0,1,0, 1,0 },
            {  0.5f,  0.5f, -0.5f, 0,1,0, 1,1 },
            { -0.5f,  0.5f, -0.5f, 0,1,0, 0,1 },

            // Bottom face (y-)
            { -0.5f, -0.5f, -0.5f, 0,-1,0, 0,0 },
            {  0.5f, -0.5f, -0.5f, 0,-1,0, 1,0 },
            {  0.5f, -0.5f,  0.5f, 0,-1,0, 1,1 },
            { -0.5f, -0.5f,  0.5f, 0,-1,0, 0,1 }
        };

        return vao;
	}

    inline std::vector<unsigned int> GenerateCubeIndices() {
        std::vector<unsigned int> indices = {
            0, 1, 2, 0, 2, 3,       // Front
            4, 5, 6, 4, 6, 7,       // Back
            8, 9,10, 8,10,11,       // Left
            12,13,14, 12,14,15,     // Right
            16,17,18, 16,18,19,     // Top
            20,21,22, 20,22,23      // Bottom
        };

        return indices;
    }

	class CubeRendererComponent : public MeshRendererComponent {
	public:
		CubeRendererComponent(GLuint texture = 0) : MeshRendererComponent(
			GenerateCubeVertices(),
			GenerateCubeIndices(),
			texture
		) {}
	};
}