#pragma once
#include "stdafx.h"
class Input_Manager;
enum BUTTON {
	QUIT,
	UP,
	DOWN,
	LEFT,
	RIGHT,
	START,
	PAUSE_BUTTON,
	DEBUG,
	EDIT,
	GUI,
	BACK,
	FREEZE,
	ABILITY_TOP,
	ABILITY_BOTTOM,
	ABILITY_LEFT,
	ABILITY_RIGHT,
	UNLOCKED,
	CHEAT_LIFE,
	CHEAT_SKIP,
	CHEAT_ABILITES,
	CHEAT_END,
	CHEAT_MAX_FRIENDS,
	CHEAT_NO_FRIENDS,
	RESIZE,
	HIDE_1,
	HIDE_2,
	JUMP_BUTTON,
	_NUM
};

class InputFacade
{
public:
	InputFacade();
	~InputFacade();
	void Update();
	Input_Manager* GetInputManager();
	bool IsButtonPressed(BUTTON button);
	bool IsButtonTriggered(BUTTON button);
	bool IsButtonReleased(BUTTON button);
	bool IsUsingController();
private:
	Input_Manager* mpIM;
	bool is_controller; //either false for keyboard or true for controller
	
};

