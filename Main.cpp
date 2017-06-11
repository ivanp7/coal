#include <locale.h>

#include "TCP.h"
#include "WebSocket.h"
#include "DataManager.h"

#define SERIALIZED_MESSAGE_ARRAY_CAPACITY 1048576 // 1 MiB

using namespace ControlObjectAbstractionLayer;

void operate(ControlSystemDataManager* manager, TCPServer* tcpserver, WebSocketClient* wsclient, int channelID)
{
	com_ivanp7_cscp::ControlSystemMessage
		*messageToControlSystem = new com_ivanp7_cscp::ControlSystemMessage,
		*messageFromControlSystem = new com_ivanp7_cscp::ControlSystemMessage;

	DWORD outgoing_vector_size = (DWORD)manager->getOutgoingVectorSize();
	DWORD incoming_vector_size = (DWORD)manager->getIncomingVectorSize();
	double *outgoing_vector = new double[outgoing_vector_size], 
		*incoming_vector = new double[incoming_vector_size];

	DWORD message_buffer_size = 0;
	BYTE *message_buffer = new BYTE[SERIALIZED_MESSAGE_ARRAY_CAPACITY];

	DWORD bytesReceived;

	wprintf(L"\nEverything is ready.\nOperating...");

	bool first_iteration = true;
	while (true)
	{
		if (first_iteration)
		{
			first_iteration = false;
			if (!manager->prepareInitialOutcomingMessage(messageToControlSystem))
			{
				wprintf(L"\nControl system data manager: error preparing the initial outcoming message.");
				break;
			}
		}
		else
		{
			if (!tcpserver->sendByteBuffer((BYTE*)outgoing_vector, outgoing_vector_size * static_cast<DWORD>(manager->getSizeOfDataType())) ||
				!tcpserver->receiveByteBuffer((BYTE*)incoming_vector, bytesReceived, incoming_vector_size * static_cast<DWORD>(manager->getSizeOfDataType())))
			{
				break;
			}

			if (!manager->convertIncomingVectorToOutcomingMessage(incoming_vector, messageToControlSystem))
			{
				wprintf(L"\nControl system data manager: error converting the incoming vector to the outcoming message.");
				break;
			}
		}
		
		message_buffer_size = (DWORD)messageToControlSystem->ByteSizeLong();
		
		if (!messageToControlSystem->SerializeToArray(message_buffer, message_buffer_size))
		{
			wprintf(L"\nProtocol buffers: error serializing message to the byte array.");
			break;
		}
		
		if (!wsclient->sendByteBuffer(message_buffer, message_buffer_size))
		{
			break;
		}

		bool error = false;
		while (true)
		{
			if (!wsclient->receiveByteBuffer(message_buffer, message_buffer_size, SERIALIZED_MESSAGE_ARRAY_CAPACITY))
			{
				error = true;
				break;
			}
			
			if (!messageFromControlSystem->ParseFromArray(message_buffer, message_buffer_size))
			{
				wprintf(L"\nProtocol buffers: error parsing message from the received bytes.");
				error = true;
				break;
			}
			
			if (!manager->checkIncomingMessage(messageFromControlSystem))
				break;
		}
		if (error)
			break;

		if (!manager->convertIncomingMessageToOutgoingVector(messageFromControlSystem, outgoing_vector))
		{
			wprintf(L"\nControl system data manager: error converting the incoming message to the outgoing vector.");
			break;
		}
	}

	wprintf(L"\nOperation has been terminated.\nShutting down.");

	delete[] outgoing_vector;
	delete[] incoming_vector;
	delete[] message_buffer;

	delete messageFromControlSystem;
	delete messageToControlSystem;
}

const wchar_t* getVersionString()
{
	return L"1.1";
}

int main(int argc, char *argv[])
{
	setlocale(LC_CTYPE, "Russian");

	wprintf(L"Control Object Abstraction Layer\n");
	wprintf(L"version %s\n", getVersionString());
	wprintf(L"(Ñ) 2017 Ivan Podmazov\n\n");

	if ((argc < 10) || (strcmp(argv[6], "#") != 0) || (strcmp(argv[8], ":") != 0)) {
		wprintf(L"Usage:\n");
		wprintf(L"coal <channel id> <control system host> <control system port> <outgoing data local port> <incoming data local port>\n");
		wprintf(L"     # <size of type> : <class name>@<name> [...]\n\n");

		wprintf(L"Example:\n");
		wprintf(L"coal 0 localhost 60000 61111 61112 : group1@object1 *3 group1@object2 group2@object3 *2\n\n");

		wprintf(L"Supported control objects:\n");
		ControlSystemDataManager::displaySupportedComponents();
		wprintf(L"\n");

		return -1;
	}

	int channelID = atoi(argv[1]);
	char *remoteHost = argv[2];
	int remotePort = atoi(argv[3]);

	int localPort_out = atoi(argv[4]);
	int localPort_in = atoi(argv[5]);

	size_t sizeof_datatype = atoi(argv[7]);
	if (sizeof_datatype != 8)
	{
		wprintf(L"Base data type size of %zi byte(s) is not currently supported.\n", sizeof_datatype);
		return 1;
	}

	wprintf(L"COAL <-> control system communication settings:\n");
	wprintf(L"-----------------------------------------------\n");
	wprintf(L"Channel ID  : %i\n", channelID);
	wprintf(L"Remote host : %hs\n", remoteHost);
	wprintf(L"Remote port : %i\n", remotePort);
	wprintf(L"\n");

	wprintf(L"COAL <-> control object communication settings:\n");
	wprintf(L"-----------------------------------------------\n");
	wprintf(L"Local port (outgoing data) : %i\n", localPort_out);
	wprintf(L"Local port (incoming data) : %i\n", localPort_in);
	wprintf(L"\n");

	ControlSystemDataManager *manager = new ControlSystemDataManager(sizeof_datatype, argv + 9, argc - 9, channelID);

	wprintf(L"Control object configuration:\n");
	wprintf(L"-----------------------------\n");
	wprintf(L"Number of components : %zi\n", manager->getNumberOfComponents());
	manager->displayComponents(5);
	wprintf(L"\n");

	wprintf(L"COAL <-> control object communication:\n");
	wprintf(L"------------------------------------------------\n");
	wprintf(L"Base data type size  : %zi byte(s)\n", manager->getSizeOfDataType());
	wprintf(L"Outgoing vector size : %zi element(s) = %zi byte(s)\n", manager->getOutgoingVectorSize(), manager->getOutgoingVectorSize() * manager->getSizeOfDataType());
	wprintf(L"Incoming vector size : %zi element(s) = %zi byte(s)\n", manager->getIncomingVectorSize(), manager->getIncomingVectorSize() * manager->getSizeOfDataType());
	wprintf(L"\n");

	wprintf(L"========================================================\n");

	TCPServer *tcpserver = new TCPServer(localPort_out, localPort_in);
	if (!tcpserver->establishConnection())
		return 1;

	WebSocketClient *wsclient = new WebSocketClient(remoteHost, remotePort);
	if (!wsclient->establishConnection())
		return 1;

	operate(manager, tcpserver, wsclient, channelID);

	delete wsclient;
	delete tcpserver;
	delete manager;

	wprintf(L"\nApplication has been shut down.\n\n");

	return 0;
}
