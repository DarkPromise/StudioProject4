#include "Gui.h"

Gui::Gui(std::string guiName) : m_sGuiName(guiName)
{
	m_GuiMesh = NULL;
}

Gui::~Gui()
{
	if (m_GuiMesh)
	{
		delete m_GuiMesh;
	}
	m_GuiMesh = NULL;
}

void Gui::setMesh(Mesh * mesh)
{
	this->m_GuiMesh = mesh;
}

Mesh* Gui::getMesh()
{
	return this->m_GuiMesh;
}

void Gui::setY(float posY)
{
	this->y = posY;
}

void Gui::setX(float setX)
{
	this->x = setX;
}

float Gui::getY()
{ 
	return this->y;
}

float Gui::getX()
{
	return this->x;
}

void Gui::setWidthOffset(float wOffset)
{
	this->screenWidthoffset = wOffset;
}

float Gui::getWidthOffset()
{
	return this->screenWidthoffset;
}

void Gui::setHeightOffset(float hOffset)
{
	this->screenHeightoffset = hOffset;
}

float Gui::getHeightOffset()
{
	return this->screenHeightoffset;
}

void Gui::setTextOffset(float tOffset)
{
	this->textOffset = tOffset;
}

float Gui::getTextOffset()
{
	return this->textOffset;
}