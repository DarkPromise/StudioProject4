#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H

#include <iostream>
#include "irrKlang.h"

// Singleton Sound Manager

#pragma comment (lib,"irrKlang.lib")
using namespace irrklang;

class SoundManager
{
public:
	static SoundManager & getInstance() 
	{
		static SoundManager soundManager;
		return soundManager;
	}
private:
	SoundManager();
public:
	SoundManager(SoundManager & const) = delete;
	void operator=(SoundManager & const) = delete;
	static void toggleSound();
	static bool getSoundStatus();
	static void playSound(const char * soundPath);
private:
	static ISoundEngine * soundEngine;
	static bool m_soundStatus;
};

#endif