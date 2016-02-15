#include "CharBuffer.h"

CharBuffer::CharBuffer()
: m_bSelected(false)
, m_bHighlight(false)
, m_fFontSize(0.f)
, m_fHeightOffset(0.f)
, m_fWidthOffset(0.f)
, m_fTextOffset(0.f)
, m_v3Position(Vector3(0.f, 0.f, 0.f))
, m_iTextLimit(20)
{

}

CharBuffer::~CharBuffer()
{

}

void CharBuffer::removeFromBuffer()
{
	if (m_cBuffer.size() > 0)
	{
		m_cBuffer.pop_back();
		m_cBuffer.shrink_to_fit();
	}
}

void CharBuffer::addToBuffer(char c)
{
	if (this->m_cBuffer.size() < this->m_iTextLimit)
	{
		this->m_cBuffer.push_back(c);
	}
}

std::vector<char> CharBuffer::getBuffer()
{
	return this->m_cBuffer;
}

void CharBuffer::printBuffer()
{
	std::string result(this->m_cBuffer.data(),this->m_cBuffer.size());
	std::cout << result << std::endl;
}

void CharBuffer::setText(std::string text)
{
	for (int i = 0; i < text.length(); i++)
	{
		m_cBuffer.push_back(text[i]);
	}
}

std::string CharBuffer::toString()
{
	std::string result(this->m_cBuffer.data(), this->m_cBuffer.size());
	return result;
}

bool CharBuffer::getSelected()
{
	return this->m_bSelected;
}

void CharBuffer::setSelected(bool status)
{
	this->m_bSelected = status;
}

void CharBuffer::setTextOffset(float textOffset)
{
	this->m_fTextOffset = textOffset;
}

float CharBuffer::getTextOffset()
{
	return this->m_fTextOffset;
}

void CharBuffer::setHeightOffset(float heightOffset)
{
	this->m_fHeightOffset = heightOffset;
}

float CharBuffer::getHeightOffset()
{
	return this->m_fHeightOffset;
}

void CharBuffer::setWidthOffset(float widthOffset)
{
	this->m_fWidthOffset = widthOffset;
}

float CharBuffer::getWidthOffset()
{
	return this->m_fWidthOffset;
}

void CharBuffer::setFontSize(float fontSize)
{
	this->m_fFontSize = fontSize;
}

float CharBuffer::getFontSize()
{
	return this->m_fFontSize;
}

void CharBuffer::setHighlighted(bool highlight)
{
	this->m_bHighlight = highlight;
}

bool CharBuffer::getHighlighted()
{
	return this->m_bHighlight;
}

BoundingBox CharBuffer::getBoundingBox()
{
	return this->m_cAABB;
}

void CharBuffer::recomputeOffsets()
{
	this->m_fTextOffset = this->toString().length() * 10.f;
}

void CharBuffer::setPosition(Vector3 newPos)
{
	this->m_v3Position = newPos;
}

Vector3 CharBuffer::getPosition()
{
	return this->m_v3Position;
}

void CharBuffer::initAABB()
{
	this->m_cAABB.setMaxMin(Vector3((toString().length() * (m_fFontSize * 0.5f)) * 0.5f, m_fFontSize * 0.5f, 1.f), Vector3(-(toString().length() * (m_fFontSize * 0.5f)) * 0.5f, -m_fFontSize * 0.5f, 1.f));
}

void CharBuffer::setTextLimit(int textLimit)
{
	this->m_iTextLimit = textLimit;
}

int CharBuffer::getTextLimit()
{
	return this->m_iTextLimit;
}