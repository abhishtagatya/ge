#version 430 core

#define NR_POINT_LIGHTS 4

out vec4 FragColor;

struct DirLight {
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight {
    vec3 position;
    
	float range;
    float constant;
    float linear;
    float quadratic;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

uniform vec3 view_pos;
uniform DirLight mainLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform int num_point_lights;

uniform vec3 unlit_color;
uniform int use_texture;

uniform sampler2D tex0;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir) {
	vec3 lightDir = normalize(-light.direction);

	float diff = max(dot(normal, lightDir), 0.0);

	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);

	vec3 ambient = light.ambient;
	vec3 diffuse = light.diffuse * diff;
	vec3 specular = light.specular * spec;
	return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    float rangeFactor = clamp(1.0 - pow(distance / light.range, 4.0), 0.0, 1.0);
    attenuation *= rangeFactor;

    vec3 ambient = light.ambient;
    vec3 diffuse = light.diffuse * diff;
    vec3 specular = light.specular * spec;

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}


void main() {
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(view_pos - FragPos);

	vec3 result = CalcDirLight(mainLight, norm, viewDir);
	for(int i = 0; i < num_point_lights; i++)
		result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);

	if (use_texture == 1) {
		FragColor = vec4(result * texture(tex0, TexCoord).rgb, 1.0);
        return;
    }

	FragColor = vec4(result * unlit_color, 1.0);
	
    // Debug Light
    //FragColor = vec4(result, 1.0);
}