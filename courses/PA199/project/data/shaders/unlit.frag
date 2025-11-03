#version 430 core

in vec2 TexCoord;

out vec4 FragColor;

uniform vec3 unlit_color;
uniform int use_texture;
uniform sampler2D tex0;

void main() {
    if (use_texture == 1) {
        FragColor = texture(tex0, TexCoord);
        return;
    }

    FragColor = vec4(unlit_color, 1.0);
}
