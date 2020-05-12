#include "stdafx.h"
#include "Start.h"


Start::Start() :Component(STARTPOINT)
{
}


Start::~Start()
{
}

void Start::Update()
{
}

void Start::Serialize(std::string obj)
{
}

void Start::HandleEvent(Event * pevent)
{
	switch (pevent->mType) {
	case PAUSE:
		paused = !paused;
		break;
	case COLLIDE:
		break;
	case RESET:
		break;
	}
}

void Start::Init()
{
	//have player set its reset coordinates to this object
}
