#include "SoundManager.h"

bool SoundManager::m_soundStatus = true;

SoundManager::SoundManager()
{
	soundEngine = createIrrKlangDevice();

	std::cout << "Sound Engine Started" << std::endl;
}

SoundManager::~SoundManager()
{
	if (soundEngine)
	{
		soundEngine->drop();
		soundEngine = NULL;
	}
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