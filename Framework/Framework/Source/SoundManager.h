#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H

#include <iostream>
#include "irrKlang.h"

#pragma comment (lib,"irrKlang.lib")
using namespace irrklang;

class SoundManager
{
public:
	SoundManager();
	virtual ~SoundManager();

	static void toggleSound();
	static bool getSoundStatus();
private:
	ISoundEngine * soundEngine;
	static bool m_soundStatus;
};

#endif