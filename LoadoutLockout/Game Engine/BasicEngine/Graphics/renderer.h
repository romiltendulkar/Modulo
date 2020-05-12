#pragma once
#include "stdafx.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

#define ASSERT(x) if(!(x)) __debugbreak(); 
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLCallLog(#x, __FILE__, __LINE__))


void GLClearError();

bool GLCallLog(const char* function, const char* file, int line);

class renderer
{
public:
	void Clear()const;
	void Draw(const VertexArray& va, const IndexBuffer* ib, const Shader* shader) const;
	void DebugDraw(const VertexArray& va, const IndexBuffer* ib, const Shader* shader) const;
	//void DebugDrawCircle(const VertexArray& va, int number, const Shader* shader) const;
};




