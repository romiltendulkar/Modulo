#pragma once
#include "stdafx.h"
class IndexBuffer
{
private:
	unsigned int m_rendererID;
	unsigned int m_count;
public:
	IndexBuffer(const unsigned* data, unsigned int count);
	~IndexBuffer();

	void Bind() const;
	void UnBind() const;

	inline unsigned int GetCount() const { return m_count; }
};