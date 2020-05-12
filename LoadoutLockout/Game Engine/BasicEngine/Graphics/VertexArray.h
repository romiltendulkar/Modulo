#pragma once
#include "stdafx.h"
#include "VertexBuffer.h"

class VertexBufferLayout;
class VertexArray
{
private:
	unsigned int RendererID;

public:
	VertexArray();
	~VertexArray();

	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
	void Bind() const;
	void Unbind() const;

};

