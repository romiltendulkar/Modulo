#pragma once
#include "GameState.h"
using namespace nlohmann;
class UtilityState :
	public GameState
{
public:
	UtilityState();
	~UtilityState();
	virtual void Init(GameEngine* _owner, std::string level = "");
	virtual void CleanUp();

	virtual void Pause();
	virtual void Resume();
	virtual void ButtonCallback(unsigned int id, std::string str);

	virtual void HandleInput();
	virtual void Update();
	virtual void Draw() {
		if (nullptr != pGfx && nullptr != pGOM) {
			pGfx->DrawBackground(background);
			pGfx->Update(mpOwner->pWindow, pGOM, mpOwner->opengl_context);
		}
		pFRC->EndFrame();
	};
	virtual void Reset(std::string _level = "pause.csv");
	static UtilityState* GetInstance() {
		return &mUtilityState;
	}
private:
	static UtilityState mUtilityState;
	bool controller_background_needed;
	json config;
};

