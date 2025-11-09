#include <iostream>
#include <glad/glad.h>

namespace gel {

	struct ShaderResource {
		GLuint vtx_shader;
		GLuint frag_shader;
		GLuint program;
	};
}