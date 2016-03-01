#include "InputHandler.h"
#include "View.h"
#include "CharBuffer.h"

InputHandler::InputHandler()
: mX(0.0)
, mY(0.0)
, dX(0.0)
, dY(0.0)
, last_x(0.0)
, last_y(0.0)
, dScroll(0.0)
, m_bMouseEnabled(true)
, m_bKeyboardEnabled(true)
, m_dClickDelay(0.0)
, m_dPressDelay(0.0)
, m_bBufferMode(false)
{
	std::cout << "InputHandler Created" << std::endl;

	for (int i = 0; i < GLFW_KEY_LAST; i++)
	{
		m_pressedKeys[i] = false;
		m_triggeredKeys[i] = false;
		m_repeatedKeys[i] = false;
	}
}

InputHandler::~InputHandler()
{
	if (!m_charBufferList.empty())
	{
		for (int i = 0; i < m_charBufferList.size(); i++)
		{
			delete m_charBufferList[i];
		}
	}
	m_charBufferList.clear();
	m_charBufferList.~vector();
}

void InputHandler::Key_Callback(int key, int scancode, int action, int mods)
{
	if (m_bKeyboardEnabled)
	{
		if (action == GLFW_PRESS)
		{
			if (m_bBufferMode)
			{
				if (key == GLFW_KEY_BACKSPACE)
				{
					for (int i = 0; i < m_charBufferList.size(); i++)
					{
						if (m_charBufferList[i]->getSelected())
						{
							this->m_charBufferList[i]->removeFromBuffer();
						}
					}
				}
				else if (key == GLFW_KEY_ENTER)
				{
					PressKey(key);
				}
			}
			else
			{
				PressKey(key);
			}
		}
		else if (action == GLFW_RELEASE)
		{
			if (m_bBufferMode)
			{
				if (((key >= GLFW_KEY_A) && (key <= GLFW_KEY_Z)) ||
					((key >= GLFW_KEY_0) && (key <= GLFW_KEY_9)) ||
					((key == GLFW_KEY_PERIOD)))
				{
					if (mods == GLFW_MOD_SHIFT)
					{
						for (int i = 0; i < m_charBufferList.size(); i++)
						{
							if (m_charBufferList[i]->getSelected())
							{
								this->m_charBufferList[i]->addToBuffer((char)key);
							}
						}
					}
					else
					{
						for (int i = 0; i < m_charBufferList.size(); i++)
						{
							if (m_charBufferList[i]->getSelected())
							{
								this->m_charBufferList[i]->addToBuffer(tolower((char)key));
							}
						}
					}
				}
				else if (key == GLFW_KEY_ENTER)
				{
					PressKey(key, false);
				}
			}
			else
			{
				PressKey(key, false);
			}
		}
		else if (action == GLFW_REPEAT)
		{
			if (m_bBufferMode)
			{
				if (((key >= GLFW_KEY_A) && (key <= GLFW_KEY_Z)) ||
					((key >= GLFW_KEY_0) && (key <= GLFW_KEY_9)) ||
					((key == GLFW_KEY_PERIOD)))
				{
					if (mods == GLFW_MOD_SHIFT)
					{
						for (int i = 0; i < m_charBufferList.size(); i++)
						{
							if (m_charBufferList[i]->getSelected())
							{
								this->m_charBufferList[i]->addToBuffer((char)key);
							}
						}
					}
					else
					{
						for (int i = 0; i < m_charBufferList.size(); i++)
						{
							if (m_charBufferList[i]->getSelected())
							{
								this->m_charBufferList[i]->addToBuffer(tolower((char)key));
							}
						}
					}
				}
				else if (key == GLFW_KEY_BACKSPACE)
				{
					for (int i = 0; i < m_charBufferList.size(); i++)
					{
						if (m_charBufferList[i]->getSelected())
						{
							this->m_charBufferList[i]->removeFromBuffer();
						}
					}
				}
				else if (key == GLFW_KEY_ENTER)
				{
					PressKey(key);
				}
			}
			else
			{
				PressKey(key);
			}
		}
	}
}

void InputHandler::Mouse_Callback(int button, int action, int mods)
{
	if (m_bMouseEnabled)
	{
		if (action == GLFW_PRESS)
		{
			PressKey(button);
		}
		else if (action == GLFW_RELEASE)
		{
			PressKey(button, false);
		}
		else if (action == GLFW_REPEAT)
		{
			PressKey(button);
		}
	}
}

void InputHandler::PressKey(int key, bool status)
{
	if (status)
	{
		// Check if key is pressed
		if (m_pressedKeys[key] == false)
		{
			// Set to true
			m_pressedKeys[key] = true;
			if (m_triggeredKeys[key] == false)
			{
				m_triggeredKeys[key] = true;
				//std::cout << key << " has been triggered" << std::endl;
			}
			else
			{
				m_triggeredKeys[key] = false;
				//std::cout << key << " has been un-triggered" << std::endl;
			}
		}
		else
		{
			// Key is still being pressed
			m_repeatedKeys[key] = true;
			//std::cout << key << " is being pressed repeatedly" << std::endl;
		}
	}
	else
	{
		// Key is not pressed anymore
		m_pressedKeys[key] = false;
		m_repeatedKeys[key] = false;
		//std::cout << "Setting " << key << " to false" << std::endl;
	}
}

void InputHandler::KeyboardUpdate(View * theView, double dt)
{
	m_dPressDelay += dt;
}

void InputHandler::MouseUpdate(View * theView, double dt)
{
	if (m_dClickDelay > 0.0)
	{
		m_dClickDelay -= dt;
	}

	glfwGetCursorPos(theView->getWindow(), &mX, &mY);

	float screenXmid = (float)theView->getWindowWidth() * 0.5f;
	float screenYmid = (float)theView->getWindowHeight() * 0.5f;

	this->dX = (screenXmid - this->mX);
	this->dY = (screenYmid - (ceil)(this->mY));

	this->dX *= 0.15;
	this->dY *= 0.15;

	if (m_bMouseEnabled == false)
	{
		glfwSetInputMode(theView->getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetCursorPos(theView->getWindow(), screenXmid, screenYmid);
	}
	else
	{
		glfwSetInputMode(theView->getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}

bool InputHandler::IsKeyPressed(int key) const
{
	return m_pressedKeys[key];
}

bool InputHandler::IsKeyTriggered(int key) const
{
	return m_triggeredKeys[key];
}

bool InputHandler::IsKeyRepeating(int key) const
{
	return m_repeatedKeys[key];
}

bool InputHandler::isKeyboardEnabled()
{
	return m_bKeyboardEnabled;
}

void InputHandler::setKeyboardEnabled(bool status)
{
	this->m_bKeyboardEnabled = status;
}

void InputHandler::setMouseX(double newX)
{
	this->mX = newX;
}

void InputHandler::setMouseY(double newY)
{
	this->mY = newY;
}

void InputHandler::setDeltaScroll(double newDs)
{
	this->dScroll = newDs;
}

void InputHandler::setClickDelay(double delay)
{
	this->m_dClickDelay = delay;
}

void InputHandler::setPressDelay(double delay)
{
	this->m_dPressDelay = delay;
}

double InputHandler::getDeltaX() const
{
	return this->dX;
}

double InputHandler::getDeltaY() const
{
	return this->dY;
}

bool InputHandler::isMouseEnabled()
{
	return m_bMouseEnabled;
}

void InputHandler::setMouseEnabled(bool status)
{
	this->m_bMouseEnabled = status;
}

double InputHandler::getMouseX()
{
	return this->mX;
}

double InputHandler::getMouseY()
{
	return this->mY;
}

double InputHandler::getClickDelay()
{
	return this->m_dClickDelay;
}

double InputHandler::getPressDelay()
{
	return this->m_dPressDelay;
}

void InputHandler::resetMousePosition(View * theView)
{
	glfwSetCursorPos(theView->getWindow(),theView->getWindowWidth() * 0.5, theView->getWindowHeight() * 0.5);
}

CharBuffer * InputHandler::getBuffer(int index)
{
	if (index < m_charBufferList.size())
	{
		return this->m_charBufferList[index];
	}
	return nullptr;
}

void InputHandler::setBufferMode(bool status)
{
	this->m_bBufferMode = status;
}

bool InputHandler::getBufferMode()
{
	return this->m_bBufferMode;
}

void InputHandler::resetBuffer()
{
	for (int i = 0; i < m_charBufferList.size(); i++)
	{
		m_charBufferList[i]->getBuffer().clear();
	}
}

CharBuffer * InputHandler::addNewBuffer()
{
	CharBuffer * newBuffer = new CharBuffer();
	m_charBufferList.push_back(newBuffer);
	return newBuffer;
}

void InputHandler::deleteBuffer()
{
	if (m_charBufferList.size() > 0)
	{
		delete m_charBufferList.back();
		m_charBufferList.pop_back();
	}
}