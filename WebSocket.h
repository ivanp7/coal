#pragma once

#include <string>

#include "GlobalContext.h"

namespace ControlObjectAbstractionLayer
{

class WebSocketClient
{
public:
	WebSocketClient(const char* _serverIPString, INTERNET_PORT _serverPort)
		:serverIPString(_serverIPString), serverPort(_serverPort), established(false)
	{}
	virtual ~WebSocketClient()
	{
		closeConnection();
	}

	const char* getServerIP() { return serverIPString.c_str(); }
	INTERNET_PORT getServerPort() { return serverPort; }

	bool establishConnection();
	void closeConnection(USHORT status = WINHTTP_WEB_SOCKET_SUCCESS_CLOSE_STATUS);

	bool isEstablishable() 
	{
		return (GlobalContext::getInstance() != nullptr) && 
			(GlobalContext::getInstance()->webSocketClientHandle == INVALID_HANDLE_VALUE);
	}
	bool isEstablished() { return established; }

	bool sendByteBuffer(BYTE byteBuffer[], DWORD byteBufferLength);
	bool receiveByteBuffer(BYTE byteBuffer[], DWORD& byteBufferLength, DWORD byteBufferCapacity);

private:
	std::string serverIPString;
	INTERNET_PORT serverPort;

	bool established;
};

} //namespace ControlObjectAbstractionLayer
