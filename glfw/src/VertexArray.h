#pragma once
#include"VertexBuffer.h"

// don't include the VertexBufferLayout header file 
// because we need to prevent cyclic includes 
// in header file, it don't need to actually need to know the VertexBufferLayout class.
class VertexBufferLayout;
class VertexArray
{
private:
	unsigned int m_RendererID;

public:
	VertexArray();
	~VertexArray();

	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

	void Bind()const;
	void UnBind()const;


};

