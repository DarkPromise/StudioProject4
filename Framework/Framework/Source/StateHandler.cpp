#include "StateHandler.h"
#include "State.h"
#include "glfw3.h"

StateHandler::StateHandler() 
: m_bRunning(false)
{

}

StateHandler::~StateHandler()
{
	for (unsigned int i = 0; i < m_deleteList.size(); ++i)
	{
		delete m_deleteList[i];
	}
	m_deleteList.clear();
	m_deleteList.~vector();

	for (unsigned int i = 0; i < m_stateList.size(); ++i)
	{
		m_stateList[i]->Cleanup();
		delete m_stateList[i];
	}
	m_stateList.clear();
	m_stateList.~vector();
}

void StateHandler::Init()
{
	m_bRunning = true;
}

void StateHandler::ChangeState(State* state)
{
	if (!m_stateList.empty())
	{
		// Mr Toh's Memory Leak!
		m_deleteList.push_back(m_stateList.back());
		m_stateList.back()->Cleanup();
		// Popping back here causes the back() state* memory ptr to be lost!
		m_stateList.pop_back();
	}
	m_stateList.push_back(state);
	m_stateList.back()->Init();
}

void StateHandler::PushState(State* state)
{
	if (!m_stateList.empty())
	{
		m_stateList.back()->Pause();
	}
	m_stateList.push_back(state);
	m_stateList.back()->Init();
}

void StateHandler::PopState()
{
	if (!m_stateList.empty())
	{
		m_stateList.back()->Cleanup();
		m_stateList.pop_back();
	}
	if (!m_stateList.empty())
	{
		m_stateList.back()->Resume();
	}
}

void StateHandler::HandleEvents()
{
	m_stateList.back()->HandleEvents(this);
}

void StateHandler::HandleEvents(const int key, bool status)
{
	m_stateList.back()->HandleEvents(this, key, status);
}

void StateHandler::Cleanup()
{
	while (!m_stateList.empty())
	{
		m_stateList.back()->Cleanup();
		m_stateList.pop_back();
	}
}

void StateHandler::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_stateList.back()->Draw(this);
}

void StateHandler::Update(double m_dElaspedTime)
{
	m_stateList.back()->Update(this, m_dElaspedTime);
}