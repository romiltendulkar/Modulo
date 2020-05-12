#include "stdafx.h"
#include "Input_Manager.h"
#include "..\Graphics\GraphicsManager.h"
#define STATE_LENGTH 512
#define STATE_SIZE sizeof(Uint8) * STATE_LENGTH

/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Input_Manager.cpp
Purpose: This file has the implementation for the Input manager.
The Input Manager helps separate the concerns of handling input by
simply storing the current and previous keyboard state and determines
if the key is triggered, pressed, or released.
Language: C, mingw64, 7.1.0
Platform: <Visual Studio 2017, Win32>
Project: trevor.harron, Milestone1
Author: Trevor C. Harron, trevor.harron, 130000218
Creation date:10/18/2018
- End Header --------------------------------------------------------*/
#include <iostream>

extern GraphicsManager* pGfx;

Input_Manager::Input_Manager()
{
	currentStateKeys =(Uint8*) malloc(STATE_SIZE);
	previousStateKeys = (Uint8*) malloc(STATE_SIZE);
	currentStatePad = (Uint8*)malloc(STATE_SIZE);
	previousStatePad = (Uint8*)malloc(STATE_SIZE);
	memset(previousStatePad, 0, STATE_SIZE);
	memset(currentStatePad, 0, STATE_SIZE);
	memset(previousStateKeys, 0, STATE_SIZE);
	memset(currentStateKeys, 0, STATE_SIZE);
	prev_x = prev_y = current_x = current_y = 0;
	pLX = cLX = pLY = cLY = pRX = cRX = pRY = cRY = 0;
	prevMouseButtonPressed = currentMouseButtonPressed = 0;
}


Input_Manager::~Input_Manager()
{
	free(currentStateKeys);
	free(previousStateKeys);
	free(currentStatePad);
	free(previousStatePad);
	if (controller != nullptr) {
		SDL_GameControllerClose(controller);
		controller = nullptr;
	}
	
}

void Input_Manager::UpdateInputManager()
{
	const Uint8 * state;
	if (SDL_NumJoysticks() > 0) {
		use_controller = true;
		if (controller == nullptr) {
			controller = SDL_GameControllerOpen(0);//only one controller
			
		} else	if (SDL_GameControllerGetAttached(controller)) {
			memcpy(previousStatePad, currentStatePad, STATE_SIZE);
			memset(currentStatePad, 0, STATE_SIZE);
			pLX = cLX; pLY = cLY; pRX = cRX; pRY = cRY;
			
			cLX = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTX);
			 if (cLX < SHRT_MIN / 2)  cLX = SHRT_MIN;
			else if (cLX > SHRT_MAX / 2) cLX = SHRT_MAX;
			else cLX = 0;
			
			cLY = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY);
			if (cLY < SHRT_MIN / 2)  cLY= SHRT_MIN;
			else if (cLY > SHRT_MAX / 2) cLY = SHRT_MAX;
			else cLY = 0;
			
			cRX = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTX);
			if (cRX < SHRT_MIN / 2)  cRX = SHRT_MIN;
			else if( cRX > SHRT_MAX / 2) cRX = SHRT_MAX;
			else cRX = 0;
			
			cRY = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTY);
			if (cRY < SHRT_MIN / 2)  cRY = SHRT_MIN;
			else if (cRY > SHRT_MAX / 2) cRY = SHRT_MAX;
			else cRY = 0;
		} else {
			std::cerr << "found controller " << SDL_GameControllerGetAttached(controller) << " found controller" << SDL_GameControllerName(controller) << std::endl;
			std::cerr << "failed to create the controller " << SDL_GetError() << std::endl;
		}
	}
	else {
		//close previous controller if opened
		use_controller = false;
		if (controller != nullptr){
			SDL_GameControllerClose(controller);
			controller = nullptr;
			memset(previousStatePad, 0, STATE_SIZE);
		}
		else {
			memcpy(previousStatePad, currentStatePad, STATE_SIZE);
		}
		//then update
	}
	memcpy(previousStateKeys,currentStateKeys,STATE_SIZE);
	state = SDL_GetKeyboardState(NULL);
	memcpy(currentStateKeys, state, STATE_SIZE);

	prevMouseButtonPressed = currentMouseButtonPressed;
	prev_x = current_x; prev_y = current_y;
	move_x = prev_x; move_y = prev_y;
	currentMouseButtonPressed = SDL_GetMouseState(&current_x, &current_y);
	SDL_GetRelativeMouseState(&move_x, &move_y);
}

bool Input_Manager::IsKeyTriggerred(SDL_Keycode key)
{
	if (key > STATE_LENGTH) return false;
	return currentStateKeys[key] && !previousStateKeys[key];
}

bool Input_Manager::IsKeyPressed(SDL_Keycode key)
{
	if (key > STATE_LENGTH) return false;
	return previousStateKeys[key] && currentStateKeys[key];
}

bool Input_Manager::IsKeyReleased(SDL_Keycode key)
{
	if (key > STATE_LENGTH) return false;
	return previousStateKeys[key] && !currentStateKeys[key];
}

bool Input_Manager::IsMouseButtonTriggered(Uint32 button)
{
	return button == currentMouseButtonPressed && currentMouseButtonPressed != prevMouseButtonPressed;;
}

bool Input_Manager::IsMouseButtonPressed(Uint32 button)
{
	return button == currentMouseButtonPressed && currentMouseButtonPressed == prevMouseButtonPressed;
}

bool Input_Manager::IsMouseButtonReleased(Uint32 button)
{
	return button == prevMouseButtonPressed && currentMouseButtonPressed != prevMouseButtonPressed;
}

bool Input_Manager::IsMouseMoved()
{
	return prev_x != current_x || prev_y != current_y;
}

std::pair<float, float> Input_Manager::GetMousePos()
{
	return std::pair<float,float>(current_x - pGfx->GetWindowWidth() / 2, pGfx->GetWindowHeight()/2 - current_y);
}

std::pair<float, float> Input_Manager::GetMouseMove()
{
	float x = current_x - prev_x - pGfx->GetWindowWidth() / 2;
	float y = pGfx->GetWindowHeight() / 2 -current_y - prev_y;
	return std::pair<float, float>(x, y);
}


bool Input_Manager::IsControllerButtonTriggerred(SDL_GameControllerButton button)
{

	if (button > STATE_LENGTH || controller == nullptr) return false;
	Uint8 pressed = SDL_GameControllerGetButton(controller, button);
	currentStatePad[button] = pressed;
	return !previousStatePad[button] && currentStatePad[button];
}

bool Input_Manager::IsControllerButtonPressed(SDL_GameControllerButton button)
{
	if (button > STATE_LENGTH || controller == nullptr) return false;
	Uint8 pressed = SDL_GameControllerGetButton(controller, button);
	currentStatePad[button] = pressed;
	return previousStatePad[button] && currentStatePad[button];
}

bool Input_Manager::IsControllerButtonReleased(SDL_GameControllerButton button)
{
	if (button > STATE_LENGTH || controller == nullptr) return false;
	Uint8 pressed = SDL_GameControllerGetButton(controller, button);
	currentStatePad[button] = pressed;
	return previousStatePad[button] && !currentStatePad[button];
}

bool Input_Manager::IsUsingController()
{
	return use_controller;
}

bool Input_Manager::IsStickTriggeredUp(SDL_GameControllerAxis axis)
{
	if (SDL_CONTROLLER_AXIS_LEFTX == axis ||
		SDL_CONTROLLER_AXIS_RIGHTX == axis) return false;

	return (SDL_CONTROLLER_AXIS_LEFTY == axis && cLY < pLY && pLY == 0)
		|| (SDL_CONTROLLER_AXIS_RIGHTY == axis && cRY < pRY && pRY == 0);
}

bool Input_Manager::IsStickTriggeredDown(SDL_GameControllerAxis axis)
{
	if (SDL_CONTROLLER_AXIS_LEFTX == axis ||
		SDL_CONTROLLER_AXIS_RIGHTX == axis) return false;

	return (SDL_CONTROLLER_AXIS_LEFTY == axis && cLY > pLY && pLY == 0)
		|| (SDL_CONTROLLER_AXIS_RIGHTY == axis && cRY > pRY && pRY == 0);
}

bool Input_Manager::IsStickTriggeredLeft(SDL_GameControllerAxis axis)
{
	if (SDL_CONTROLLER_AXIS_LEFTY == axis || 
		SDL_CONTROLLER_AXIS_RIGHTY == axis) return false;

	return (SDL_CONTROLLER_AXIS_LEFTX == axis && cLX < pLX && pLX == 0)
		|| (SDL_CONTROLLER_AXIS_RIGHTX == axis && cRX < pRX && pRX == 0);
}

bool Input_Manager::IsStickTriggeredRight(SDL_GameControllerAxis axis)
{
	if (SDL_CONTROLLER_AXIS_LEFTY == axis || 
		SDL_CONTROLLER_AXIS_RIGHTY == axis) return false;
	
	return (SDL_CONTROLLER_AXIS_LEFTX == axis && cLX > pLX && pLX == 0)
		|| (SDL_CONTROLLER_AXIS_RIGHTX == axis && cRX > pRX && pRX == 0);
}

//

bool Input_Manager::IsStickPressedUp(SDL_GameControllerAxis axis)
{
	if (SDL_CONTROLLER_AXIS_LEFTX == axis ||
		SDL_CONTROLLER_AXIS_RIGHTX == axis) return false;

	return (SDL_CONTROLLER_AXIS_LEFTY == axis && cLY == pLY && cLY < 0)
		|| (SDL_CONTROLLER_AXIS_RIGHTY == axis && cRY == pRY && cRY < 0);
}

bool Input_Manager::IsStickPressedDown(SDL_GameControllerAxis axis)
{
	if (SDL_CONTROLLER_AXIS_LEFTX == axis ||
		SDL_CONTROLLER_AXIS_RIGHTX == axis) return false;

	return (SDL_CONTROLLER_AXIS_LEFTY == axis && cLY == pLY && cLY > 0)
		|| (SDL_CONTROLLER_AXIS_RIGHTY == axis && cRY == pRY && cRY > 0);
}

bool Input_Manager::IsStickPressedLeft(SDL_GameControllerAxis axis)
{
	if (SDL_CONTROLLER_AXIS_LEFTY == axis || 
		SDL_CONTROLLER_AXIS_RIGHTY == axis) return false;
	
	return (SDL_CONTROLLER_AXIS_LEFTX == axis && cLX == pLX && cLX < 0)
		|| (SDL_CONTROLLER_AXIS_RIGHTX == axis && cRX == pRX && cRX < 0);
}

bool Input_Manager::IsStickPressedRight(SDL_GameControllerAxis axis)
{
	if (SDL_CONTROLLER_AXIS_LEFTY == axis || 
		SDL_CONTROLLER_AXIS_RIGHTY == axis) return false;
	
	return (SDL_CONTROLLER_AXIS_LEFTX == axis && cLX == pLX && cLX > 0)
		|| (SDL_CONTROLLER_AXIS_RIGHTX == axis && cRX == pRX && cRX > 0);
}

//

bool Input_Manager::IsStickReleasedUp(SDL_GameControllerAxis axis)
{
	if (SDL_CONTROLLER_AXIS_LEFTX == axis ||
		SDL_CONTROLLER_AXIS_RIGHTX == axis) return false;

	return (SDL_CONTROLLER_AXIS_LEFTY == axis && cLY > pLY && pLY < 0)
		|| (SDL_CONTROLLER_AXIS_RIGHTY == axis && cRY > pRY && pRY < 0);
}

bool Input_Manager::IsStickReleasedDown(SDL_GameControllerAxis axis)
{
	if (SDL_CONTROLLER_AXIS_LEFTX == axis ||
		SDL_CONTROLLER_AXIS_RIGHTX == axis) return false;

	return (SDL_CONTROLLER_AXIS_LEFTY == axis && cLY < pLY && pLY > 0)
		|| (SDL_CONTROLLER_AXIS_RIGHTY == axis && cRY < pRY && pRY > 0);
}

bool Input_Manager::IsStickReleasedLeft(SDL_GameControllerAxis axis)
{
	if (SDL_CONTROLLER_AXIS_LEFTY == axis || 
		SDL_CONTROLLER_AXIS_RIGHTY == axis) return false;
	
	return (SDL_CONTROLLER_AXIS_LEFTX == axis && cLX > pLX && pLX < 0)
		|| (SDL_CONTROLLER_AXIS_RIGHTX == axis && cRX > pRX && pRX < 0);
}

bool Input_Manager::IsStickReleasedRight(SDL_GameControllerAxis axis)
{
	if (SDL_CONTROLLER_AXIS_LEFTY == axis || 
		SDL_CONTROLLER_AXIS_RIGHTY == axis) return false;
	
	return (SDL_CONTROLLER_AXIS_LEFTX == axis && cLX < pLX && pLX > 0)
		|| (SDL_CONTROLLER_AXIS_RIGHTX == axis && cRX < pRX && pRX > 0);
}
