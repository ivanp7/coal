#include "WebSocket.h"

namespace ControlObjectAbstractionLayer
{

bool WebSocketClient::establishConnection()
{
	if (!isEstablishable() || isEstablished())
		return false;

	WCHAR serverAddress[64];
	{
		char buffer[64];
		sprintf_s(buffer, "%s", serverIPString.c_str());
		GlobalContext::multiCharToUniChar(buffer, serverAddress);
	}

	HINTERNET sessionHandle = NULL;
	HINTERNET connectionHandle = NULL;
	HINTERNET requestHandle = NULL;
	const WCHAR *pcwszPath = L"";

	sessionHandle = WinHttpOpen(L"Simulink abstraction layer", WINHTTP_ACCESS_TYPE_AUTOMATIC_PROXY, NULL, NULL, 0);
	if (sessionHandle == INVALID_HANDLE_VALUE)
	{
		GlobalContext::errorDetailedInformation(TEXT("WinHttpOpen"), GetLastError());
		GlobalContext::message(L"WebSocket client: could not connect to the server.");
		return false;
	}

	connectionHandle = WinHttpConnect(sessionHandle, serverAddress, serverPort, 0);
	if (connectionHandle == INVALID_HANDLE_VALUE)
	{
		WinHttpCloseHandle(sessionHandle);
		GlobalContext::errorDetailedInformation(TEXT("WinHttpConnect"), GetLastError());
		GlobalContext::message(L"WebSocket client: could not connect to the server.");
		return false;
	}

	requestHandle = WinHttpOpenRequest(connectionHandle, L"GET", pcwszPath, NULL, NULL, NULL, 0);
	if (requestHandle == INVALID_HANDLE_VALUE)
	{
		WinHttpCloseHandle(sessionHandle);
		WinHttpCloseHandle(connectionHandle);
		GlobalContext::errorDetailedInformation(TEXT("WinHttpOpenRequest"), GetLastError());
		GlobalContext::message(L"WebSocket client: could not connect to the server.");
		return false;
	}

	if (!WinHttpSetOption(requestHandle, WINHTTP_OPTION_UPGRADE_TO_WEB_SOCKET, NULL, 0))
	{
		WinHttpCloseHandle(sessionHandle);
		WinHttpCloseHandle(connectionHandle);
		WinHttpCloseHandle(requestHandle);
		GlobalContext::errorDetailedInformation(TEXT("WinHttpSetOption"), GetLastError());
		GlobalContext::message(L"WebSocket client: could not connect to the server.");
		return false;
	}

	if (!WinHttpSendRequest(requestHandle, WINHTTP_NO_ADDITIONAL_HEADERS, 0, NULL, 0, 0, 0))
	{
		WinHttpCloseHandle(sessionHandle);
		WinHttpCloseHandle(connectionHandle);
		WinHttpCloseHandle(requestHandle);
		GlobalContext::errorDetailedInformation(TEXT("WinHttpSendRequest"), GetLastError());
		GlobalContext::message(L"WebSocket client: could not connect to the server.");
		return false;
	}

	if (!WinHttpReceiveResponse(requestHandle, NULL))
	{
		WinHttpCloseHandle(sessionHandle);
		WinHttpCloseHandle(connectionHandle);
		WinHttpCloseHandle(requestHandle);
		GlobalContext::errorDetailedInformation(TEXT("WinHttpReceiveResponse"), GetLastError());
		GlobalContext::message(L"WebSocket client: could not connect to the server.");
		return false;
	}

	GlobalContext *context = GlobalContext::getInstance();
	context->webSocketClientHandle = WinHttpWebSocketCompleteUpgrade(requestHandle, NULL);

	WinHttpCloseHandle(sessionHandle);
	WinHttpCloseHandle(connectionHandle);
	WinHttpCloseHandle(requestHandle);

	if (context->webSocketClientHandle == INVALID_HANDLE_VALUE)
	{
		GlobalContext::errorDetailedInformation(TEXT("WinHttpWebSocketCompleteUpgrade"), GetLastError());
		GlobalContext::message(L"WebSocket client: could not connect to the server.");
		return false;
	}

	GlobalContext::message(L"WebSocket client is up.");

	established = true;
	return true;
}

void WebSocketClient::closeConnection(USHORT status)
{
	if (!isEstablished())
		return;
	
	GlobalContext *context = GlobalContext::getInstance();
	WinHttpWebSocketClose(context->webSocketClientHandle, status, NULL, 0);
	context->webSocketClientHandle = INVALID_HANDLE_VALUE;
	established = false;

	WaitForSingleObject(GetCurrentThread(), 1000); // 1 sec should be enough for the kernel to close the handle
	GlobalContext::message(L"WebSocket client is down.");
}

bool WebSocketClient::sendByteBuffer(BYTE byteBuffer[], DWORD byteBufferLength)
{
	if (!isEstablished())
		return false;

	if ((byteBufferLength == 0) || (byteBuffer == nullptr))
		return false;

	GlobalContext *context = GlobalContext::getInstance();

	if (WinHttpWebSocketSend(context->webSocketClientHandle, WINHTTP_WEB_SOCKET_BINARY_MESSAGE_BUFFER_TYPE, byteBuffer, byteBufferLength) != NO_ERROR)
	{
		GlobalContext::errorDetailedInformation(TEXT("WinHttpWebSocketSend"), GetLastError());
		return false;
	}

	if (FlushFileBuffers(context->webSocketClientHandle) != NO_ERROR)
	{
		GlobalContext::errorDetailedInformation(TEXT("FlushFileBuffers"), GetLastError());
		return false;
	}

	return true;
}

bool WebSocketClient::receiveByteBuffer(BYTE byteBuffer[], DWORD& byteBufferLength, DWORD byteBufferCapacity)
{
	if (!isEstablished())
		return false;

	BYTE *byteBufferPointer = byteBuffer;
	ZeroMemory(byteBuffer, byteBufferCapacity);

	DWORD byteBufferAvailableLength = byteBufferCapacity;
	DWORD bytesTransferred = 0;
	byteBufferLength = 0;

	WINHTTP_WEB_SOCKET_BUFFER_TYPE eBufferType = WINHTTP_WEB_SOCKET_CLOSE_BUFFER_TYPE;

	do
	{
		if (byteBufferAvailableLength == 0)
		{
			GlobalContext::message(L"WebSocket client: could not receive message from the server. Out of buffer memory.");
			break;
		}

		if (WinHttpWebSocketReceive(GlobalContext::getInstance()->webSocketClientHandle, byteBufferPointer, byteBufferAvailableLength, &bytesTransferred, &eBufferType) != NO_ERROR)
		{
			GlobalContext::errorDetailedInformation(TEXT("WinHttpWebSocketReceive"), GetLastError());
			break;
		}

		byteBufferPointer += bytesTransferred;
		byteBufferLength += bytesTransferred;

		byteBufferAvailableLength -= bytesTransferred;
	} while (eBufferType == WINHTTP_WEB_SOCKET_BINARY_FRAGMENT_BUFFER_TYPE || eBufferType == WINHTTP_WEB_SOCKET_UTF8_FRAGMENT_BUFFER_TYPE);

	if (eBufferType == WINHTTP_WEB_SOCKET_CLOSE_BUFFER_TYPE)
	{
		GlobalContext::message(L"WebSocket client: received 'close'-type buffer.");
		closeConnection();
	}

	return byteBufferLength > 0;
}

} //namespace ControlObjectAbstractionLayer
