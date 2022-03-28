#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"
#include<iostream>

Texture::Texture(const std::string& filePath)
	:m_RendererID(0), m_FilePath(filePath), m_LocationBuffer(nullptr)
	, m_Width(0), m_Height(0), m_nrComponents(0) {

	GLCALL(glGenTextures(1, &m_RendererID));

	stbi_set_flip_vertically_on_load(1);
	m_LocationBuffer = stbi_load(filePath.c_str(), &m_Width, &m_Height, &m_nrComponents, 0);
	if (m_LocationBuffer) {

		GLenum format;
		if (m_nrComponents == 1)
			format = GL_RED;
		else if (m_nrComponents == 3)
			format = GL_RGB;
		else if (m_nrComponents == 4)
			format = GL_RGBA;

		GLCALL(glBindTexture(GL_TEXTURE_2D, m_RendererID));

		GLCALL(glTexImage2D(GL_TEXTURE_2D, 0, format, m_Width, m_Height, 0,
			format, GL_UNSIGNED_BYTE, m_LocationBuffer));
		GLCALL(glGenerateMipmap(GL_TEXTURE_2D));
		GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
		GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
		GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

		
	}
	else {
		std::cout << "Texture failed to load at path: " << m_FilePath << std::endl;
		stbi_image_free(m_LocationBuffer);
	}
}
		

Texture::~Texture(){
	GLCALL(glDeleteTextures(1, &m_RendererID));
}

void Texture::Bind(unsigned int slot) const{
	GLCALL(glActiveTexture(GL_TEXTURE0 + slot));
	GLCALL(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void Texture::UnBind() const{
	GLCALL(glBindTexture(GL_TEXTURE_2D, 0));
}
