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

static GLuint create_shader_program(GLuint const  vertex_shader, GLuint const fragment_shader) {
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

    GLuint unlit_vtx_shader = load_shader(lecture_folder_path / "data" / "shaders" / "unlit.vert", GL_VERTEX_SHADER);
    GLuint unlit_frg_shader = load_shader(lecture_folder_path / "data" / "shaders" / "unlit.frag", GL_FRAGMENT_SHADER);
    GLuint unlit_program = create_shader_program(unlit_vtx_shader, unlit_frg_shader);

    GLuint lit_vtx_shader = load_shader(lecture_folder_path / "data" / "shaders" / "lit.vert", GL_VERTEX_SHADER);
    GLuint lit_frg_shader = load_shader(lecture_folder_path / "data" / "shaders" / "lit.frag", GL_FRAGMENT_SHADER);
    GLuint lit_program = create_shader_program(lit_vtx_shader, lit_frg_shader);

    mainScene.addShaderResource("UNLIT", unlit_vtx_shader, unlit_frg_shader, unlit_program);
    mainScene.addShaderResource("LIT", lit_vtx_shader, lit_frg_shader, lit_program);

	GLuint grass_texture = load_texture(lecture_folder_path / "data" / "textures" / "grass.png");
	GLuint grey_moss_texture = load_texture(lecture_folder_path / "data" / "textures" / "grey_moss.png");
	GLuint black_moss_texture = load_texture(lecture_folder_path / "data" / "textures" / "black_moss.png");
	GLuint green_moss_texture = load_texture(lecture_folder_path / "data" / "textures" / "green_moss.png");
	GLuint brown_moss_texture = load_texture(lecture_folder_path / "data" / "textures" / "brown_moss.png");
	GLuint metal_texture = load_texture(lecture_folder_path / "data" / "textures" / "metal.png");

    auto ball_sphere = new gel::SphereRendererComponent(0.5f, 9, 9, metal_texture);
	auto platform_circle = new gel::CircleRendererComponent(1.75f, 32, grass_texture);

    auto ball = new gel::GameEntity(
		gem::Vector<float, 3> { 0.0f, -0.25f, -2.5f },
		gem::Quaternion<float> { 1.0f, 0.0f, 0.0f, 0.0f },
		gem::Vector<float, 3> { 1.0f, 1.0f, 1.0f } * 0.5f
    );
    ball->addComponent(ball_sphere);
    auto ball_rigidbody = new gel::RigidbodyComponent(10.0f);
	ball_rigidbody->applyImpulse(gem::Vector<float, 3> { 0.0f, 0.0f, 0.05f });
    ball->addComponent(ball_rigidbody);
	ball->addComponent(new gel::BallResetComponent(ball->getPosition()));

    int layer = 3;
	int blocks_per_layer = 6;
	float offset = 0.5f;
    float ringAngle = (float)(M_PI * 2.0f) / blocks_per_layer;

    auto arcBlack = new gel::ArcRendererComponent(1.0f, 1.5f, 16, 0.5f, ringAngle, black_moss_texture);
    auto arcGreen = new gel::ArcRendererComponent(1.0f, 1.5f, 16, 0.5f, ringAngle, green_moss_texture);

    for (int i = 0; i < layer; i++) {
        for (int j = 0; j < blocks_per_layer; j++) {
            auto block = new gel::GameEntity(
                gem::Vector<float, 3> { 0.0f, -0.25f + (i * offset), 0.0f },
                gem::AxisAngle{ (ringAngle * i) + (ringAngle * j), 0.0f, 1.0f, 0.0f }.toQuaternion(),
                gem::Vector<float, 3> { 1.0f, 1.0f, 1.0f }
            );

            if (j % 2 == 0)
                block->addComponent(arcBlack);
			else
                block->addComponent(arcGreen);

            mainScene.addEntity(block);
		}
    }

    auto paddle_ring_a = new gel::ArcRendererComponent(4.25f, 5.0f, 16, 0.5f, (float)(M_PI * 2.0f) / 8.0f, brown_moss_texture);
    auto paddle_ring_b = new gel::ArcRendererComponent(4.25f, 5.0f, 16, 0.5f, (float)(M_PI * 2.0f) / 8.0f, brown_moss_texture);
    auto paddle_a = new gel::GameEntity(
        gem::Vector<float, 3> { 0.0f, -0.25f, 0.0f },
        gem::AxisAngle{ 1.0f, 0.0f, 0.0f, 0.0f }.toQuaternion(),
        gem::Vector<float, 3> { 1.0f, 1.0f, 1.0f }
    );
	paddle_a->addComponent(paddle_ring_a);
	paddle_a->addComponent(new gel::PaddleControllerComponent());

    auto paddle_b = new gel::GameEntity(
        gem::Vector<float, 3> { 0.0f, -0.25f, 0.0f },
        gem::AxisAngle{ (float)(M_PI), 0.0f, 1.0f, 0.0f}.toQuaternion(),
        gem::Vector<float, 3> { 1.0f, 1.0f, 1.0f }
    );
    paddle_b->addComponent(paddle_ring_b);
	paddle_b->addComponent(new gel::PaddleControllerComponent());

    auto platform = new gel::GameEntity(
        gem::Vector<float, 3> { 0.0f, -0.5f, 0.0f },
        gem::Quaternion<float> { 1.0f, 0.0f, 0.0f, 0.0f },
		gem::Vector<float, 3> { 1.0f, 1.0f, 1.0f } * 3.0f
    );
    platform->addComponent(platform_circle);

	firstCamera = new gel::GameEntity(
        gem::Vector<float, 3> { 0.0f, 0.0f, -7.0f },
        gem::Quaternion<float> { 1.0f, 0.0f, 0.0f, 0.0f },
        gem::Vector<float, 3> { 1.0f, 1.0f, 1.0f }
    );
	auto cc1 = new gel::CameraComponent(
        90.0f, width / height, 0.1f, 1000.0f
    );
    cc1->targetEntity = platform;
    firstCamera->addComponent(cc1);

    secondCamera = new gel::GameEntity(
        gem::Vector<float, 3> { 0.0f, 7.0f, -7.0f },
        gem::Quaternion<float> { 1.0f, 0.0f, 0.0f, 0.0f },
        gem::Vector<float, 3> { 1.0f, 1.0f, 1.0f }
    );
    auto cc2 = new gel::CameraComponent(
        90.0f, width / height, 0.1f, 1000.0f
    );
    cc2->targetEntity = platform;
    secondCamera->addComponent(cc2);

    thirdCamera = new gel::GameEntity(
        gem::Vector<float, 3> {0.0f, 7.0f, 0.0f },
        gem::Quaternion<float> { 1.0f, 0.0f, 0.0f, 0.0f },
        gem::Vector<float, 3> {1.0f, 1.0f, 1.0f }
    );
    auto cc3 = new gel::CameraComponent(
        90.0f, width / height, 0.1f, 1000.0f
    );
    cc3->targetEntity = platform;
    thirdCamera->addComponent(cc3);

    auto sunLight = new gel::DirectionalLightComponent(
        gem::Vector<float, 3> {0.2f, 0.2f, 0.2f },
        gem::Vector<float, 3> {0.8f, 0.8f, 0.8f },
        gem::Vector<float, 3> {1.0f, 1.0f, 1.0f }
    );

    auto directLight = new gel::GameEntity(
        gem::Vector<float, 3> { 0.0f, 5.0f, -5.0f },
		gem::AxisAngle{ (float)(-M_PI / 8.0f), 1.0f, 0.0f, 0.0f }.toQuaternion(),
        gem::Vector<float, 3> { 1.0f, 1.0f, 1.0f }
	);
    directLight->addComponent(sunLight);
	mainScene.addEntity(directLight);

    auto l1 = new gel::PointLightComponent(
        gem::Vector<float, 3> {0.4f, 0.4f, 0.4f },
        gem::Vector<float, 3> {0.8f, 0.8f, 0.8f },
        gem::Vector<float, 3> {1.0f, 1.0f, 1.0f },
        10.0f
    );
    auto pointLight = new gel::GameEntity(
        gem::Vector<float, 3> {0.0f, 7.0f, 0.0f },
        gem::Quaternion<float> { 1.0f, 0.0f, 0.0f, 0.0f },
        gem::Vector<float, 3> {1.0f, 1.0f, 1.0f }
	);
	pointLight->addComponent(l1);

    auto collisionManager = new gel::GameEntity();
    collisionManager->addComponent(new gel::AdhocPaddleBroadphaseCollisionComponent(
        ball, paddle_a, paddle_b
    ));

	mainScene.addEntity(pointLight);
	mainScene.addExtraLight(l1);

	mainScene.addEntity(ball);
	mainScene.addEntity(platform);

    mainScene.addEntity(paddle_a);
    mainScene.addEntity(paddle_b);
	mainScene.addEntity(collisionManager);

	mainScene.addEntity(firstCamera);
    mainScene.addEntity(secondCamera);
	mainScene.addEntity(thirdCamera);

	mainScene.setMainCamera(firstCamera);
	mainScene.setMainLight(sunLight);
}

Application::~Application()
{}

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

    glDisable(GL_CULL_FACE);
	//glCullFace(GL_BACK);h
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

	mainScene.handleKeyPressed(key, scancode, action, mods);

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
        case GLFW_KEY_9:
            mainScene.useShaderProgram("UNLIT");
            std::cout << "Unlit Shader Active" << std::endl;
            break;
        case GLFW_KEY_0:
            mainScene.useShaderProgram("LIT");
            std::cout << "Lit Shader Active" << std::endl;
            break;
        }

        mainScene.getMainCamera()->setAspectRatio(float(width) / float(height));
    }
}
