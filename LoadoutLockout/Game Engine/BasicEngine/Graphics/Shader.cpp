#include "stdafx.h"
#include "Shader.h"
#include "renderer.h"

/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Shader.cpp
Purpose: The implementation of the Shader class. This is based on the 
implementaion of the shader class in https://learnopengl.com/.
Platform: Windows 10 x86
Project: trevor.harron, Final Project
Author: Trevor C. Harron, trevor.harron, 130000218
Creation date:11/25/2018
- End Header --------------------------------------------------------*/


Shader::Shader(const std::string & Filepath) :m_FilePath(Filepath), m_RendererID(0)
{
	ShaderSource Source = ParseShader(Filepath);
	m_RendererID = CreateShader(Source.VertexSource, Source.FragmentSource);
}

Shader::~Shader()
{
	GLCall(glDeleteProgram(m_RendererID));
}



ShaderSource Shader::ParseShader(const std::string& source)
{
	std::ifstream stream(source);

	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;
	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
			{
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos)
			{
				type = ShaderType::FRAGMENT;
			}
		}
		else
		{
			ss[(int)type] << line << '\n';
		}

	}

	return{ ss[0].str(), ss[1].str() };
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);

	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* msg = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, msg);
		std::cout << "failed to compile" << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "shader !" << std::endl;
		std::cout << msg << std::endl;
		glDeleteShader(id);
		return 0;
	}
	return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);

	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

int Shader::getUniformLocation(const std::string & name)
{
	if (m_LocationCache.find(name) != m_LocationCache.end())
		return m_LocationCache[name];
	int location = glGetUniformLocation(m_RendererID, name.c_str());
	if (location == -1)
	{
		std::cout << "Warning doesnt exist" << name << "Doesnt exist!" << std::endl;
	}

	m_LocationCache[name] = location;
	return location;
}

void Shader::Bind() const
{
	glUseProgram(m_RendererID);
}

void Shader::Unbind() const
{
	GLCall(glUseProgram(0));
}
void Shader::setBool(const std::string & name, bool value) 
{
	unsigned int loc = getUniformLocation(name);
	glUniform1i(loc, value);
}
void Shader::setUniform1i(const std::string & name, int value)
{
	unsigned int loc = getUniformLocation(name);
	GLCall(glUniform1i(loc, value));
}

void Shader::setUniform1f(const std::string & name, float value)
{
	GLCall(glUniform1f(getUniformLocation(name), value));
}

void Shader::setUniformmat4f(const std::string & name, const glm::mat4 & matrix)
{
	unsigned int loc = getUniformLocation(name);
	GLCall(glUniformMatrix4fv(loc, 1, GL_FALSE, &matrix[0][0]));
}

void Shader::setUniform4f(const std::string & name, float v0, float v1, float v2, float v3)
{
	GLCall(glUniform4f(getUniformLocation(name), v0, v1, v2, v3));
}

void Shader::setUniform3f(const std::string & name, float v0, float v1, float v2)
{
	GLCall(glUniform3f(getUniformLocation(name), v0, v1, v2));
}

void Shader::setUniform2f(const std::string & name, float v0, float v1)
{
	GLCall(glUniform2f(getUniformLocation(name), v0, v1));
}
