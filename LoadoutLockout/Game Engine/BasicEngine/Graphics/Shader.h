#ifndef __SHADER__
#define __SHADER__
#include "stdafx.h"
/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Shader.h
Purpose: The definition of the Shader class. A Shader class will compile,
and run a vertex and fragment shader and maybe a geometry shader.
For this project the shaders have the file endings v_s, f_s, and g_s. Reason 
for this is to prevent confusion with other file types.
Platform: Windows 10 x86
Project: trevor.harron, Final Project
Author: Trevor C. Harron, trevor.harron, 130000218
Creation date:11/25/2018
- End Header --------------------------------------------------------*/

struct ShaderSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader
{
private:
	std::string m_FilePath;
	unsigned int m_RendererID;
	std::unordered_map<std::string, int> m_LocationCache;

public:
	Shader(const std::string& Filepath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	void setBool(const std::string &name, bool value) ;
	void setUniform1i(const std::string& name, int value);
	void setUniform1f(const std::string& name, float value);
	void setUniformmat4f(const std::string& name, const glm::mat4& matrix);
	void setUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void setUniform3f(const std::string& name, float v0, float v1, float v2);
	void setUniform2f(const std::string& name, float v0, float v1);
	unsigned int GetID() { return m_RendererID; }

private:

	ShaderSource ParseShader(const std::string& source);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	int getUniformLocation(const std::string& name);
};

#endif