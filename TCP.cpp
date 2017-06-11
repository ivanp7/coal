#include "TCP.h"

namespace ControlObjectAbstractionLayer
{

bool TCPServer::establishConnection()
{
	if (!isEstablishable() || isEstablished())
		return false;
	
	GlobalContext *context = GlobalContext::getInstance();

	GlobalContext::message(L"TCP server: waiting for the control object to connect...");

	bool result = connectTo(outPort, context->tcpServerSendSocket);

	if (!result)
		return false;
	
	context->tcpServerEqualSockets = (inPort == outPort);
	if (inPort != outPort)
	{
		result = connectTo(inPort, context->tcpServerReceiveSocket);
		if (!result)
			return false;
	}
	else
		context->tcpServerReceiveSocket = context->tcpServerSendSocket;

	GlobalContext::message(L"TCP server: connected.");
	GlobalContext::message(L"TCP server is up.");

	established = true;
	return true;
}

bool TCPServer::connectTo(INTERNET_PORT port, SOCKET& s)
{
	SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listenSocket == INVALID_SOCKET)
	{
		GlobalContext::errorDetailedInformation(TEXT("socket"), WSAGetLastError());
		GlobalContext::message(L"TCP server: could not start the server.");
		return false;
	}

	sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	serverAddress.sin_port = htons(port);

	if (bind(listenSocket, (sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
	{
		closesocket(listenSocket);
		GlobalContext::errorDetailedInformation(TEXT("bind"), WSAGetLastError());
		GlobalContext::message(L"TCP server: could not start the server.");
		return false;
	}

	if (listen(listenSocket, 1) == SOCKET_ERROR)
	{
		closesocket(listenSocket);
		GlobalContext::errorDetailedInformation(TEXT("listen"), WSAGetLastError());
		GlobalContext::message(L"TCP server: could not start the server.");
		return false;
	}

	s = accept(listenSocket, NULL, NULL);
	if (s == INVALID_SOCKET)
	{
		closesocket(listenSocket);
		GlobalContext::errorDetailedInformation(TEXT("accept"), WSAGetLastError());
		GlobalContext::message(L"TCP server: could not start the server.");
		return false;
	}

	closesocket(listenSocket);
	return true;
}

void TCPServer::closeConnection()
{
	if (!isEstablished())
		return;

	GlobalContext *context = GlobalContext::getInstance();

	shutdown(context->tcpServerSendSocket, SD_BOTH);
	closesocket(context->tcpServerSendSocket);
	context->tcpServerSendSocket = INVALID_SOCKET;

	if (inPort != outPort)
	{
		shutdown(context->tcpServerReceiveSocket, SD_BOTH);
		closesocket(context->tcpServerReceiveSocket);
		context->tcpServerReceiveSocket = INVALID_SOCKET;
	}

	context->tcpServerEqualSockets = false;

	established = false;

	WaitForSingleObject(GetCurrentThread(), 1000); // 1 sec should be enough for the kernel to close the handle
	GlobalContext::message(L"TCP server is down.");
}

bool TCPServer::sendByteBuffer(BYTE byteBuffer[], DWORD byteBufferLength)
{
	if (!isEstablished())
		return false;

	if ((byteBufferLength == 0) || (byteBuffer == nullptr))
		return false;

	GlobalContext *context = GlobalContext::getInstance();
	int bytesSent = send(context->tcpServerSendSocket, (const char*)byteBuffer, byteBufferLength, 0);
	if (bytesSent == SOCKET_ERROR)
	{
		GlobalContext::errorDetailedInformation(TEXT("send"), WSAGetLastError());
		GlobalContext::message(L"TCP server: error sending bytes.");
		return false;
	}
	else if (bytesSent != byteBufferLength)
	{
		GlobalContext::message(L"TCP server: number of sent bytes != buffer length.");
	}
	
	return true;
}

bool TCPServer::receiveByteBuffer(BYTE byteBuffer[], DWORD& byteBufferLength, DWORD byteBufferCapacity)
{
	if (!isEstablished())
		return false;

	GlobalContext *context = GlobalContext::getInstance();

	BYTE *byteBufferPtr = byteBuffer;
	DWORD bytesReceived;

	ZeroMemory(byteBuffer, byteBufferCapacity);
	byteBufferLength = 0;

	while (byteBufferCapacity > 0)
	{
		bytesReceived = recv(context->tcpServerReceiveSocket, (char*)byteBufferPtr, byteBufferCapacity, 0);

		if (bytesReceived == SOCKET_ERROR)
		{
			GlobalContext::errorDetailedInformation(TEXT("recv"), WSAGetLastError());
			GlobalContext::message(L"TCP server: error receiving bytes.");
			return false;
		}
		else if (bytesReceived == 0)
		{
			GlobalContext::message(L"TCP server: received zero-length buffer.");
			closeConnection();
			break;
		}

		byteBufferPtr += bytesReceived;

		byteBufferLength += bytesReceived;
		byteBufferCapacity -= bytesReceived;
	}

	return byteBufferLength > 0;
}

} //namespace ControlObjectAbstractionLayer
