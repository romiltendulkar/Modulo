#include "stdafx.h"
#include "FrameRate_Controller.h"
/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: FrameRate_Controller.cpp
Purpose: This file has the implementation for the Frame Rate
Controller. THe Frame rate controller should be called at the start
and end of the game loop. At the beginning the time to be calculated 
is is taken and then is capped at the end of the loop with the FrameEnd()
call. The fps is initially set and used henceforth.
Platform: Windows 10 x86
Project: trevor.harron, Final Project
Project: trevor.harron, Milestone1
Author: Trevor C. Harron, trevor.harron, 130000218
Creation date:10/18/2018
- End Header --------------------------------------------------------*/

FrameRate_Controller::FrameRate_Controller(Uint32 deisredFPS)
{
	startTicks = endTicks = frameTicks = 0;
	if (deisredFPS > 0) {
		desiredFPSTime = 1000 / deisredFPS;
	}
	else {
		desiredFPSTime = 0;
	}
	frameTicks = 0;
}

FrameRate_Controller::~FrameRate_Controller()
{
}


void FrameRate_Controller::StartFrame()
{
	startTicks = SDL_GetTicks();
}

void FrameRate_Controller::EndFrame()
{
	endTicks = SDL_GetTicks();
	while (endTicks - startTicks < desiredFPSTime) {
		endTicks = SDL_GetTicks();
	}
	frameTicks = endTicks - startTicks;
}

Uint32 FrameRate_Controller::GetFrameTime()
{
	return frameTicks;
}

float FrameRate_Controller::GetDT()
{
	float dt = ((float)frameTicks) * 0.001f;
	
	return dt;
}

