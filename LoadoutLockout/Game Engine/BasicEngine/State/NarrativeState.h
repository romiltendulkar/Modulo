#pragma once
#include "GameState.h"
#include "..\GameEngine.h"
#include "../Managers/NarrativeManager.h"

class NarrativeState :
	public GameState
{
public:
	NarrativeState();
	~NarrativeState();
	virtual void Init(GameEngine* _owner, std::string level = "");
	virtual void CleanUp();

	virtual void ButtonCallback(unsigned int id, std::string str);

	virtual void HandleInput();
	virtual void Update();
	virtual void Draw();
	virtual void Resume();
	virtual void Pause();
	virtual void Reset(std::string _level = "act-0.csv");
	static NarrativeState* GetInstance() {
		return &mNarrativeState;
	}

private:
	static NarrativeState mNarrativeState;
	NarrativeManager *pNM;
	int script;
};

