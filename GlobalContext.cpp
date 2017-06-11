#include "GlobalContext.h"

namespace ControlObjectAbstractionLayer
{

GlobalContext::GlobalContext()
	:webSocketClientHandle(INVALID_HANDLE_VALUE), tcpServerSendSocket(INVALID_SOCKET), tcpServerReceiveSocket(INVALID_SOCKET), tcpServerEqualSockets(false)
{
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != NO_ERROR)
	{
		GlobalContext::errorDetailedInformation(TEXT("WSAStartup"), WSAGetLastError());
		return;
	}
}

void GlobalContext::GlobalContextDeleter::operator ()(GlobalContext* context)
{
	if (context->webSocketClientHandle != INVALID_HANDLE_VALUE)
		WinHttpWebSocketClose(context->webSocketClientHandle, WINHTTP_WEB_SOCKET_SUCCESS_CLOSE_STATUS, NULL, 0);

	if (context->tcpServerSendSocket != INVALID_SOCKET)
	{
		shutdown(context->tcpServerSendSocket, SD_BOTH);
		closesocket(context->tcpServerSendSocket);
	}
	if ((context->tcpServerReceiveSocket != INVALID_SOCKET) && !context->tcpServerEqualSockets)
	{
		shutdown(context->tcpServerReceiveSocket, SD_BOTH);
		closesocket(context->tcpServerReceiveSocket);
	}

	WSACleanup();

	WaitForSingleObject(GetCurrentThread(), 1000); // 1 sec should be enough for the kernel to release all resources
}

void GlobalContext::message(const WCHAR* text)
{
	wprintf(L"\n%s", text);
}

void GlobalContext::errorDetailedInformation(const WCHAR* functionName, DWORD lastError)
{
	LPVOID messageBuffer;

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		lastError,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&messageBuffer,
		0, NULL);

	wprintf(TEXT("[%s] failed with error %u: %s"), functionName, lastError, (LPTSTR)messageBuffer);
}

LPCWSTR GlobalContext::multiCharToUniChar(char* charBuffer, WCHAR* wCharBuffer)
{
	size_t len = strlen(charBuffer) + 1;
	size_t reqsize = 0;
	mbstowcs_s(&reqsize, NULL, 0, charBuffer, len);
	if (!reqsize) return NULL;
	mbstowcs_s(NULL, &wCharBuffer[0], len, charBuffer, len);
	return (LPCWSTR)wCharBuffer;
}

} //namespace ControlObjectAbstractionLayer
