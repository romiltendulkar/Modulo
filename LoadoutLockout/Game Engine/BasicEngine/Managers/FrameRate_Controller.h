#ifndef FRAMERATE_CONTROLLER
#define FRAMERATE_CONTROLLER
#include "stdafx.h"
#include <SDL2/SDL.h>
/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: FrameRate_Controller.h
Purpose: This file has the defines the variables and methods for the Frame Rate Controller.
Language: C, mingw64, 7.1.0
Platform: Windows 10 x86
Project: trevor.harron, Final Project
Author: Trevor C. Harron, trevor.harron, 130000218
Creation date:10/18/2018
- End Header --------------------------------------------------------*/

class FrameRate_Controller
{
public:
	FrameRate_Controller(Uint32 deisredFPS);
	~FrameRate_Controller();
	void StartFrame();
	void EndFrame();
	Uint32 GetFrameTime();
	float GetDT();
private:
	Uint32 desiredFPSTime;
	Uint32 startTicks;
	Uint32 endTicks;
	Uint32 frameTicks;
};
#endif
