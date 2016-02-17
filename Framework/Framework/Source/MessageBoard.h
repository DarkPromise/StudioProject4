#ifndef MESSAGE_BOARD_H
#define MESSAGE_BOARD_H

#pragma warning(disable:4018)

class Entity;

#include <vector>
#include <string>

enum MESSAGE_TYPE
{
	MESSAGE_SUPPORT,
	MESSAGE_HELP,
	MESSAGE_UNKNOWN
};

struct Message
{
	Message()
	: message("Empty")
	, from(-1)
	, type(MESSAGE_UNKNOWN)
	{
	}

	MESSAGE_TYPE type;
	std::string message;
	int from;
	Entity * entity;
};

class MessageBoard
{
public:
	MessageBoard();
	~MessageBoard();

	void setBoard(std::vector<Message*> messageBoard);
	void addMessage(Message * message);
	void deleteMessage(int index);
	std::vector<Message*> getMessageBoard();
private:
	std::vector<Message*> m_messageList;
};

#endif