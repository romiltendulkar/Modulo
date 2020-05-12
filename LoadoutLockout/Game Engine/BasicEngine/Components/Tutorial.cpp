#include "stdafx.h"
#include "Tutorial.h"
#include "..\Events\ChangeSpriteEvent.h"
#include "..\State\GameState.h"
#include "..\Managers\EventManager.h"
#include "..\Managers\InputFacade.h"

extern Audio_Manager* pAM;
extern InputFacade* pIF;
extern GameState* current_state;
Tutorial::Tutorial() :Component(_TUTORIAL)
{
	key_image = "";
	controller_image = "";
	prev_controller = false;
}


Tutorial::~Tutorial()
{
}

void Tutorial::Update()
{
	if (paused) return;
	bool controller = pIF->IsUsingController();
		ChangeSpriteEvent* pCE = new ChangeSpriteEvent();
		if (controller) {
			pCE->new_name = controller_image;
		} else {
			pCE->new_name = key_image;
		}
		current_state->pEM->SendEvent(pCE, mpOwner);
}

void Tutorial::Serialize(std::string obj)
{
	json j_tut = json::parse(obj);
	controller_image = SafeGet<std::string>(j_tut, "controller", controller_image);
	key_image = SafeGet<std::string>(j_tut, "keyboard", key_image);
}

void Tutorial::HandleEvent(Event * pevent)
{
	switch (pevent->mType) {
	case PAUSE:
		paused = (mpOwner->unpausePriority > 0);
		break;
	}
}
