#include "MessageBoard.h"

MessageBoard::MessageBoard()
{

}

MessageBoard::~MessageBoard()
{
	if (!m_messageList.empty())
	{
		for (int i = 0; i < m_messageList.size(); i++)
		{
			delete m_messageList[i];
		}
	}
	m_messageList.clear();
	m_messageList.~vector();
}

void MessageBoard::setBoard(std::vector<Message*> messageBoard)
{
	this->m_messageList = messageBoard;
}

void MessageBoard::addMessage(Message * message)
{
	m_messageList.push_back(message);
}

std::vector<Message*> MessageBoard::getMessageBoard()
{
	return this->m_messageList;
}

void MessageBoard::deleteMessage(int index)
{
	std::swap(m_messageList[index],m_messageList.back());
	m_messageList.pop_back();
}
