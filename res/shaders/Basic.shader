#shader vertex
#version 330 core

layout (location = 0) in vec2 aPos;
layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoord;

void main(){
   gl_Position = vec4(aPos.x,aPos.y,0.0f,1.0f);
   v_TexCoord = texCoord;
};

#shader fragment
#version 330 core

in vec2 v_TexCoord;

out vec4 FragColor;

uniform vec4 u_Color;
uniform sampler2D u_Texture1;

void main(){
	vec4 texColor = texture(u_Texture1,v_TexCoord);	
    FragColor = texColor;
};