#pragma once

#include <vector>

#include "control_system_communication_protocol.pb.h"

#include "AbstractControlObjectComponent.h"

namespace ControlObjectAbstractionLayer
{

class ControlSystemDataManager
{
public:
	ControlSystemDataManager(ControlSystemDataManager const&) = delete;
	ControlSystemDataManager(ControlSystemDataManager&&) = delete;
	ControlSystemDataManager& operator =(ControlSystemDataManager const&) = delete;
	ControlSystemDataManager& operator =(ControlSystemDataManager&&) = delete;

	ControlSystemDataManager(size_t _sizeof_datatype, char *argv[], int argc, int _channelID = 0);
	virtual ~ControlSystemDataManager();

	size_t getSizeOfDataType() { return sizeof_datatype; }

	size_t getNumberOfComponents() { return components.size(); }

	size_t getOutgoingVectorSize();
	size_t getIncomingVectorSize();

	int getChannelID() { return channelID; }
	int getMessageAge() { return messageAge; }
	void resetMessageAge() { messageAge = 1; }

	bool checkIncomingMessage(com_ivanp7_cscp::ControlSystemMessage* message);

	bool prepareInitialOutcomingMessage(com_ivanp7_cscp::ControlSystemMessage* message);
	bool convertIncomingVectorToOutcomingMessage(double* incoming_vector, com_ivanp7_cscp::ControlSystemMessage* message);
	bool convertIncomingMessageToOutgoingVector(com_ivanp7_cscp::ControlSystemMessage* message, double* outgoing_vector);

	void displayComponents(int componentsPerLine);
	static void displaySupportedComponents();

private:
	size_t sizeof_datatype;

	int channelID;
	int messageAge;

	std::vector<AbstractControlObjectComponent*> components;
};

} // namespace ControlObjectAbstractionLayer
