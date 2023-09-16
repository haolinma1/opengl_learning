#pragma once
#include "Renderer.h"
class Texture
{
private:
	unsigned int m_RendererID{};
	std::string m_FilePath;
	unsigned char* m_LocalBuffer{};
	// BPP stands for bytes  per pixel
	int m_width{}, m_hight{}, m_BPP{};
public:
	Texture(const std::string& FilePath);
	~Texture();

	void Bind(unsigned int slot=0)const;
	void UnBind()const;

	inline int GetWidth()const { return m_width; }
	inline int GetHight()const { return m_hight; }
};

