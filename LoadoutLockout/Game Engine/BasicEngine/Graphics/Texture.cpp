#include "stdafx.h"
#include "Texture.h"
#include "..\Managers\FileManager.h"

/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Texture.cpp
Purpose: The implementaion of the Texture class.
Platform: Windows 10 x86
Project: trevor.harron, Final Project
Author: Trevor C. Harron, trevor.harron, 130000218
Creation date:11/25/2018
- End Header --------------------------------------------------------*/
Texture::Texture(const std::string& path) :m_RendererID(0), FilePath(path), m_localBuffer(nullptr), m_width(0), m_height(0), m_bitsperpixel(0)
{
	FileManager::GetAndLoadTexture(&m_localBuffer, FilePath, &m_width, &m_height, &m_bitsperpixel, &m_RendererID);
}


Texture::~Texture()
{
	GLCall(glDeleteTextures(1, &m_RendererID));
}

void Texture::Bind(unsigned int slot) const
{
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void Texture::Unbind() const
{
}
