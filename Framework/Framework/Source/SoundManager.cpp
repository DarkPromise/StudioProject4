#include "SoundManager.h"

bool SoundManager::m_soundStatus = true;
ISoundEngine *SoundManager::soundEngine;

SoundManager::SoundManager()
{
	soundEngine = createIrrKlangDevice();
	std::cout << "Sound Engine Started" << std::endl;
}

void SoundManager::toggleSound()
{
	if (SoundManager::m_soundStatus)
	{
		SoundManager::m_soundStatus = false;
	}
	else
	{
		SoundManager::m_soundStatus = true;
	}
}

bool SoundManager::getSoundStatus()
{
	return SoundManager::m_soundStatus;
}

void SoundManager::playSound(const char * soundPath, bool loop)
{
	if (m_soundStatus)
	{
		soundEngine->play2D(soundPath, loop);
	}
}

void SoundManager::stopAllSounds()
{
	soundEngine->stopAllSounds();
}

void SoundManager::setSoundVolume(float volumeLevel)
{
	soundEngine->setSoundVolume(volumeLevel);
}