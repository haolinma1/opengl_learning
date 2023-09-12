#include <GL/glew.h>
#include "VertexBuffer.h"
#include "Renderer.h"


VertexBuffer::VertexBuffer(const void* data, unsigned int size) {
    // generate buffer for gpu to work with
    // the buffer means the id that represent this buffer.
    // so we can just pass in the integet buffer that represent the buffer that we are going to generate
    GLDebugger(glGenBuffers(1, &m_RenderID));
    // select the buffer that we have created
    // this is the array of buffer, buffer is the id.
    // GL_ARRAY_BUFFER target means that binding the buffer for vertex attribute data
    GLDebugger(glBindBuffer(GL_ARRAY_BUFFER, m_RenderID));
    // insert the data to this buffer.
    // opengl know which buffer it need to insert the data by the first argument, which is GL_ARRAY_BUFFER 
    GLDebugger(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer() {
    GLDebugger(glDeleteBuffers(1, &m_RenderID));
}

void VertexBuffer::Bind()const {
    GLDebugger(glBindBuffer(GL_ARRAY_BUFFER, m_RenderID));
}

void VertexBuffer::Unbind()const {
    GLDebugger(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
