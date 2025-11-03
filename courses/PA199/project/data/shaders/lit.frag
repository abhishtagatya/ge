#version 430 core

out vec4 FragColor;

struct DirLight {
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

uniform vec3 view_pos;
uniform vec3 unlit_color;
uniform int use_texture;
uniform sampler2D tex0;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir) {
	vec3 lightDir = normalize(-light.direction);

	// diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);

	// specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);

	// combine results
	vec3 ambient = light.ambient;
	vec3 diffuse = light.diffuse * diff;
	vec3 specular = light.specular * spec;
	return (ambient + diffuse + specular);
}

void main() {
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(view_pos - FragPos);

	DirLight dirLight;
	dirLight.direction = vec3(-0.2, -1.0, -0.3);
	dirLight.ambient = vec3(0.2, 0.2, 0.2);
	dirLight.diffuse = vec3(0.5, 0.5, 0.5);
	dirLight.specular = vec3(1.0, 1.0, 1.0);

	vec3 result = CalcDirLight(dirLight, norm, viewDir);

	if (use_texture == 1) {
		FragColor = vec4(result * texture(tex0, TexCoord).rgb, 1.0);
        return;
    }

	FragColor = vec4(result * unlit_color, 1.0);
}