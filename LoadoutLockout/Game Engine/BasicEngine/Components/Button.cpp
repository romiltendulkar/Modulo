#include "stdafx.h"
#include "Button.h"
#include "Cursor.h"
#include "..\Events\ChangeSpriteEvent.h"
#include "..\Events\CollisionEvent.h"
#include "..\Events\ClickEvent.h"
#include "..\State\GameState.h"
#include "..\Managers\EventManager.h"
#include "..\Managers\CollisionManager.h"
#include "..\Audio\Audio_Manager.h"
extern Audio_Manager* pAM;
extern GameState* current_state;

Button::Button() : Component(_BUTTON)
{
	is_selected = prev_selected = false;
	level = n_selected = selected = "";
	
	cursor = nullptr;
}


Button::~Button()
{
}

void Button::Update()
{
	if (paused) return;
	if (nullptr == cursor) {
		return;
	}
  	Body* b1 = static_cast<Body*>(mpOwner->GetComponent(BODY));
	Body* b2 = static_cast<Body*>(cursor->GetComponent(BODY));
	
	is_selected = b1->shape->TestPoint(b2->pos.x,b2->pos.y);
	ChangeSpriteEvent* cSE;

	if (!is_selected) {
		cursor = nullptr;
		if (n_selected.empty() || selected.empty()) return;
		cSE = new ChangeSpriteEvent();
		cSE->new_name = n_selected;
		current_state->pEM->SendEvent(cSE, mpOwner);
	}
	else if (is_selected != prev_selected) {
		if (n_selected.empty() || selected.empty()) return;
		cSE = new ChangeSpriteEvent();
		cSE->new_name = selected;
		current_state->pEM->SendEvent(cSE, mpOwner);
		pAM->load_audio_no_loop("door_sound.wav");
	}
}

void Button::Serialize(std::string obj)
{
	json j_button = json::parse(obj);
	level = SafeGet<std::string>(j_button, "level",level);
	selected = SafeGet<std::string>(j_button, "selected", selected);
	n_selected = SafeGet<std::string>(j_button, "n_selected", n_selected);
}

void Button::HandleEvent(Event * pevent)
{
	CollisionEvent* cE;
	Cursor* cur;
	GameObject* other_object;
	switch (pevent->mType) {
	case COLLIDE:
		cE = static_cast<CollisionEvent*>(pevent);
		other_object = cE->b1->mpOwner;
		if (other_object == this->mpOwner) {
			other_object = cE->b2->mpOwner;
		}
		cur = static_cast<Cursor*>(other_object->GetComponent(_CURSOR));
		prev_selected = is_selected;
		is_selected = true;
		if(cur != nullptr)
			cursor = other_object;
		else break;
		if (paused) break;
		if (cur->IsClick) 
			current_state->pEM->SendEvent(new ClickEvent(), mpOwner);
		break;
	case CLICK:
		current_state->ButtonCallback(id, level);
		pAM->load_audio_no_loop("hit.wav");
		break;
	case PAUSE:
		paused = (mpOwner->unpausePriority > 0);
		break;
	}
}
