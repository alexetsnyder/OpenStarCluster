#pragma once
#include <queue>
#include <mutex>

enum MessageType
{
	STOP,
	LOAD,
	LOADED,
	MERGED
};

enum MessageDestination
{
	LOADCHUNKS,
	GRAPHICS
};

class Message
{
	public:
		Message() {}
		Message(MessageType type, MessageDestination destination) { _messageType = type; _messageDest = destination; }

		MessageType GetMessageType() { return _messageType; }
		MessageDestination GetMessageDestination() { return _messageDest; }
	private:
		MessageType _messageType;
		MessageDestination _messageDest;
};

class Pipe
{
	public:
		static bool Listen(MessageDestination reicever, Message& outMessage);
		static void SendMessage(MessageType type, MessageDestination dest);

	private:
		static std::mutex _init;
		static std::mutex _messsage;
		static std::queue<Message> _pipeline;
};
