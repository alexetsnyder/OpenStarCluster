#include "Message.h"

std::mutex Pipe::_init;
std::mutex Pipe::_messsage;
std::queue<Message> Pipe::_pipeline;


bool Pipe::Listen(MessageDestination reicever, Message& outMessage)
{
	bool success = false;

	_messsage.lock();
	if (!_pipeline.empty())
	{	
		outMessage = _pipeline.front();
		if (reicever == outMessage.GetMessageDestination())
		{
			success = true;
			_pipeline.pop();
		}
	}
	_messsage.unlock();

	return success;
}

void Pipe::SendMessage(MessageType type, MessageDestination dest)
{
	_messsage.lock();
	_pipeline.push(Message(type, dest));
	_messsage.unlock();
}