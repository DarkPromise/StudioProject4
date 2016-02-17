#ifndef GUI_BUTTON_H
#define GUI_BUTTON_H

#include "GuiManager.h"
#include "AxisAlignedBoundingBox.h"

class GuiButton : public Gui
{
public:
	GuiButton(std::string m_sName, std::string m_sText, float wOffset, float hOffset, const float fontSize) : Gui(m_sName), m_sText(m_sText), m_fFontSize(fontSize), m_bHighlight(false)
	{
		this->setHeightOffset(hOffset);
		this->setWidthOffset(wOffset);
		
		// Auto Formatter upon Creation
		this->setTextOffset(m_sText.length() * 10.f);
		this->m_cAABB.setMaxMin(Vector3((m_sText.length() * (m_fFontSize * 0.5f)) * 0.5f, m_fFontSize * 0.5f , 1.f), Vector3(-(m_sText.length() * (m_fFontSize * 0.5f)) * 0.5f, -m_fFontSize * 0.5f, 1.f));
	}

	~GuiButton();

	void Update();
	BoundingBox getBoundingBox();
	std::string getText();
	void setText(std::string);
	float getSize();
	int getLength();
	void highlightButton(bool highlight);
	bool getHighlighted();
private:
	std::string m_sText;
	BoundingBox m_cAABB;
	const float m_fFontSize;
	bool m_bHighlight;
};


#endif