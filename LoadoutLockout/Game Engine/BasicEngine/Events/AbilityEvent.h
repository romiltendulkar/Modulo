#pragma once
#include "stdafx.h"
#include "Event.h"
#include "..\Components\definitions.h"
class AbilityEvent :
	public Event
{
public:
	AbilityEvent();
	~AbilityEvent();
	A_TYPE a_type;
	bool make_solid;
};

