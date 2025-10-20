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
    , vertex_shader(load_shader(lecture_folder_path / "data" / "shaders" / "sample.vert", GL_VERTEX_SHADER))
    , fragment_shader(load_shader(lecture_folder_path / "data" / "shaders" / "sample.frag", GL_FRAGMENT_SHADER))
    , shader_program([](GLuint const  vertex_shader, GLuint const  fragment_shader) -> GLuint {
            return create_shader_program(vertex_shader, fragment_shader);
            }(vertex_shader, fragment_shader))
    , vertex_arrays([]() -> GLuint {
            GLuint vertex_arrays;
            glGenVertexArrays(1, &vertex_arrays);
            glBindVertexArray(vertex_arrays);
            return vertex_arrays;
            }())
    , vertex_buffer([]() -> GLuint {
            struct Vertex { float x,y,z,u,v; };
            std::vector<Vertex> const vertices {
                { -0.5f, -0.125f, 0.0f,       0.0f, 0.0f },
                {  0.5f, -0.125f, 0.0f,       1.0f, 0.0f },
                {  0.5f,  0.125f, 0.0f,       1.0f, 1.0f },
                { -0.5f,  0.125f, 0.0f,       0.0f, 1.0f },
            };
            GLuint vertex_buffer;
            glGenBuffers(1, &vertex_buffer);
            assert(glGetError() == 0U);
            glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
            assert(glGetError() == 0U);
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices.front(), GL_STATIC_DRAW);
            assert(glGetError() == 0U);
            glEnableVertexAttribArray(0);
            assert(glGetError() == 0U);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
            assert(glGetError() == 0U);
            glEnableVertexAttribArray(1);
            assert(glGetError() == 0U);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3*sizeof(float)));
            assert(glGetError() == 0U);
            return vertex_buffer;
            }())
    , index_buffer([]() -> GLuint {
            std::vector<GLuint> const indices { 0U, 1U, 2U,     0U, 2U, 3U };
            GLuint index_buffer;
            glGenBuffers(1, &index_buffer);
            assert(glGetError() == 0U);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
            assert(glGetError() == 0U);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices.front(), GL_STATIC_DRAW);
            assert(glGetError() == 0U);
            return index_buffer;
            }())
    , texture(load_texture(lecture_folder_path / "data" / "textures" / "you_win.png"))
{
    glViewport(0, 0, width, height);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    default_vertex_shader = load_shader(lecture_folder_path / "data" / "shaders" / "default.vert", GL_VERTEX_SHADER);
    default_fragment_shader = load_shader(lecture_folder_path / "data" / "shaders" / "default.frag", GL_FRAGMENT_SHADER);
    default_shader_program = create_shader_program(default_vertex_shader, default_fragment_shader);

    std::vector<MeshRendererVAO> cubeVertices = {
        // Positions          Normals         UVs
        // Front face
        { -0.5f, -0.5f,  0.5f, 0,0,1, 0,0 },
        {  0.5f, -0.5f,  0.5f, 0,0,1, 1,0 },
        {  0.5f,  0.5f,  0.5f, 0,0,1, 1,1 },
        { -0.5f,  0.5f,  0.5f, 0,0,1, 0,1 },
        // Back face
        { -0.5f, -0.5f, -0.5f, 0,0,-1, 0,0 },
        {  0.5f, -0.5f, -0.5f, 0,0,-1, 1,0 },
        {  0.5f,  0.5f, -0.5f, 0,0,-1, 1,1 },
        { -0.5f,  0.5f, -0.5f, 0,0,-1, 0,1 },
        // Left face
        { -0.5f, -0.5f, -0.5f, -1,0,0, 0,0 },
        { -0.5f, -0.5f,  0.5f, -1,0,0, 1,0 },
        { -0.5f,  0.5f,  0.5f, -1,0,0, 1,1 },
        { -0.5f,  0.5f, -0.5f, -1,0,0, 0,1 },
        // Right face
        { 0.5f, -0.5f, -0.5f, 1,0,0, 0,0 },
        { 0.5f, -0.5f,  0.5f, 1,0,0, 1,0 },
        { 0.5f,  0.5f,  0.5f, 1,0,0, 1,1 },
        { 0.5f,  0.5f, -0.5f, 1,0,0, 0,1 },
        // Top face
        { -0.5f, 0.5f,  -0.5f, 0,1,0, 0,0 },
        {  0.5f, 0.5f,  -0.5f, 0,1,0, 1,0 },
        {  0.5f, 0.5f,   0.5f, 0,1,0, 1,1 },
        { -0.5f, 0.5f,   0.5f, 0,1,0, 0,1 },
        // Bottom face
        { -0.5f, -0.5f,  -0.5f, 0,-1,0, 0,0 },
        {  0.5f, -0.5f,  -0.5f, 0,-1,0, 1,0 },
        {  0.5f, -0.5f,   0.5f, 0,-1,0, 1,1 },
        { -0.5f, -0.5f,   0.5f, 0,-1,0, 0,1 },
    };

    std::vector<unsigned int> cubeIndices = {
        0,1,2, 0,2,3,       // front
        4,5,6, 4,6,7,       // back
        8,9,10, 8,10,11,    // left
        12,13,14, 12,14,15, // right
        16,17,18, 16,18,19, // top
        20,21,22, 20,22,23  // bottom
    };

	auto testEntity = new gel::GameEntity(
        gem::Vector<float, 3> { 0.0f, 0.0f, 0.0f },
        gem::Vector<float, 3> { M_PI / 4.0f, M_PI / 4.0f, 0.0f },
        gem::Vector<float, 3> { 1.0f, 1.0f, 1.0f }
    );
    auto planeRenderComponent = new gel::MeshRendererComponent(
        cubeVertices,
        cubeIndices,
        default_shader_program
    );
	testEntity->addComponent(planeRenderComponent);

	auto cameraEntity = new gel::GameEntity(
        gem::Vector<float, 3> { 0.0f, 0.0f, 10.0f },
        gem::Vector<float, 3> { 0.0f, 0.0f, 0.0f },
        gem::Vector<float, 3> { 1.0f, 1.0f, 1.0f }
    );

    std::cout << width << std::endl;
    std::cout << height << std::endl;

	auto cameraComponent = new gel::CameraComponent(
        45.0f, width / height, 0.1f, 1000.0f
    );
    cameraComponent->targetEntity = testEntity;
    cameraEntity->addComponent(cameraComponent);

	mainScene.addEntity(testEntity);
	mainScene.addEntity(cameraEntity);

	mainScene.setMainCamera(cameraComponent);
}

Application::~Application()
{

    glDeleteVertexArrays(1U, &vertex_arrays);
    glDeleteTextures(1, &texture);
    glDeleteProgram(shader_program);
    glDeleteShader(fragment_shader);
    glDeleteShader(vertex_shader);
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

    glDisable(GL_CULL_FACE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // see edges
    mainScene.render();
}

void Application::render_ui() {}

void Application::on_resize(int width, int height) {
    // Calls the default implementation to set the class variables.
    IApplication::on_resize(width, height);
    // Changes the viewport.
    glViewport(0, 0, width, height);
}

void Application::on_mouse_move(double x, double y) {}

void Application::on_mouse_button(int button, int action, int mods) {}

void Application::on_key_pressed(int key, int scancode, int action, int mods) {
    /*
    if (action == GLFW_PRESS) {
        red = 0;
        green = 0;
        blue = 0;
        switch (key) {
        case GLFW_KEY_R:
            red = 1;
            break;
        case GLFW_KEY_G:
            green = 1;
            break;
        case GLFW_KEY_B:
            blue = 1;
            break;
        }
    }
    */
}
