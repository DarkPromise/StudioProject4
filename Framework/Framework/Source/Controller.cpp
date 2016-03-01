#include "Controller.h"

#include "StateTest.h"
#include "StateSplash.h"

Controller::Controller(Model * theModel, View * theView)
: theModel(theModel)
, theView(theView)
{
	std::cout << "Controller Created" << std::endl;
	m_dElapsedTime = 0.0;
	m_dAccumulatedTime_ThreadOne = 0.0;
	m_dAccumulatedTime_ThreadTwo = 0.0;
}

Controller::~Controller()
{
	this->theModel = NULL;
	this->theView = NULL;

	if (m_cStateHandler != NULL)
	{
		delete m_cStateHandler;
		m_cStateHandler = NULL;
	}
	if(m_cInputHandler)
	{
		delete m_cInputHandler;
		m_cInputHandler = NULL;
	}
}

void Controller::RunGameLoop()
{
	bool LoopTheGame = true;
	m_timer.startTimer();

	// Load Lua File (for window params)
	LuaReader windowInitScript("Scripts//WindowConfig.lua");
	std::string title = windowInitScript.get<std::string>("WindowConfig.title.title");
	int height = windowInitScript.get<int>("WindowConfig.height");
	int width = windowInitScript.get<int>("WindowConfig.width");
	
	m_cInputHandler = new InputHandler();
	m_cStateHandler = new StateHandler();
	SoundManager::getInstance(); // Create SoundManager Singleton
	theView->setInputHandler(m_cInputHandler);
	m_cStateHandler->Init();

	if (theView->CreateGLWindow(title.c_str(), width, height, 32))
	{
		std::cout << "OpenGL Window Created\n" << std::endl;
	}
	else
	{
		std::cout << "Failed to create OpenGL Context" << std::endl;
	}

	// Setting the first state to render to be the Splash Screen
	m_cStateHandler->ChangeState(new StateSplash("Splash State", theView));

	while(LoopTheGame)
	{
		m_dElapsedTime = m_timer.getElapsedTime();
		m_dAccumulatedTime_ThreadOne += m_dElapsedTime;
		m_dAccumulatedTime_ThreadTwo += m_dElapsedTime;

		if (m_dAccumulatedTime_ThreadOne > 0.01)
		{
			m_cInputHandler->MouseUpdate(theView, m_dAccumulatedTime_ThreadOne);
			m_cInputHandler->KeyboardUpdate(theView, m_dAccumulatedTime_ThreadOne);

			m_cStateHandler->HandleEvents();
			m_cStateHandler->Update(m_dAccumulatedTime_ThreadOne);
			m_dAccumulatedTime_ThreadOne = 0.0;
		}
		if (m_dAccumulatedTime_ThreadTwo > 0.03)
		{
			m_dAccumulatedTime_ThreadTwo = 0.0;
		}

		m_cStateHandler->Draw();

		// For Quitting Purposes (Temporarily)
		if (m_cInputHandler->IsKeyPressed(GLFW_KEY_ESCAPE))
		{
			LoopTheGame = false;
		}
	}
}