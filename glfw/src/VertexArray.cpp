#include "GL/glew.h"
#include "VertexArray.h"

#include "Renderer.h"


VertexArray::VertexArray() {
	GLDebugger(glGenVertexArrays(1, &m_RendererID);)
	
}
VertexArray::~VertexArray() {
	GLDebugger(glDeleteVertexArrays(1, &m_RendererID);)
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout) {
	//Bind the array
	Bind();
	// bind the buffer
	vb.Bind();
	// set the layout for it
	const auto& TotalElement = layout.GetElement();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < TotalElement.size(); i++)
	{
		const auto& element = TotalElement[i];
		GLDebugger(glEnableVertexAttribArray(i));
		GLDebugger(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset));
		offset += element.GetSizeOfType(element.type)*element.count;
	}
}

void VertexArray::Bind() const {
	GLDebugger(glBindVertexArray(m_RendererID));
}

void VertexArray::UnBind() const {
	GLDebugger(glBindVertexArray(0));
}


