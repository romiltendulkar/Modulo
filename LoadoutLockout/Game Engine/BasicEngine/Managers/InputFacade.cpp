#include "stdafx.h"
#include "InputFacade.h"
#include "Input_Manager.h"

InputFacade::InputFacade()
{
	mpIM = new Input_Manager();
}


InputFacade::~InputFacade()
{
	delete mpIM;
}

void InputFacade::Update()
{
	mpIM->UpdateInputManager();
	is_controller = mpIM->IsUsingController();
}

Input_Manager * InputFacade::GetInputManager()
{
	return mpIM;
	
}

bool InputFacade::IsButtonPressed(BUTTON button)
{
	switch (button)
	{
	case UNLOCKED:
		return mpIM->IsControllerButtonPressed(SDL_CONTROLLER_BUTTON_RIGHTSHOULDER)
			||  mpIM->IsKeyPressed(SDL_SCANCODE_UP);
	case QUIT:
		return false ||  mpIM->IsKeyPressed(SDL_SCANCODE_ESCAPE);
	case BACK:
		return false || mpIM->IsKeyPressed(SDL_SCANCODE_0);
	case JUMP_BUTTON:
		 return mpIM->IsControllerButtonPressed(SDL_CONTROLLER_BUTTON_A)
			 || (mpIM->IsKeyPressed(SDL_SCANCODE_SPACE) || mpIM->IsKeyPressed(SDL_SCANCODE_W));
	case DOWN:
		return mpIM->IsStickPressedDown(SDL_CONTROLLER_AXIS_LEFTY)
			|| mpIM->IsKeyPressed(SDL_SCANCODE_S);
	case LEFT:
		return mpIM->IsStickPressedLeft(SDL_CONTROLLER_AXIS_LEFTX)
			|| mpIM->IsKeyPressed(SDL_SCANCODE_A);
	case RIGHT:
		return mpIM->IsStickPressedRight(SDL_CONTROLLER_AXIS_LEFTX)
			|| mpIM->IsKeyPressed(SDL_SCANCODE_D);
	case START:
	case PAUSE_BUTTON:
		return mpIM->IsControllerButtonPressed(SDL_CONTROLLER_BUTTON_START)
			|| mpIM->IsKeyPressed(SDL_SCANCODE_ESCAPE);
#ifdef _DEBUG //debug features
	case DEBUG:
		return mpIM->IsControllerButtonPressed(SDL_CONTROLLER_BUTTON_GUIDE)
			|| mpIM->IsKeyPressed(SDL_SCANCODE_O);
	case FREEZE:
	 return mpIM->IsControllerButtonPressed(SDL_CONTROLLER_BUTTON_BACK)
		 || mpIM->IsKeyPressed(SDL_SCANCODE_G);
	case EDIT://only edit with the keyboard
		return false
			|| mpIM->IsKeyPressed(SDL_SCANCODE_M);
	
#endif
	case ABILITY_TOP:
		return mpIM->IsControllerButtonPressed(SDL_CONTROLLER_BUTTON_Y)
			|| mpIM->IsKeyPressed(SDL_SCANCODE_UP);
	case ABILITY_BOTTOM:
		return mpIM->IsControllerButtonPressed(SDL_CONTROLLER_BUTTON_A)
			|| mpIM->IsKeyPressed(SDL_SCANCODE_DOWN);
	case ABILITY_LEFT:
		return mpIM->IsControllerButtonPressed(SDL_CONTROLLER_BUTTON_X)
			|| mpIM->IsKeyPressed(SDL_SCANCODE_LEFT);
	case ABILITY_RIGHT:
		return mpIM->IsControllerButtonPressed(SDL_CONTROLLER_BUTTON_B)
			||  mpIM->IsKeyPressed(SDL_SCANCODE_RIGHT);
	default:
		return false;
	}
}

bool InputFacade::IsButtonTriggered(BUTTON button)
{
	switch(button)
	{
	case UNLOCKED:
		return mpIM->IsControllerButtonTriggerred(SDL_CONTROLLER_BUTTON_RIGHTSHOULDER)
			|| mpIM->IsKeyTriggerred(SDL_SCANCODE_UP);
	case QUIT:
		return false
			|| mpIM->IsKeyTriggerred(SDL_SCANCODE_DELETE);
	case CHEAT_LIFE:
		return mpIM->IsControllerButtonTriggerred(SDL_CONTROLLER_BUTTON_DPAD_UP)
			|| mpIM->IsKeyTriggerred(SDL_SCANCODE_1);
	case CHEAT_SKIP:
		return mpIM->IsControllerButtonTriggerred(SDL_CONTROLLER_BUTTON_DPAD_RIGHT)
			|| mpIM->IsKeyTriggerred(SDL_SCANCODE_2);
	case CHEAT_ABILITES:
		return mpIM->IsControllerButtonTriggerred(SDL_CONTROLLER_BUTTON_DPAD_DOWN)
			|| mpIM->IsKeyTriggerred(SDL_SCANCODE_3);
	case CHEAT_END:
		return mpIM->IsControllerButtonTriggerred(SDL_CONTROLLER_BUTTON_DPAD_LEFT)
			|| mpIM->IsKeyTriggerred(SDL_SCANCODE_4);
	case CHEAT_MAX_FRIENDS:
		return mpIM->IsControllerButtonTriggerred(SDL_CONTROLLER_BUTTON_RIGHTSTICK)
			|| mpIM->IsKeyTriggerred(SDL_SCANCODE_5);
	case CHEAT_NO_FRIENDS:
		return mpIM->IsControllerButtonTriggerred(SDL_CONTROLLER_BUTTON_GUIDE)
			|| mpIM->IsKeyTriggerred(SDL_SCANCODE_6);
	case HIDE_1:
		return false
			|| mpIM->IsKeyTriggerred(SDL_SCANCODE_LALT);
	case HIDE_2:
		return false
			|| mpIM->IsKeyTriggerred(SDL_SCANCODE_TAB);
	case RESIZE:
		return false
			|| mpIM->IsKeyTriggerred(SDL_SCANCODE_F11);
	case BACK:
		return false
			|| mpIM->IsKeyTriggerred(SDL_SCANCODE_BACKSPACE);
	case JUMP_BUTTON:
		return mpIM->IsControllerButtonTriggerred(SDL_CONTROLLER_BUTTON_A)
			|| (mpIM->IsKeyTriggerred(SDL_SCANCODE_SPACE) || mpIM->IsKeyTriggerred(SDL_SCANCODE_W));
	case DOWN:
		 return mpIM->IsStickTriggeredDown(SDL_CONTROLLER_AXIS_LEFTY)
			 || mpIM->IsKeyTriggerred(SDL_SCANCODE_S);
	case LEFT:
		return mpIM->IsStickTriggeredLeft(SDL_CONTROLLER_AXIS_LEFTX)
			|| mpIM->IsKeyTriggerred(SDL_SCANCODE_A);
	case RIGHT:
		return mpIM->IsStickTriggeredRight(SDL_CONTROLLER_AXIS_LEFTX)
			|| mpIM->IsKeyTriggerred(SDL_SCANCODE_D);
	case START:
	case PAUSE_BUTTON:
		return mpIM->IsControllerButtonTriggerred(SDL_CONTROLLER_BUTTON_START)
			|| mpIM->IsKeyTriggerred(SDL_SCANCODE_ESCAPE);
#ifdef _DEBUG //debug features
	case DEBUG:
		return mpIM->IsControllerButtonTriggerred(SDL_CONTROLLER_BUTTON_GUIDE)
			|| mpIM->IsKeyTriggerred(SDL_SCANCODE_O);
	case FREEZE:
		return mpIM->IsControllerButtonTriggerred(SDL_CONTROLLER_BUTTON_BACK)
			|| mpIM->IsKeyTriggerred(SDL_SCANCODE_G);
	case EDIT://only edit with the keyboard
		return false
			|| mpIM->IsKeyTriggerred(SDL_SCANCODE_M);
	case GUI://gui window
		return mpIM->IsControllerButtonTriggerred(SDL_CONTROLLER_BUTTON_GUIDE)
			|| mpIM->IsKeyTriggerred(SDL_SCANCODE_V);
#endif
	case ABILITY_TOP:
		return mpIM->IsControllerButtonTriggerred(SDL_CONTROLLER_BUTTON_Y)
			|| mpIM->IsKeyTriggerred(SDL_SCANCODE_UP);
	case ABILITY_BOTTOM:
		return mpIM->IsControllerButtonTriggerred(SDL_CONTROLLER_BUTTON_A)
			|| mpIM->IsKeyTriggerred(SDL_SCANCODE_DOWN);
	case ABILITY_LEFT:
		return mpIM->IsControllerButtonTriggerred(SDL_CONTROLLER_BUTTON_X)
			|| mpIM->IsKeyTriggerred(SDL_SCANCODE_LEFT);
	case ABILITY_RIGHT:
		return mpIM->IsControllerButtonTriggerred(SDL_CONTROLLER_BUTTON_B)
			|| mpIM->IsKeyTriggerred(SDL_SCANCODE_RIGHT);
	default:
		return false;
	}
}

bool InputFacade::IsButtonReleased(BUTTON button)
{
	switch (button)
	{
	case UNLOCKED:
		 return mpIM->IsControllerButtonReleased(SDL_CONTROLLER_BUTTON_RIGHTSHOULDER)
			|| mpIM->IsKeyReleased(SDL_SCANCODE_UP);
	case QUIT:
		return false
			|| mpIM->IsKeyReleased(SDL_SCANCODE_ESCAPE);
	case BACK:
		return false
			|| mpIM->IsKeyReleased(SDL_SCANCODE_0);
	case JUMP_BUTTON:
		return mpIM->IsControllerButtonReleased(SDL_CONTROLLER_BUTTON_A)
			|| (mpIM->IsKeyReleased(SDL_SCANCODE_SPACE) || (mpIM->IsKeyReleased(SDL_SCANCODE_W)));
	case DOWN:
		 return mpIM->IsStickReleasedDown(SDL_CONTROLLER_AXIS_LEFTY)
			 || mpIM->IsKeyReleased(SDL_SCANCODE_S);
	case LEFT:
		return mpIM->IsStickReleasedLeft(SDL_CONTROLLER_AXIS_LEFTX)
			|| mpIM->IsKeyReleased(SDL_SCANCODE_A);
	case RIGHT:
		return mpIM->IsStickReleasedRight(SDL_CONTROLLER_AXIS_LEFTX)
			|| mpIM->IsKeyReleased(SDL_SCANCODE_D);
	case START:
	case PAUSE_BUTTON:
		return mpIM->IsControllerButtonReleased(SDL_CONTROLLER_BUTTON_START)
			|| mpIM->IsKeyReleased(SDL_SCANCODE_ESCAPE);
#ifdef _DEBUG //debug features
	case DEBUG:
		return mpIM->IsControllerButtonReleased(SDL_CONTROLLER_BUTTON_GUIDE)
			|| mpIM->IsKeyReleased(SDL_SCANCODE_O);
	case FREEZE:
		return mpIM->IsControllerButtonReleased(SDL_CONTROLLER_BUTTON_BACK)
			|| mpIM->IsKeyReleased(SDL_SCANCODE_G);
	case EDIT://only edit with the keyboard
		return false
			|| mpIM->IsKeyReleased(SDL_SCANCODE_M);
#endif
	case ABILITY_TOP:
		 return mpIM->IsControllerButtonReleased(SDL_CONTROLLER_BUTTON_Y)
			 || mpIM->IsKeyReleased(SDL_SCANCODE_UP);
	case ABILITY_BOTTOM:
		return mpIM->IsControllerButtonReleased(SDL_CONTROLLER_BUTTON_A)
			|| mpIM->IsKeyReleased(SDL_SCANCODE_DOWN);
	case ABILITY_LEFT:
		return mpIM->IsControllerButtonReleased(SDL_CONTROLLER_BUTTON_X)
			|| mpIM->IsKeyReleased(SDL_SCANCODE_LEFT);
	case ABILITY_RIGHT:
		return mpIM->IsControllerButtonReleased(SDL_CONTROLLER_BUTTON_B)
		|| mpIM->IsKeyReleased(SDL_SCANCODE_RIGHT);
	default:
		return false;
	}
}

bool InputFacade::IsUsingController()
{
	return is_controller;
}
