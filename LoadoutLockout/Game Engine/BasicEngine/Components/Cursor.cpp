#include "stdafx.h"
#include "Cursor.h"
#include "Body.h"
#include "Transform.h"
#include "..\Managers\InputFacade.h"
#include "..\Managers\Input_Manager.h"
#include "..\Graphics\Camera.h"
#include "..\BasicEngine\Graphics\GraphicsManager.h"

extern GraphicsManager* pGfx;
extern InputFacade* pIF;
Cursor::Cursor() : Component(_CURSOR)
{
	IsClick = false;
	
}


Cursor::~Cursor()
{
}

void Cursor::Update()
{
	Body* pB = static_cast<Body*>(mpOwner->GetComponent(BODY));
	if (nullptr == pB) return;
	Input_Manager *tmp = pIF->GetInputManager();
	std::pair<float, float> move = tmp->GetMouseMove();
	std::pair<float, float> pos = tmp->GetMousePos();
	Camera* cam = pGfx->GetCamera();
	int w = pGfx->GetWindowWidth() / 2;
	int h = pGfx->GetWindowHeight() / 2;
	if (tmp->IsMouseMoved()) {
	if (pos.first <  w - w / 7 && pos.first > -w + w / 7)
		pB->pos.x = pos.first + cam->camera_space->pos.x;

	if (pos.second < h - h / 8 && pos.second > -h + h / 8)
		pB->pos.y = pos.second + cam->camera_space->pos.y;
	}

	
	if (tmp->IsStickPressedRight(SDL_CONTROLLER_AXIS_LEFTX)) pB->pos.x += 5.5f;
	if (tmp->IsStickPressedLeft(SDL_CONTROLLER_AXIS_LEFTX)) pB->pos.x -= 5.5f;
	if (tmp->IsStickPressedUp(SDL_CONTROLLER_AXIS_LEFTY)) pB->pos.y += 5.5f;
	if (tmp->IsStickPressedDown(SDL_CONTROLLER_AXIS_LEFTY)) pB->pos.y -= 5.5f;

	IsClick = tmp->IsControllerButtonTriggerred(SDL_CONTROLLER_BUTTON_A) || tmp->IsMouseButtonTriggered(SDL_BUTTON_LEFT);
	
	Transform* pT = static_cast<Transform*>(mpOwner->GetComponent(TRANSFORM));
	if (nullptr == pT) return;
	
	pT->SetX(pB->pos.x);
	pT->SetY(pB->pos.y);
	
}

void Cursor::Serialize(std::string obj)
{
}

void Cursor::HandleEvent(Event * pevent)
{
	switch (pevent->mType) {
	case PAUSE:
		IsClick = false;
		break;
	default:
		break;
	}
}

void Cursor::Init()
{
	Camera* cam = pGfx->GetCamera();
	Body* pB = static_cast<Body*>(mpOwner->GetComponent(BODY));
	pB->pos = cam->camera_space->pos;

}
