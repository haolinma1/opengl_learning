#pragma once
class IndexBuffer
{
private:
	// get this buffer id
	unsigned int m_ShaderID;
	// get the number of point
	unsigned int m_count;
public:
	IndexBuffer(const unsigned int* data,int count);
	~IndexBuffer();
	void Bind()const;
	void Unbind()const;

	inline unsigned int GetCount()const {
		return m_count;
	}
};

