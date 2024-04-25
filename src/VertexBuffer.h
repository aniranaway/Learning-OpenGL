#pragma once

class VertexBuffer
{
private:
	unsigned int m_RendererID; /* Numeric ID for every object type created */

public:
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;
};
