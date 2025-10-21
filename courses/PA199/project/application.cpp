// ################################################################################
// Common Framework for Computer Graphics Courses at FI MUNI.
// 
// Copyright (c) 2021-2022 Visitlab (https://visitlab.fi.muni.cz)
// All rights reserved.
// ################################################################################

#include "application.hpp"
#include "glad/glad.h"
#include "lodepng.h"
#include <filesystem>
#include <iostream>
#include <iomanip>

#include <gem.hpp>
#include <gel.hpp>

static GLuint load_shader(std::filesystem::path const& path, GLenum const shader_type)
{
    std::filesystem::path const current = std::filesystem::current_path();
    GLuint const shader = glCreateShader(shader_type);
    assert(glGetError() == 0U && shader != 0);
    std::ifstream ifs(path);
    assert(ifs.is_open());
    std::string const str((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
    char const* code = str.c_str();
    glShaderSource(shader, 1, &code, nullptr);
    assert(glGetError() == 0U);
    glCompileShader(shader);
    assert(glGetError() == 0U);
    return shader;
}

static GLuint load_texture(std::filesystem::path const& path)
{
    std::vector<unsigned char> texels;
    unsigned int width, height;
    unsigned int error_code = lodepng::decode(texels, width, height, path.string(), LCT_RGBA);
    assert(error_code == 0);

    //flip the image vertically
    for (unsigned int lo = 0, hi = height - 1; lo < hi; ++lo, --hi)
        for (unsigned int*  lo_ptr = (unsigned int*)texels.data() + lo * width,
                         *  lo_end = lo_ptr + width,
                         *  hi_ptr = (unsigned int*)texels.data() + hi * width;
                lo_ptr != lo_end; ++lo_ptr, ++hi_ptr)
            std::swap(*lo_ptr, *hi_ptr);

    GLuint texture;
	glGenTextures(1, &texture);
    assert(glGetError() == 0U);
	glBindTexture(GL_TEXTURE_2D, texture);
    assert(glGetError() == 0U);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (void*)texels.data());
    assert(glGetError() == 0U);
    glGenerateMipmap(GL_TEXTURE_2D);
    assert(glGetError() == 0U);
    return texture;
}

static GLuint create_shader_program(GLuint const  vertex_shader, GLuint const  fragment_shader) {
    GLuint const shader_program = glCreateProgram();

    assert(glGetError() == 0U && shader_program != 0);
    glAttachShader(shader_program, vertex_shader);
    assert(glGetError() == 0U);
    glAttachShader(shader_program, fragment_shader);
    assert(glGetError() == 0U);
    glLinkProgram(shader_program);
    assert(glGetError() == 0U);
    glDetachShader(shader_program, vertex_shader);
    assert(glGetError() == 0U);
    glDetachShader(shader_program, fragment_shader);
    assert(glGetError() == 0U);

    return shader_program;
}

// ----------------------------------------------------------------------------
// Constructors & Destructors
// ----------------------------------------------------------------------------

Application::Application(int initial_width, int initial_height, std::vector<std::string> arguments)
    : IApplication(initial_width, initial_height, arguments)
{
	std::cout << "Application started with following arguments:\n" << std::endl;
    glViewport(0, 0, width, height);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    unlit_vtx_shader = load_shader(lecture_folder_path / "data" / "shaders" / "default.vert", GL_VERTEX_SHADER);
    unlit_frg_shader = load_shader(lecture_folder_path / "data" / "shaders" / "default.frag", GL_FRAGMENT_SHADER);
    unlit_program = create_shader_program(unlit_vtx_shader, unlit_frg_shader);

	GLuint texture = load_texture(lecture_folder_path / "data" / "textures" / "container.png");

    std::vector<MeshRendererVAO> cubeVertices = {
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

    std::vector<unsigned int> cubeIndices = {
        0, 1, 2, 0, 2, 3,       // Front
        4, 5, 6, 4, 6, 7,       // Back
        8, 9,10, 8,10,11,       // Left
        12,13,14, 12,14,15,     // Right
        16,17,18, 16,18,19,     // Top
        20,21,22, 20,22,23      // Bottom
    };

    auto cubeRenderComponent = new gel::MeshRendererComponent(
        cubeVertices,
        cubeIndices,
        texture
    );

	auto c1 = new gel::GameEntity(
        gem::Vector<float, 3> { 1.0f, 0.0f, 0.0f },
        gem::Vector<float, 3> { 0.0f, 0.0f, 0.0f },
        gem::Vector<float, 3> { 1.0f, 1.0f, 1.0f }
    );
    c1->addComponent(cubeRenderComponent);

    auto c2 = new gel::GameEntity(
		gem::Vector<float, 3> { 0.0f, 0.0f, 0.0f },
		gem::Vector<float, 3> { 0.0f, 0.0f, 0.0f },
		gem::Vector<float, 3> { 1.0f, 1.0f, 1.0f }
    );
    c2->addComponent(cubeRenderComponent);

    auto c3 = new gel::GameEntity(
        gem::Vector<float, 3> { -1.0f, 0.0f, 0.0f },
        gem::Vector<float, 3> { 0.0f, 0.0f, 0.0f },
        gem::Vector<float, 3> { 1.0f, 1.0f, 1.0f }
    );
    c3->addComponent(cubeRenderComponent);

    auto c4 = new gel::GameEntity(
        gem::Vector<float, 3> { 0.0f, 1.0f, 0.0f },
        gem::Vector<float, 3> { 0.0f, 0.0f, 0.0f },
        gem::Vector<float, 3> { 1.0f, 1.0f, 1.0f }
    );
    c4->addComponent(cubeRenderComponent);

    auto c5 = new gel::GameEntity(
        gem::Vector<float, 3> { 0.0f, -5.0f, 0.0f },
        gem::Vector<float, 3> { 0.0f, 0.0f, 0.0f },
		gem::Vector<float, 3> { 1.0f, 1.0f, 1.0f } * 5.0f
    );
    c5->addComponent(cubeRenderComponent);

	firstCamera = new gel::GameEntity(
        gem::Vector<float, 3> { 0.0f, 0.0f, -5.0f },
        gem::Vector<float, 3> { 0.0f, 0.0f, 0.0f },
        gem::Vector<float, 3> { 1.0f, 1.0f, 1.0f }
    );

	auto cameraComponent = new gel::CameraComponent(
        90.0f, width / height, 0.1f, 1000.0f
    );
    cameraComponent->targetEntity = c2;
    firstCamera->addComponent(cameraComponent);

    secondCamera = new gel::GameEntity(
        gem::Vector<float, 3> { 0.0f, 5.0f, -5.0f },
        gem::Vector<float, 3> { 0.0f, 0.0f, 0.0f },
        gem::Vector<float, 3> { 1.0f, 1.0f, 1.0f }
    );
    auto secondaryCameraComponent = new gel::CameraComponent(
        45.0f, width / height, 0.1f, 1000.0f
    );
    secondaryCameraComponent->targetEntity = c2;
    secondCamera->addComponent(secondaryCameraComponent);

	mainScene.addEntity(c1);
	mainScene.addEntity(c2);
	mainScene.addEntity(c3);
	mainScene.addEntity(c4);
	mainScene.addEntity(c5);

	mainScene.addEntity(firstCamera);
    mainScene.addEntity(secondCamera);

	mainScene.setMainCamera(firstCamera);
	mainScene.setShaderProgram(unlit_program);
}

Application::~Application()
{
    // Cleanup (Default)
	glDeleteProgram(unlit_program);
	glDeleteShader(unlit_vtx_shader);
	glDeleteShader(unlit_frg_shader);
}

// ----------------------------------------------------------------------------
// Methods
// ----------------------------------------------------------------------------

void Application::update(float delta) {
	mainScene.update(delta);
}

void Application::render() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    // Sets the clear color.
    glClearColor(0.0, 0.0, 0.0, 1.0f);
    // Clears the window using the above color.
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /*
    glUseProgram(shader_program);
    assert(glGetError() == 0U);
    glBindVertexArray(vertex_arrays);
    assert(glGetError() == 0U);
    glActiveTexture(GL_TEXTURE0);
    assert(glGetError() == 0U);
    glBindTexture(GL_TEXTURE_2D, texture);
    assert(glGetError() == 0U);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    assert(glGetError() == 0U);
    */

    glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    mainScene.render();
}

void Application::render_ui() {}

void Application::on_resize(int width, int height) {
    // Calls the default implementation to set the class variables.
    IApplication::on_resize(width, height);
    // Changes the viewport.
    glViewport(0, 0, width, height);
	mainScene.getMainCamera()->setAspectRatio(float(width) / float(height));
}

void Application::on_mouse_move(double x, double y) {}

void Application::on_mouse_button(int button, int action, int mods) {}

void Application::on_key_pressed(int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        switch (key) {
        case GLFW_KEY_1:
			mainScene.setMainCamera(firstCamera);
            break;
        case GLFW_KEY_2:
            mainScene.setMainCamera(secondCamera);
            break;
        }
    }
}
