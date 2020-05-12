#include "stdafx.h"
#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

#include"VertexBufferLayout.h"
struct Character {
	GLuint TextureID;   // ID handle of the glyph texture
	glm::ivec2 Size;    // Size of glyph
	glm::ivec2 Bearing;  // Offset from baseline to left/top of glyph
	GLuint Advance;    // Horizontal offset to advance to next glyph
};

class TextRendering
{
public:
	TextRendering();
	~TextRendering();
	void init();
	void RenderText( std::string text, GLfloat x, GLfloat y, GLfloat scale,  glm::mat4 mvp);
	VertexArray va;
	std::map<GLchar, Character> Characters;
};

