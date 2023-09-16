#include "GL/glew.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
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
		// so bascially, in each vertext, we have multiple attributes like position, color, texture
		// tell opengl what the layout of our data
		// index means the index of attribute in your buffer that you want to use 
		// let say, we have position in index 0, color in index 1, texture in index 2, so the index 0 means we 
		// get the position
		// the size, means how many float that we are providing, in the position, we want to make a 2d graphic
		// hence, the size will be 2
		// normalize means if you want to trasfer the number (like the color, from 0 to 255) to floating number
		// stride means how many bytes between each vertex (since, we have many things in one vertix, like color, texture, position)
		// pointer means, in a vertext, we have so many things right, so from position to color, (let say we have two floating number to represent the position, and one number represent the color)
		// we all know that in 32-bit system, 4 bytes represent a pointer, hence to get to the pointer of that color(which is the next vertex), we need 8
		// in this case, the position is the first attribute, hence the pointer would be (const void*)0
		// if we want to get to the color, the pointer would be (const void*)8
		// for function glVertexAttribPointer
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


