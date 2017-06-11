#pragma once

#include <memory>

#include <wchar.h>
#include <stdio.h>

#include <winsock2.h>
#include <ws2tcpip.h>

#include <windows.h>
#include <WinHttp.h>

namespace ControlObjectAbstractionLayer
{

class GlobalContext final
{
public:
	static GlobalContext* getInstance()
	{
		static std::unique_ptr<GlobalContext, GlobalContextDeleter> instance(new (std::nothrow) GlobalContext);
		return instance.get();
	}
	GlobalContext(GlobalContext const&) = delete;
	GlobalContext(GlobalContext&&) = delete;
	GlobalContext& operator =(GlobalContext const&) = delete;
	GlobalContext& operator =(GlobalContext&&) = delete;
	
	HINTERNET webSocketClientHandle;
	WSADATA wsaData;
	SOCKET tcpServerSendSocket, tcpServerReceiveSocket;
	bool tcpServerEqualSockets;

	static void message(const WCHAR* text);
	static void errorDetailedInformation(const WCHAR* functionName, DWORD lastError);
	static LPCWSTR multiCharToUniChar(char* charBuffer, WCHAR* wCharBuffer);

private:
	GlobalContext();
	~GlobalContext() {}

	struct GlobalContextDeleter
	{
		void operator ()(GlobalContext* context);
	};
};

} //namespace ControlObjectAbstractionLayer
