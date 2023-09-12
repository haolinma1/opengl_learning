#include <GL/glew.h>
#include "IndexBuffer.h"
#include "Renderer.h"

IndexBuffer::IndexBuffer(const unsigned int* data, int count) {
    m_count = count;
    GLDebugger(glGenBuffers(1, &m_ShaderID));
    // GL_ELEMENT_ARRAY_BUFFER target is used for index data that defines the order in which vertices are rendered 
    GLDebugger(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ShaderID));
    GLDebugger(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_count *sizeof(unsigned int), data, GL_STATIC_DRAW));
}
IndexBuffer::~IndexBuffer() {
    GLDebugger(glDeleteBuffers(1, &m_ShaderID));
}
void IndexBuffer::Bind()const{
    GLDebugger(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ShaderID));
}

void IndexBuffer::Unbind()const {
    GLDebugger(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0))
}
