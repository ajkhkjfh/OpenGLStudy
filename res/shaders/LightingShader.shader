#shader vertex
#version 330 core

layout(location=0) in vec3 aPos;
layout(location=1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 Normal;
out vec3 worldPos;

void main(){
	worldPos = vec3(model*vec4(aPos,1.0f));
	//∑®œﬂæÿ’Û
	Normal = mat3(transpose(inverse(model)))*aNormal;
	gl_Position = projection*view*vec4(aPos,1.0f);
}

#shader fragment
#version 330 core

struct Material{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 Normal;
in vec3 worldPos;

out vec4 FragColor;

uniform vec3 viewPos;
uniform vec3 lightPos;
//uniform vec3 objectColor;
//uniform vec3 lightColor;
uniform Material material;
uniform Light light;

void main(){
	

	//float ambientStrength = 0.01f;

	//ambient
	vec3 ambient = light.ambient*material.ambient;

	//diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - worldPos);
	float diff = max(dot(lightDir,norm),0.0f);
	vec3 diffuse = diff*(light.diffuse*material.diffuse);

	//specular
	//float specularStrength = 0.5f;
	vec3 viewDir = normalize(viewPos - worldPos);
	vec3 reflectDir = reflect(-lightDir,norm);
	float spec = pow(max(dot(viewDir,reflectDir),0.0f),material.shininess);
	vec3 specular = light.specular*(spec*material.specular);

	
	vec3 result = diffuse+ambient+specular;
	FragColor = vec4(result,1.0f);
}
