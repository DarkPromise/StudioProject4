#ifndef CONTROLLER_H
#define CONTROLLER_H

// To Check for memory leaks
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "Model.h"
#include "View.h"
#include "timer.h"

#include "InputHandler.h"
#include "SoundManager.h"

#include "StateHandler.h"

#include "LuaReader.h"

class Controller
{
public:
	Controller(Model * theModel, View * theView);
	virtual ~Controller(void);

	void RunGameLoop();
	bool ProcessInputs();
	void MouseUpdate();
	void KeyboardUpdate();

	Model * theModel;
	View * theView;
private:
	StopWatch m_timer;
	double m_dElapsedTime;
	double m_dAccumulatedTime_ThreadOne;
	double m_dAccumulatedTime_ThreadTwo;
	InputHandler * m_cInputHandler;
	StateHandler * m_cStateHandler;
};

#endif
