#shader vertex
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 WorldPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    WorldPos = vec3(model * vec4(aPos, 1.0));
    //∑®œﬂæÿ’Û
    Normal = mat3(transpose(inverse(model))) * aNormal;  
    TexCoords = aTexCoords;
    
    gl_Position = projection * view * vec4(WorldPos, 1.0);
}

#shader fragment
#version 330 core

out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;   
    sampler2D emission;
    float shininess;
}; 

struct Light {
    vec3 lightPos;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

in vec3 WorldPos;  
in vec3 Normal;  
in vec2 TexCoords;
  
uniform vec3 viewPos;
uniform Material material;
uniform Light light;
uniform vec3 matrixLight;
uniform float matrixMove;

void main()
{
    // ambient
    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.lightPos-WorldPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;  
    
    // specular
    vec3 viewDir = normalize(viewPos - WorldPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular *spec * texture(material.specular,TexCoords).rgb;  

    //vec3 emission = matrixLight * texture(material.emission,
        //vec2(TexCoords.x,TexCoords.y+matrixMove)).rgb;

    float distance = length(light.lightPos-WorldPos);
    float attenuation = 1.0/(light.constant+light.linear*distance+light.quadratic*distance*distance);
        
    vec3 result = (ambient + diffuse + specular) *attenuation;
    FragColor = vec4(result, 1.0);
} 