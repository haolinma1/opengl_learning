#pragma once
#include "GL/glew.h"
#include <vector>
#include <assert.h>
#include "Renderer.h"
//  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (const void*)0);
// in the vertex array, from zero to a number, we have multiple format for the array


struct VerTextBufferElement  {
	// in one index of vertex array
	
	unsigned int type;
	unsigned int count;
	unsigned char normalized;

	static unsigned int GetSizeOfType(unsigned int type) {
		switch (type)
		{
		case GL_FLOAT: return 4;
		case GL_UNSIGNED_INT: return 4;
		case GL_BYTE: return 1;
		}
		ASSERT(false);
		return 0;
	}
};

class VertexBufferLayout {
private:
	// in this verctor, each element is for one index in the vertex array
	std::vector<VerTextBufferElement> m_Element;
	unsigned int m_Stride;
public:

	VertexBufferLayout() { m_Stride = 0; };

	template<typename T>
	void Push(unsigned int count) {
		assert(false);
	}

	template<>
	void Push<float>(unsigned int count) {
		m_Element.push_back({ GL_FLOAT, count, GL_FALSE });
		// 4 bytes
		m_Stride += VerTextBufferElement::GetSizeOfType(GL_FLOAT)*count;
	}

	template<>
	void Push<unsigned int>(unsigned int count) {
		m_Element.push_back({ GL_UNSIGNED_INT, count, GL_FALSE});
		m_Stride += VerTextBufferElement::GetSizeOfType(GL_UNSIGNED_INT) * count;
	}

	template<>
	void Push<unsigned char>(unsigned int count) {
		m_Element.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
		m_Stride += VerTextBufferElement::GetSizeOfType(GL_BYTE) * count;
	}
	
	inline const std::vector<VerTextBufferElement>& GetElement()const { return m_Element; }
	inline unsigned int GetStride()const { return m_Stride; }
};
