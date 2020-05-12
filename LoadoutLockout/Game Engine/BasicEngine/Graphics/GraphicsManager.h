#ifndef __GRAPHICS_MANAGER__
#define __GRAPHICS_MANAGER__
#include "stdafx.h"
#include"VertexBufferLayout.h"
#include "TextRendering.h"

/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: GraphicsManager.h
Purpose: The definition of the Graphics Manager class. This utilizes SDL
and GLAD and OPENGL.
Platform: Windows 10 x86
Project: trevor.harron, Final Project
Author: Trevor C. Harron, trevor.harron, 130000218
Creation date:11/25/2018
- End Header --------------------------------------------------------*/

class GameObject_Manager;
class GameObject;
class Sprite;
class Camera;
class GraphicsManager
{
public:
	GraphicsManager(SDL_Window* pWindow, SDL_GLContext opengl_context, bool follow = false);
	~GraphicsManager();
	void Update(SDL_Window* pWindow, GameObject_Manager* pGOM,SDL_GLContext opengl_context, 
		bool showText=false, glm::vec4 color=glm::vec4(1.f));
	void DisplayState(SDL_Window* pWindow, SDL_GLContext opengl_context, GameObject* player, 
		bool showText = false, glm::vec4 color = glm::vec4(1.f));
	void HandleResize(SDL_Window* pWindow);
	void ToggleDebug();
	void ToggleGUI();
	bool GetDebug() { return UseDebugMode; };
	bool show_demo_window = false;
	bool show_another_window = true;
	//ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	void ToggleTextures();
	int GetWindowHeight();
	int GetWindowWidth();
	int FollowPlayer(float x, float y);
	int ResetCamera(float x, float y);
	int ResetCamera();
	void SetFollow(bool _follow);
	void SetView(float width, float height);
	void SetDrawSpace(float w, float h, bool use_window=true);
	Camera* GetCamera() { return camera; }
	void DrawBackground(std::string name, glm::vec4 color= glm::vec4(1.f));
	int windowWidth, windowHeight;
	bool useText;
	float textx;
	float texty;
	std::string renderString;
private:
	void Animate(Sprite* s);
	float drawWidth, drawHeight;

	Camera* camera;

	GLint gProgramId;
	VertexArray va;
	
	IndexBuffer *pib;
	glm::mat4 vp;

	bool UseTextures, UseDebugMode, UseGUIMode, IsFollowCharacter;
	unsigned int VBO, VAO, EBO, texture;
	glm::mat4 projection;
	glm::mat4 view;

	TextRendering text;

	Shader* shader;
	Shader* debugShader;
	Shader* TextShader;
	Shader* ParticleShader;
};

#endif // !__GRAPHICS_MANAGER__

