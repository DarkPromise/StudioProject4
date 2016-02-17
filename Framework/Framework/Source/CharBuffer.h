#ifndef CHAR_BUFFER_H
#define CHAR_BUFFER_H

#pragma warning(disable:4018)

#include <iostream>
#include <vector>
#include <string>

#include "AxisAlignedBoundingBox.h"

class CharBuffer
{
public:
	CharBuffer();
	~CharBuffer();

	std::vector<char> getBuffer();
	void addToBuffer(char c);
	void removeFromBuffer();
	
	void setSelected(bool status);
	bool getSelected();
	void setTextOffset(float textOffset);
	float getTextOffset();
	void setHeightOffset(float heightOffset);
	float getHeightOffset();
	void setWidthOffset(float widthOffset);
	float getWidthOffset();
	void setFontSize(float fontSize);
	float getFontSize();
	void setHighlighted(bool highlight);
	bool getHighlighted();
	void setPosition(Vector3 newPos);
	Vector3 getPosition();
	void setTextLimit(int textLimit);
	int getTextLimit();

	void initAABB();
	BoundingBox getBoundingBox();

	void setText(std::string text);

public:
	std::string toString();
	void printBuffer();
	void recomputeOffsets();
private:
	std::vector<char> m_cBuffer;
	float m_fTextOffset;
	float m_fHeightOffset;
	float m_fWidthOffset;
	float m_fFontSize;
	BoundingBox m_cAABB;
	bool m_bSelected;
	bool m_bHighlight;
	Vector3 m_v3Position;
	int m_iTextLimit;
};

#endif