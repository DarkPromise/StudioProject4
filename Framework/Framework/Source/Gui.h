#ifndef GUI_H
#define GUI_H

#include "MeshBuilder.h"
#include "AxisAlignedBoundingBox.h"

enum GUI_TYPE
{
	GUI_BUTTON,
	GUI_CONTAINER,
};

class Gui
{
public:
	Gui(std::string guiName);
	virtual ~Gui();

	virtual void Update() = 0;
	virtual BoundingBox getBoundingBox() = 0;
	virtual std::string getText() = 0;
	virtual float getSize() = 0;
	virtual int getLength() = 0;
	virtual void highlightButton(bool highlight) = 0;
	virtual bool getHighlighted() = 0;

	virtual void setMesh(Mesh * mesh);
	virtual Mesh * getMesh();
	virtual void setY(float yPos);
	virtual float getY();
	virtual void setX(float xPos);
	virtual float getX();
	virtual void setWidthOffset(float wOffset);
	virtual float getWidthOffset();
	virtual void setHeightOffset(float hOffset);
	virtual float getHeightOffset();
	virtual void setTextOffset(float tOffset);
	virtual float getTextOffset();
private:
	std::string m_sGuiName;
	Mesh * m_GuiMesh;
	
	float x;
	float y;

	// These values go by percentage (25% of screen width.. etc)
	// 1 = 100%
	// 0.5 = 50%
	// 0.25 = 25%
	float screenWidthoffset;
	float screenHeightoffset;
	float textOffset;
};

#endif