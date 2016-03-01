#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#pragma warning(disable:4018)

#define PRESS_DELAY 0.25

#include <iostream>

class View;

#include "CharBuffer.h"

class InputHandler
{
public:
	InputHandler();
	virtual ~InputHandler();

	// Callbacks
	void Key_Callback(int key, int scancode, int action, int mods);
	void Mouse_Callback(int button, int action, int mods);

	// Keyboard
	void PressKey(int key, bool status = true);

	bool IsKeyPressed(int key) const;
	bool IsKeyTriggered(int key) const;
	bool IsKeyRepeating(int key) const;

	bool isKeyboardEnabled();
	void setKeyboardEnabled(bool status);

	// Mouse
	void setMouseX(double newX);
	void setMouseY(double newY);
	void setDeltaScroll(double newDs);

	double getDeltaX() const;
	double getDeltaY() const;

	double getMouseX();
	double getMouseY();

	bool isMouseEnabled();
	void setMouseEnabled(bool status);

	void resetMousePosition(View * theView);

	// Generic Functions
	void MouseUpdate(View * theView, double dt);
	void KeyboardUpdate(View * theView, double dt);
	void setClickDelay(double delay);
	double getClickDelay();
	void setPressDelay(double delay);
	double getPressDelay();

	// Buffer
	CharBuffer * addNewBuffer();
	CharBuffer * getBuffer(int index);
	void deleteBuffer();
	void setBufferMode(bool status);
	bool getBufferMode();
	void resetBuffer();
private:
	bool m_pressedKeys[348];
	bool m_triggeredKeys[348];
	bool m_repeatedKeys[348];

	double mX, mY;
	double dX, dY;
	double last_x, last_y;
	double dScroll;

	bool m_bMouseEnabled;
	bool m_bKeyboardEnabled;
	double m_dClickDelay;
	double m_dPressDelay;

	bool m_bBufferMode;
	std::vector<CharBuffer*> m_charBufferList;
};

#endif