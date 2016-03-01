#include "StateGameOver.h"
#include "View.h"
#include "LoadTGA.h"
#include "StateAGDevMenu.h"

CRecord playerRecord;
StateGameOver::~StateGameOver()
{
}

State * StateGameOver::getInstance()
{
	return this;
}

void StateGameOver::Init()
{
	// Menu in 2D
	theView->LoadOrthoCamera();
	
	currentLetter = 0;
	tempName = "     ";
	// Create Meshes
	Mesh * newMesh;
	newMesh = MeshBuilder::GenerateText("Source Font", 16, 16);
	newMesh->textureID = LoadTGA("Fonts//source.tga");
	newMesh->alpha = 0.f;
	m_meshList.push_back(newMesh);

	newMesh = MeshBuilder::GenerateQuad("AGDev Menu BG", Color(1.f, 1.f, 1.f), 1.f);
	//newMesh->alpha = 0.f;
	m_meshList.push_back(newMesh);

	newMesh = MeshBuilder::GenerateQuad("Project", Color(0.f, 0.f, 0.f), 1.f);
	newMesh->textureArray[0] = LoadTGA("Images//Project.tga");
	//newMesh->alpha = 0.f;
	m_meshList.push_back(newMesh);

	m_bStartFadeIn = true;
	m_bStartFadeOut = false;
	m_dFadeDelay = 0.0;

	highscore.ReadFromTextFile();
}

void StateGameOver::Update(StateHandler * stateHandler, double dt)
{
	static bool IsKeyPressedReturn = false;
	static bool backsp = false;

	static bool upkey = false;
	static bool downkey = false;

	for (int i = 0; i < 32; ++i)
	{
		if (theView->getInputHandler()->IsKeyPressed(GLFW_KEY_A + i) && !chara[i])
		{
			chara[i] = true;

			if (currentLetter < 5)
			{
				tempName[currentLetter] = 'A' + i;
				currentLetter++;
			}
		}
		else if (!theView->getInputHandler()->IsKeyPressed(GLFW_KEY_A + i) && chara[i])
		{
			chara[i] = false;
		}
	}
	if (theView->getInputHandler()->IsKeyPressed(GLFW_KEY_BACKSPACE) && !backsp)
	{
		backsp = true;
		if (currentLetter > 0)
		{
			tempName[currentLetter - 1] = NULL;
			currentLetter--;
		}
	}
	else if (!theView->getInputHandler()->IsKeyPressed(GLFW_KEY_BACKSPACE) && backsp)
	{
		backsp = false;
	}
	if (theView->getInputHandler()->IsKeyPressed(GLFW_KEY_ENTER) && !IsKeyPressedReturn)
	{
		if (highscore.HighscoreCheck(playerRecord))
		{
			if (playerRecord.getName().size() == 0)
			{
				gamestate = GETTINGPLAYERNAME;
			}
			else
			{
				highscore.storeNewRecord(playerRecord);
				gamestate = MAINMENU;
				playerRecord.reset();
			}
		}
		else
		{
			gamestate = MAINMENU;
			playerRecord.reset();
		}
		if (gamestate == GETTINGPLAYERNAME && currentLetter > 0)
		{
			std::string tempd = "      ";
			for (int i = 1; i < 6; ++i)
			{
				if (tempName[i - 1] != NULL)
				{
					tempd[i] = tempName[i - 1];
				}
				else
					break;
			}
			playerRecord.setName(tempd);
			//store record
			highscore.storeNewRecord(playerRecord);

			playerRecord.getTiming().Reset();
			gamestate = MAINMENU;
		}
	}
	else if (!theView->getInputHandler()->IsKeyPressed(GLFW_KEY_ENTER) && IsKeyPressedReturn)
	{
		IsKeyPressedReturn = false;
	}
	
	
}


void StateGameOver::HandleEvents(StateHandler * stateHandler)
{
	if (gamestate == MAINMENU)
	{
		stateHandler->ChangeState(new StateAGDevMenu("AGDev Menu State", theView, false));
	}
}

void StateGameOver::HandleEvents(StateHandler * stateHandler, const int key, const bool status)
{
}

void StateGameOver::Cleanup()
{
	if (!m_meshList.empty())
	{
		for (int i = 0; i < m_meshList.size(); i++)
		{
			delete m_meshList[i];
		}
	}
	m_meshList.clear();
	m_meshList.~vector();
}

void StateGameOver::Pause()
{

}

void StateGameOver::Resume()
{

}

void StateGameOver::Draw(StateHandler* stateHandler)
{	
	RenderBackground();	
	RenderHighscore();
	theView->SwapBuffers();
}

void StateGameOver::RenderBackground()
{
	theView->Render2DMesh(m_meshList[1], false, false, (float)theView->getWindowWidth(), (float)theView->getWindowHeight(), (float)theView->getWindowWidth() * 0.5f, (float)theView->getWindowHeight() * 0.5f);
	theView->Render2DMesh(m_meshList[2], false, false, 400.f * ((float)theView->getWindowWidth() / theView->getWindowHeight()), 150.f * ((float)theView->getWindowWidth() / theView->getWindowHeight()), (float)theView->getWindowWidth() * 0.5f, (float)theView->getWindowHeight() * 0.7f);
}

void StateGameOver::FadeInEffect(double dt)
{
	if (m_meshList[0]->alpha < 1)
	{
		for (Mesh * mesh : m_meshList)
		{
			mesh->alpha += 2.f * dt;
		}
	}
	else
	{
		m_bStartFadeIn = false;
	}
}

void StateGameOver::FadeOutEffect(double dt, StateHandler * stateHandler)
{
	for (Mesh * mesh : m_meshList)
	{
		mesh->alpha -= 2.f * dt;
	}
}

void StateGameOver::RenderHighscore()
{
	std::ostringstream ss;
	
	ss << "Your time: " << playerRecord.getTiming().getMin() << ":" << playerRecord.getTiming().getSec() << std::endl;

	theView->RenderTextOnScreen(m_meshList[TEXT_FONT], ss.str(), Color(1.f, 0.f, 0.f), 36.f, (float)theView->getWindowWidth() * 0.5f, (float)theView->getWindowHeight() * 0.5f);


	theView->RenderTextOnScreen(m_meshList[TEXT_FONT], "Please write your name: ", Color(1.f, 0.f, 0.f), 36.f, (float)theView->getWindowWidth() * 0.5f, (float)theView->getWindowHeight() * 0.4f);
	theView->RenderTextOnScreen(m_meshList[TEXT_FONT], tempName, Color(1.f, 0.f, 0.f), 36.f, (float)theView->getWindowWidth() * 0.5f, (float)theView->getWindowHeight() * 0.4f);


}