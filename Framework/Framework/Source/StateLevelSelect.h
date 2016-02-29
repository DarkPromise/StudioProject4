#ifndef STATE_LEVEL_SELECT_H
#define STATE_LEVEL_SELECT_H

#include "State.h"
#include "GuiButton.h"

#define TEXT_FONT 0

class StateLevelSelect : public State
{
public:
	enum MENU_BUTTONS
	{
		LEVEL1_BUTTON,
		LEVEL2_BUTTON,
		LEVEL3_BUTTON,
		LEVEL4_BUTTON,
	};

	StateLevelSelect(std::string name, View * theView)
		: State(name, theView)
		, runOnce(false)
		, levelSelected(1)
	{
		this->theView = theView;
	}
	~StateLevelSelect();

	State* getInstance();
	void Init();
	void Update(StateHandler * stateHandler, double dt);
	void Cleanup();
	void Pause();
	void Resume();
	void HandleEvents(StateHandler * stateHandler);
	void HandleEvents(StateHandler * stateHandler, const int key, const bool status = true);
	void Draw(StateHandler * stateHandler);

	// State Specific Functions
	void UpdateSelection(StateHandler * stateHandler);
	void FadeInEffect(double dt);
	void FadeOutEffect(double dt, StateHandler * stateHandler);

	// Rendering Functions
	void RenderBackground();
	void RenderButtons();

private:
	View * theView;
	std::vector<Mesh*> m_meshList;
	std::vector<Gui*> m_guiList;

	// Fade Effect
	double m_dFadeDelay;
	bool m_bStartFadeIn;
	bool m_bStartFadeOut;
	bool runOnce;
	int levelSelected;
};

#endif