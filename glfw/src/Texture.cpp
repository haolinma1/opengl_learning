#include "GL/glew.h"
#include <iostream>
#include "Texture.h"
#include "vendor/stb_image/stb_image.h"

Texture::Texture(const std::string& FilePath):m_FilePath(FilePath) {
	
	stbi_set_flip_vertically_on_load(1);
	// 4 stands for rgba
	// the path star from the solution file, what a joke
	m_LocalBuffer = stbi_load(FilePath.c_str(), &m_width, &m_hight, &m_BPP, 4);
	if (stbi_failure_reason())
	{
		std::cout << "Unsuccessfully get the image: ";
		std::cout << stbi_failure_reason();
	}
	// first gen Texture
	GLDebugger(glGenTextures(1, &m_RendererID));
	GLDebugger(glBindTexture(GL_TEXTURE_2D, m_RendererID));
	// set the texture parameter
	// must set these four parameter
	GLDebugger(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLDebugger(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLDebugger(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLDebugger(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	// interal format means how the opengl will store the format
	// the next format is you will provide for opengl
	GLDebugger(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8/*8 stands for each actual bit per channel*/, m_width, m_hight, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));
	GLDebugger(glBindTexture(GL_TEXTURE_2D, 0));


	if (m_LocalBuffer)
	{
		stbi_image_free(m_LocalBuffer);
	}
}
Texture::~Texture() {
	GLDebugger(glDeleteTextures(1,&m_RendererID));

}

void Texture::Bind(unsigned int slot)const {
	// 0 stands for the first slot
	GLDebugger(glActiveTexture(GL_TEXTURE0+slot));
	GLDebugger(glBindTexture(GL_TEXTURE_2D, m_RendererID));

}
void Texture::UnBind()const {
	GLDebugger(glBindTexture(GL_TEXTURE_2D, 0));
}
