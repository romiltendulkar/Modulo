#include "GraphicsManager.h"

#include "stdafx.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include "..\Components\Body.h"
#include "..\Managers\CollisionManager.h"
#include "..\BasicEngine\Managers\GameObject_Manager.h"
#include "..\Managers\Resource_Manager.h"
#include "..\State\GameState.h"
#include "../Managers/FrameRate_Controller.h"
#include "Camera.h"
#include "..\Components\Player.h"
#include "..\Managers\ParticleManager.h"
#include "Particle.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_opengl3.h"
#include "ImGui/imgui_impl_sdl.h"


#define RESOURCE_TEX_PATH ".\\Resources\\Textures\\"
#define WIN_WIDTH  800;
#define WIN_HEIGHT  600;
#define DEBUG 0


extern FrameRate_Controller* pFRC;
extern GameState* current_state;


GraphicsManager::GraphicsManager(SDL_Window* pWindow, SDL_GLContext opengl_context, bool follow)
{
	shader = new Shader("./Shaders/Basic.shader");
	debugShader = new Shader("./Shaders/Debug.shader");
	TextShader = new Shader("./Shaders/Text.shader");
	UseDebugMode = false;
	UseTextures = true;
	UseGUIMode = false;
	useText = false;
	renderString = { 0 };
	
	
	const char* glsl_version = "#version 130";

	{
		float vertices[] = {
			 0.5f,  0.5f,  1.0f, 1.0f,
			 0.5f, -0.5f,  1.0f, 0.0f,
			-0.5f, -0.5f,  0.0f, 0.0f,
			-0.5f,  0.5f,  0.0f, 1.0f
		};



		unsigned int indices[] = {// this makes a rectangle
			3, 0, 1, // first triangle
			1, 2, 3  // second triangle
		};
			
			//Generate an OpenGL buffer to store data;
			VertexBuffer vb(vertices, 4 * 4 * sizeof(float));

		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);
		va.AddBuffer(vb, layout);

		pib = new IndexBuffer(indices, 6);
		
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		SDL_GetWindowSize(pWindow, &windowWidth, &windowHeight);
		drawHeight = (float)windowHeight;
		drawWidth = (float)windowWidth;

		//additional variables for calcualtion
		camera = new Camera(drawHeight*.5f ,drawWidth*.5f );

		view = camera->GetViewMatrix();
		projection = glm::ortho(-600.f, 600.f , -400.f , 400.f , 0.f, 100.0f );
		IsFollowCharacter = follow;
		vp = projection * view;
		
		shader->Bind();
		debugShader->Bind();
		TextShader->Bind();
		text.init();

		va.Unbind();
		shader->Unbind();
		vb.UnBind();
		pib->UnBind();

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();
		ImGui_ImplSDL2_InitForOpenGL(pWindow, opengl_context);
		ImGui_ImplOpenGL3_Init(glsl_version);

		
	}
	
	//*/
}

GraphicsManager::~GraphicsManager()
{
	delete camera;
	delete shader;
	delete debugShader;
	delete TextShader;
}

void GraphicsManager::Update(SDL_Window* pWindow, GameObject_Manager* pGOM, 
	SDL_GLContext opengl_context, bool showText, glm::vec4 color)
{
	/* draw a background
	*/
	DrawBackground(current_state->background);
	mat4 model;
	mat4 mvp;
	renderer renderer;
	camera->Update(pFRC->GetDT());
	if (nullptr != pGOM) {
		for (auto go : pGOM->GetObjects()) {
			//go->visable = camera->IsVisable(go);
			if (!go->visable) continue;
			Sprite* s = static_cast<Sprite*>(go->GetComponent(SPRITE));
			Transform* t = static_cast<Transform*>(go->GetComponent(TRANSFORM));
			if (nullptr == t || nullptr == s) continue;
			Texture *tex = current_state->pRM->LoadTexture(s->file);
			tex->Bind();

			Body* b = static_cast<Body*>(go->GetComponent(BODY));
			model = t->MakeTransformMatrix();

			if (UseDebugMode && nullptr != b) {

				Circle* shape = static_cast<Circle*>(b->shape);
				debugShader->Bind();
				if (Shape::CIRCLE == shape->type)
				{
					tex = current_state->pRM->LoadTexture("circle.png");
					tex->Bind();
					mvp = vp * Transform::MakeTransformFromBody(b);
					debugShader->Bind();
					debugShader->setUniform1i("u_Texture", 0);
					debugShader->setUniformmat4f("u_mvp", mvp);
					debugShader->setBool("is_circle", true);
					renderer.Draw(va, pib, debugShader);

					tex = current_state->pRM->LoadTexture(s->file);
					tex->Bind();
					mvp = vp * t->MakeTransformMatrix();
					shader->Bind();
					Animate(s);
					shader->setUniform1i("u_Texture", 0);
					shader->setUniformmat4f("u_mvp", mvp);

				}
				else
				{
					mvp = vp * model;

					shader->Bind();
					shader->setUniform1i("u_Texture", 0);
					shader->setUniformmat4f("u_mvp", mvp);
					Animate(s);
					renderer.Draw(va, pib, shader);

					mvp = vp * Transform::MakeTransformFromBody(b);
					debugShader->Bind();
					debugShader->setUniform1i("u_Texture", 0);
					debugShader->setUniformmat4f("u_mvp", mvp);
					debugShader->setBool("is_circle", false);
					renderer.DebugDraw(va, pib, debugShader);
				}



			}
			else {

				mvp = vp * model;
				shader->Bind();
				Animate(s);
				shader->setUniform1i("u_Texture", 0);
				shader->setUniformmat4f("u_mvp", mvp);
				shader->setUniform4f("u_Color", color.r, color.g, color.b, color.a);
				renderer.Draw(va, pib, shader);

			}


		}
		if (UseGUIMode)
		{
			ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplSDL2_NewFrame(pWindow);
			ImGui::NewFrame();

			static float f = 0.0f;
			static int counter = 0;

			ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

			ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
			ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
			ImGui::Checkbox("Another Window", &show_another_window);

			ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f    
			ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

			if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
				counter++;
			ImGui::SameLine();
			ImGui::Text("counter = %d", counter);

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
			ImGui::Render();

			SDL_GL_MakeCurrent(pWindow, opengl_context);
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		}
		//Render Text
		if (useText) {
			//render text

			vec4 Color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
			TextShader->Bind();
			TextShader->setUniformmat4f("u_mvp", projection * view
				* glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0.f))
				* glm::scale(glm::mat4(1.0f), glm::vec3(6.5, 6.5, 1.0f)));
			TextShader->setUniform4f("u_Color", Color.r, Color.g, Color.b, Color.a);
			text.RenderText(renderString, textx, texty, 0.2f, mvp);

		}

		GameObject* player = pGOM->GetObjectByType(TYPE_PLAYER);
		if (nullptr != player)
			DisplayState(pWindow, opengl_context, player);

		if (nullptr != current_state->pPM) {

			for (auto p : current_state->pPM->GetActiveParticles()) {

				Texture *t = current_state->pRM->LoadTexture(p->GetName());
				t->Bind();
				mvp = projection * view * p->MakeTransform();
				shader->Bind();
				unsigned int utextureOffSet = glGetUniformLocation(shader->GetID(), "tex_off");
				unsigned int umulTexture = glGetUniformLocation(shader->GetID(), "tex_mul");
				glUniform2f(umulTexture, 1, 1);
				glUniform2f(utextureOffSet, 0, 0);
				shader->setUniform1i("u_Texture", 0);
				shader->setUniformmat4f("u_mvp", mvp);
				shader->setUniform4f("u_Color", p->GetColor().r, p->GetColor().g, p->GetColor().b, p->GetColor().a);
				renderer.Draw(va, pib, shader);
			}
		}
	}

	SDL_GL_SwapWindow(pWindow);
	
}

void GraphicsManager::DisplayState(SDL_Window * pWindow, SDL_GLContext opengl_context, 
	GameObject * player, bool showText,  glm::vec4 color)
{
	Rect* cam_shape = static_cast<Rect*>(camera->camera_space->shape);
	float win_h = windowHeight / 2.f;
	float screen_h_buff = win_h / 4.f;
	float win_x = windowWidth / 2.f;
	float screen_w_buff = win_x / 2.f;
	float status_w = screen_w_buff/4.f;
	float status_h = screen_h_buff;
	float status_x = camera->camera_space->pos.x - win_x +screen_w_buff/3.f ;
	float status_y = camera->camera_space->pos.y + win_h - screen_h_buff/2.f;
	
	mat4 model;
	mat4 trans = glm::translate(mat4(1.0f), vec3(status_x, status_y, 0.0f));
	model = trans * glm::scale(mat4(1.0f), vec3(status_w, status_h, 1.0f));
	
	float increment_x = status_w / 4.f;
	float increment_y = status_h / 4.f;
	Player* pP = static_cast<Player*>(player->GetComponent(PLAYER));
	//lives
	mat4 mvp;
	{
		if (nullptr == pP) return;
		Texture *tex;
		tex = current_state->pRM->LoadTexture("Face.png");
		tex->Bind();
		for (int i = 1; i <= pP->lives; ++i) {
			float x = status_x  + (i*increment_x) + increment_x / 2.5+ 50.f*i;
			float y = status_y;
			mat4 trans = glm::translate(mat4(1.0f), vec3(x, y, 0.0f));
			model = trans * glm::scale(mat4(1.0f), vec3(increment_x*2.5f, increment_y*2.5f, 1.0f));
			mvp = ortho(-windowWidth/2.f, windowWidth /2.f,-windowHeight/2.f,windowHeight/2.f,0.f,100.f) * view    * model;
			//border first
			shader->Bind();
			shader->setUniform1i("u_Texture", 0);
			shader->setUniformmat4f("u_mvp", mvp);
			
			unsigned int utextureOffSet = glGetUniformLocation(shader->GetID(), "tex_off");
			unsigned int umulTexture = glGetUniformLocation(shader->GetID(), "tex_mul");
			glUniform2f(umulTexture, 1, 1);
			glUniform2f(utextureOffSet, 0, 0);
			renderer renderer;
			renderer.Draw(va, pib, shader);
		}
	}
	//abilities
	status_x = camera->camera_space->pos.x - win_x + screen_w_buff;
	status_y = camera->camera_space->pos.y -win_h + screen_h_buff;
	 status_w = screen_w_buff;
	 status_h = screen_h_buff;
	{
		float ability_x = status_x;
		int index = pP->Loadout.size() - 1;
		for (; index >= 0; --index) {
			float x = ability_x  +((index+1)* increment_x*6.75f);
			float y = status_y  - increment_y;
			mat4 trans = glm::translate(mat4(1.0f), vec3(x, y, 0.0f));
			model = trans * glm::scale(mat4(1.0f), vec3(increment_x * 5.f , increment_x*5.f, 1.0f));
			mvp = ortho(-windowWidth / 2.f, windowWidth / 2.f, -windowHeight / 2.f, windowHeight / 2.f, 0.f, 100.f) * view  * model;
			//border first
			shader->Bind();
			shader->setUniform1i("u_Texture", 0);
			shader->setUniformmat4f("u_mvp", mvp);
			Texture *tex;
			tex = current_state->pRM->LoadTexture(pP->Loadout[index]->sprite_file);
			tex->Bind();
			unsigned int utextureOffSet = glGetUniformLocation(shader->GetID(), "tex_off");
			unsigned int umulTexture = glGetUniformLocation(shader->GetID(), "tex_mul");
			glUniform2f(umulTexture, 1, 1);
			glUniform2f(utextureOffSet, 0, 0);
			renderer renderer;
			renderer.Draw(va, pib, shader);
		}
	}
	glClear(GL_DEPTH_BUFFER_BIT);
	
}


void GraphicsManager::HandleResize(SDL_Window* pWindow)
{
	SDL_GetWindowSize(pWindow, &windowWidth, &windowHeight);
	float w = windowWidth; float h = windowHeight;
	
	glViewport(0, 0, windowWidth, windowHeight);
	shader->Bind();
	shader->setUniformmat4f("u_mvp", vp);
	shader->Unbind();
	debugShader->Bind();
	debugShader->setUniformmat4f("u_mvp", vp);
	debugShader->Unbind();
}

void GraphicsManager::ToggleDebug()
{
	UseDebugMode = !UseDebugMode;
}

void GraphicsManager::ToggleGUI()
{
	UseGUIMode = !UseGUIMode;
}

void GraphicsManager::ToggleTextures()
{
	UseTextures = !UseTextures;
}

int GraphicsManager::GetWindowHeight()
{
	return windowHeight;
}

int GraphicsManager::GetWindowWidth()
{
	return windowWidth;
}

void GraphicsManager::SetFollow(bool _follow) {
	if (_follow) {
		projection = glm::ortho(-350.f, 350.f, -200.f, 200.f, 0.f, 100.0f);
	}
	else {
		projection = glm::ortho(-600.f, 600.f, -400.f, 400.f, 0.f, 100.0f);
	}
	if (camera != nullptr) {
		vp = projection * camera->GetViewMatrix();
	}
	IsFollowCharacter = _follow;
}

int GraphicsManager::FollowPlayer(float x, float y)
{
	if (IsFollowCharacter) {
		
		camera->SetCameraSize(windowWidth / 2.f, windowHeight / 2.f);
		camera->MoveCamera(x, y);
		vp = projection * camera->GetViewMatrix();
	} else {
		camera->MoveCamera(0, 0);
		camera->SetCameraSize(windowWidth/2.f, windowHeight/2.f);
		vp = projection * camera->GetViewMatrix();
	}
	
	return 0;
}

int GraphicsManager::ResetCamera(float x, float y)
{
	camera->ResetCameraToPoint(vec4(x, y, 0, 0));
	view = camera->GetViewMatrix();
	vp = projection * view;
	return 0;
}

int GraphicsManager::ResetCamera()
{
		camera->ResetCameraToOrigin();
		camera->SetCameraSize(windowWidth / 2.f, windowHeight / 2.f);
		SetFollow(false);
		view = camera->GetViewMatrix();
		vp =  projection * view;
	    return 0;
}

void GraphicsManager::SetView(float width, float height)
{
	camera->SetCameraSize(width, height);
}

void GraphicsManager::SetDrawSpace(float w, float h, bool use_window)
{
	if (use_window) {
		drawHeight = (float)windowHeight;
		drawWidth = (float)windowWidth;
		
	}
	else {
		drawWidth = w;
		drawHeight = h;
	}
	
}

void GraphicsManager::DrawBackground(std::string name, glm::vec4 color)
{
	glClear(GL_DEPTH_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.f, 0.f, 0.f, 1.0f);
	mat4 model;
	view = camera->GetViewMatrix();
	renderer renderer;
	// Setup Platform/Renderer bindings
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	mat4 mvp;
	if (!name.empty()) {

		mat4 scale = glm::scale(mat4(1.0f), vec3(drawWidth, drawHeight, 1.0f));
		
		if (current_state == nullptr) return;
		shader->Bind();
		Texture *tex = current_state->pRM->LoadTexture(name);
		tex->Bind();
		model = scale;
		mvp = glm::ortho(-drawWidth/2.f, drawWidth/2.f, -drawHeight/2.f, drawHeight/2.f, 0.0f, 100.f)   * model;
		
		shader->setUniform1i("u_Texture", 0);
		shader->setUniformmat4f("u_mvp", mvp);
		shader->setUniform4f("u_Color", color.r, color.g, color.b, color.a);
		float w = ((camera->camera_space->pos.x + drawWidth / 2) / drawWidth )+.5;

		unsigned int utextureOffSet = glGetUniformLocation(shader->GetID(), "tex_off");
		unsigned int umulTexture = glGetUniformLocation(shader->GetID(), "tex_mul");
		glUniform2f(utextureOffSet, w,1);
		glUniform2f(umulTexture, 1, 1);
		renderer.Draw(va, pib, shader);

		glClear(GL_DEPTH_BUFFER_BIT);
	}
}

void GraphicsManager::Animate(Sprite* s) {
	unsigned int utextureOffSet = glGetUniformLocation(shader->GetID(), "tex_off");
	unsigned int umulTexture = glGetUniformLocation(shader->GetID(), "tex_mul");
	glUniform2f(umulTexture, s->x_mul, s->y_mul);
	glUniform2f(utextureOffSet, s->x_off, s->y_off);	
}


