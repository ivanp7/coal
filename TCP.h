#pragma once

#include "GlobalContext.h"

namespace ControlObjectAbstractionLayer
{

class TCPServer
{
public:
	TCPServer(INTERNET_PORT _outPort, INTERNET_PORT _inPort)
		:outPort(_outPort), inPort(_inPort), established(false)
	{}
	virtual ~TCPServer()
	{
		closeConnection();
	}

	INTERNET_PORT getOutPort() { return outPort; }
	INTERNET_PORT getInPort() { return inPort; }

	bool establishConnection();
	void closeConnection();

	bool isEstablishable()
	{
		return (outPort > 0) && (inPort > 0) &&
			(GlobalContext::getInstance() != nullptr) &&
			(GlobalContext::getInstance()->tcpServerSendSocket == INVALID_SOCKET) &&
			(GlobalContext::getInstance()->tcpServerReceiveSocket == INVALID_SOCKET);
	}
	bool isEstablished() { return established; }

	bool sendByteBuffer(BYTE byteBuffer[], DWORD byteBufferLength);
	bool receiveByteBuffer(BYTE byteBuffer[], DWORD& byteBufferLength, DWORD byteBufferCapacity);

private:
	INTERNET_PORT outPort, inPort;
	bool connectTo(INTERNET_PORT port, SOCKET& s);

	bool established;
};

} //namespace ControlObjectAbstractionLayer
