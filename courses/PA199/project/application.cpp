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

    auto cubeRenderComponent = new gel::CubeRendererComponent(texture);
    auto sphereRenderComponent = new gel::SphereRendererComponent(0.5f, 6, 6);
	auto planeRenderComponent = new gel::PlaneRendererComponent(1.0f, 10.0f);
	auto circleRenderComponent = new gel::CircleRendererComponent(1.0f, 16, texture);
	auto cylinderRenderComponent = new gel::CylinderRendererComponent(0.5f, 16, 1.0f);

    auto c2 = new gel::GameEntity(
		gem::Vector<float, 3> { 0.0f, 0.0f, -1.0f },
		gem::Quaternion<float> { 1.0f, 0.0f, 0.0f, 0.0f },
		gem::Vector<float, 3> { 1.0f, 1.0f, 1.0f } * 0.5f
    );
    c2->addComponent(sphereRenderComponent);

    auto c3 = new gel::GameEntity(
        gem::Vector<float, 3> { 0.0f, 0.0f, 0.0f },
        gem::Quaternion<float> { 1.0f, 0.0f, 0.0f, 0.0f },
        gem::Vector<float, 3> { 1.0f, 1.0f, 1.0f }
    );
	c3->addComponent(cylinderRenderComponent);

    auto c5 = new gel::GameEntity(
        gem::Vector<float, 3> { 0.0f, -0.5f, 0.0f },
        gem::Quaternion<float> { 1.0f, 0.0f, 0.0f, 0.0f },
		gem::Vector<float, 3> { 1.0f, 1.0f, 1.0f } * 3.0f
    );
    c5->addComponent(circleRenderComponent);

	firstCamera = new gel::GameEntity(
        gem::Vector<float, 3> { 0.0f, 0.0f, -5.0f },
        gem::Quaternion<float> { 1.0f, 0.0f, 0.0f, 0.0f },
        gem::Vector<float, 3> { 1.0f, 1.0f, 1.0f }
    );

	auto cc1 = new gel::CameraComponent(
        90.0f, width / height, 0.1f, 1000.0f
    );
    cc1->targetEntity = c3;

    firstCamera->addComponent(cc1);

    auto cc2 = new gel::CameraComponent(
        90.0f, width / height, 0.1f, 1000.0f
    );
    cc2->targetEntity = c3;

    secondCamera = new gel::GameEntity(
        gem::Vector<float, 3> { 0.0f, 5.0f, -5.0f },
        gem::Quaternion<float> { 1.0f, 0.0f, 0.0f, 0.0f },
        gem::Vector<float, 3> { 1.0f, 1.0f, 1.0f }
    );
    secondCamera->addComponent(cc2);

    auto cc3 = new gel::CameraComponent(
        90.0f, width / height, 0.1f, 1000.0f
    );
    cc3->targetEntity = c3;

    thirdCamera = new gel::GameEntity(
        gem::Vector<float, 3> {0.0f, 5.0f, 0.0f },
        gem::Quaternion<float> { 1.0f, 0.0f, 0.0f, 0.0f },
        gem::Vector<float, 3> {1.0f, 1.0f, 1.0f }
    );
    thirdCamera->addComponent(cc3);

	mainScene.addEntity(c2);
	mainScene.addEntity(c3);
	mainScene.addEntity(c5);

	mainScene.addEntity(firstCamera);
    mainScene.addEntity(secondCamera);
	mainScene.addEntity(thirdCamera);

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
            std::cout << "First Camera Active" << std::endl;
            break;
        case GLFW_KEY_2:
            mainScene.setMainCamera(secondCamera);
			std::cout << "Second Camera Active" << std::endl;
            break;
        case GLFW_KEY_3:
            mainScene.setMainCamera(thirdCamera);
            std::cout << "Third Camera Active" << std::endl;
            break;
        }

        mainScene.getMainCamera()->setAspectRatio(float(width) / float(height));
    }
}
