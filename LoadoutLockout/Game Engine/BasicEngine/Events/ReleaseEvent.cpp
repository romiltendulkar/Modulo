#include "stdafx.h"
#include "ReleaseEvent.h"

/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: ReleaseEvent.cpp
Purpose: Release Event implementaion, mainly blank.
Platform: Windows 10 x86
Project: trevor.harron, Final Project
Author: Trevor C. Harron, trevor.harron, 130000218
Creation date:12/06/2018
- End Header --------------------------------------------------------*/


ReleaseEvent::ReleaseEvent(GameObject* pGO) : pPlayerObject(pGO), Event(RELEASE)
{
}


ReleaseEvent::~ReleaseEvent()
{
}
