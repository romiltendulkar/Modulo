#ifndef INPUT_MANAGER
#define INPUT_MANAGER
#include "stdafx.h"

/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Input_Manager.h
Purpose: This file has the defines the variables and methods for the Input manager.
Language: C, mingw64, 7.1.0
Platform: <Visual Studio 2017, Win32>
Project: trevor.harron, Milestone1
Author: Trevor C. Harron, trevor.harron, 130000218
Creation date:10/18/2018
- End Header --------------------------------------------------------*/
class Input_Manager
{
public:
	Input_Manager();
	~Input_Manager();
	void UpdateInputManager();
	bool IsKeyTriggerred(SDL_Keycode key);
	bool IsKeyPressed(SDL_Keycode key);
	bool IsKeyReleased(SDL_Keycode key);
	bool IsMouseButtonTriggered(Uint32 button);
	bool IsMouseButtonPressed(Uint32 button);
	bool IsMouseButtonReleased(Uint32 button);
	bool IsMouseMoved();
	std::pair<float, float> GetMousePos();
	std::pair<float, float> GetMouseMove();
	bool IsControllerButtonTriggerred(SDL_GameControllerButton button);
	bool IsControllerButtonPressed(SDL_GameControllerButton button);
	bool IsControllerButtonReleased(SDL_GameControllerButton button);
	bool IsUsingController();

	bool IsStickTriggeredUp(SDL_GameControllerAxis axis);
	bool IsStickTriggeredDown(SDL_GameControllerAxis axis);
	bool IsStickTriggeredLeft(SDL_GameControllerAxis axis);
	bool IsStickTriggeredRight(SDL_GameControllerAxis axis);

	bool IsStickPressedUp(SDL_GameControllerAxis axis);
	bool IsStickPressedDown(SDL_GameControllerAxis axis);
	bool IsStickPressedLeft(SDL_GameControllerAxis axis);
	bool IsStickPressedRight(SDL_GameControllerAxis axis);

	bool IsStickReleasedUp(SDL_GameControllerAxis axis);
	bool IsStickReleasedDown(SDL_GameControllerAxis axis);
	bool IsStickReleasedLeft(SDL_GameControllerAxis axis);
	bool IsStickReleasedRight(SDL_GameControllerAxis axis);



private:
	Uint8 * previousStateKeys;
	Uint8 * currentStateKeys;
	Uint8 * previousStatePad;
	Uint8 * currentStatePad;

	SDL_GameController* controller;

	Uint32 prevMouseButtonPressed;
	Uint32 currentMouseButtonPressed;

	Sint16 pLX, pLY, pRX, pRY;//previous axis variables
	Sint16 cLX, cLY, cRX, cRY;//current axis variables

	int current_x, current_y, prev_x, prev_y;
	int move_x, move_y;
	bool use_controller;
};
#endif

