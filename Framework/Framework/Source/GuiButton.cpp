#include "GuiButton.h"

GuiButton::~GuiButton()
{

}

void GuiButton::Update()
{
	//Update Bounding Box
}

BoundingBox GuiButton::getBoundingBox()
{
	return this->m_cAABB;
}

std::string GuiButton::getText()
{
	return this->m_sText;
}

void GuiButton::setText(std::string text)
{
	this->m_sText = text;
}

float GuiButton::getSize()
{
	return this->m_fFontSize;
}

int GuiButton::getLength()
{
	return this->m_sText.length();
}

void GuiButton::highlightButton(bool highlight)
{
	m_bHighlight = highlight;
}

bool GuiButton::getHighlighted()
{
	return m_bHighlight;
}