#ifndef STATE_AGDEV_OPTIONS_H
#define STATE_AGDEV_OPTIONS_H

#include "State.h"
#include "MeshBuilder.h"
#include <string>
#include "GuiButton.h"
#include "SoundManager.h"

#define TEXT_FONT 0

enum BUTTON
{
	SOUND_BUTTON,
	DIFFICULTY_BUTTON,
	DEFAULT_BUTTON,
};

class StateAGDevOptions : public State
{
public:
	StateAGDevOptions(std::string name, View * theView) : State(name, theView)
	{
		this->theView = theView;
	}
	~StateAGDevOptions();

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
	void toggleDifficulty();

	// Rendering Functions
	void RenderBackground();
	void RenderButtons();

	static int difficulty;
private:
	View * theView;
	std::vector<Mesh*> m_meshList;
	std::vector<Gui*> m_guiList;
	BUTTON m_eCurrentSelection;

	// Fade Effect
	double m_dFadeDelay;
	bool m_bStartFadeIn;
	bool m_bStartFadeOut;
};

#endif