#version 430 core

in vec2 TexCoord;

out vec4 FragColor;

uniform vec3 unlit_color;
uniform int use_texture;
uniform sampler2D tex0;

uniform float breakpoint;

void main() {
    vec3 breakpoint_interp = ((1.0 - breakpoint) * vec3(0.1, 0.0, 0.0));

    if (use_texture == 1) {
        FragColor = vec4(texture(tex0, TexCoord).rgb + breakpoint_interp, 1.0);
        return;
    }

    FragColor = vec4(unlit_color + breakpoint_interp, 1.0);
}
