#include "DataManager.h"

#include "AbstractControlObjectComponentFactory.h"

#include <string>

namespace ControlObjectAbstractionLayer
{

ControlSystemDataManager::ControlSystemDataManager(size_t _sizeof_datatype, char *argv[], int argc, int _channelID)
	:sizeof_datatype(_sizeof_datatype), channelID(_channelID), messageAge(1)
{
	std::string arg, last_arg;

	std::string numstr;
	int number_of;

	AbstractControlObjectComponent *component = nullptr;

	int index = 0;
	for (int i = 0; i < argc; i++)
	{
		index++;

		arg = argv[i];

		if (arg[0] == '*')
		{
			numstr = arg.substr(1);
			number_of = atoi(numstr.c_str());

			if (component != nullptr)
				for (int j = 1; j < number_of; j++)
				{
					component = AbstractControlObjectComponentFactory::makeComponent(last_arg, index);
					components.push_back(component);
				}
		}
		else
		{
			component = AbstractControlObjectComponentFactory::makeComponent(arg, index);
			if (component != nullptr)
				components.push_back(component);
			else
				wprintf(L"\nIncorrect argument \"%hs\" is given, skipping.", arg.c_str());

			last_arg = arg;
		}
	}
}

ControlSystemDataManager::~ControlSystemDataManager()
{
	for (auto component : components)
		delete component;
}

size_t ControlSystemDataManager::getOutgoingVectorSize()
{
	size_t actuatorVectorSize = 0;
	for (auto component : components)
		actuatorVectorSize += component->getOutgoingVectorSize();

	return actuatorVectorSize;
}

size_t ControlSystemDataManager::getIncomingVectorSize()
{
	size_t sensorVectorSize = 0;
	for (auto component : components)
		sensorVectorSize += component->getIncomingVectorSize();
	
	return sensorVectorSize;
}

bool ControlSystemDataManager::checkIncomingMessage(com_ivanp7_cscp::ControlSystemMessage* message)
{
	if ((message->direction() == com_ivanp7_cscp::Direction::FROM_CONTROL_SYSTEM_TO_OBJECT) &&
		(message->communication_channel_id() == channelID) &&
		(message->age() == messageAge))
		return false;
	else
		return true;
}

bool ControlSystemDataManager::prepareInitialOutcomingMessage(com_ivanp7_cscp::ControlSystemMessage* message)
{
	if (message == nullptr)
		return false;

	message->set_description(""); // TODO: user-provided descriptions
	message->set_communication_channel_id(channelID);

	message->set_direction(com_ivanp7_cscp::Direction::INITIAL_FROM_OBJECT_TO_CONTROL_SYSTEM);
	message->set_age(0);

	message->clear_components();
	for (int i = 0; i < components.size(); i++) 
		if (!components[i]->prepareInitialOutcomingMessage(message->add_components()))
			return false;

	return true;
}

bool ControlSystemDataManager::convertIncomingVectorToOutcomingMessage(double* incoming_vector, com_ivanp7_cscp::ControlSystemMessage* message)
{
	if (message == nullptr)
		return false;

	message->set_direction(com_ivanp7_cscp::Direction::FROM_OBJECT_TO_CONTROL_SYSTEM);
	message->set_age(messageAge);
	messageAge++;

	size_t displacement = 0;
	for (int i = 0; i < components.size(); i++)
	{
		if (!components[i]->convertIncomingVectorToOutcomingMessage(incoming_vector + displacement, message->mutable_components(i)))
			return false;
		displacement += components[i]->getIncomingVectorSize();
	}

	return true;
}

bool ControlSystemDataManager::convertIncomingMessageToOutgoingVector(com_ivanp7_cscp::ControlSystemMessage* message, double* outgoing_vector)
{
	if (message == nullptr)
		return false;

	size_t displacement = 0;
	for (int i = 0; i < components.size(); i++)
	{
		if (!components[i]->convertIncomingMessageToOutgoingVector(message->mutable_components(i), outgoing_vector + displacement))
			return false;
		displacement += components[i]->getOutgoingVectorSize();
	}

	return true;
}

void ControlSystemDataManager::displayComponents(int componentsPerLine)
{
	if (componentsPerLine <= 0)
		return;

	for (int i = 0; i < components.size(); i++)
	{
		if (i % componentsPerLine == 0)
			wprintf(L"\n");

		if (i < components.size() - 1)
			wprintf(L"%i) %hs  ", i + 1, components[i]->getFullName().c_str());
		else
			wprintf(L"%i) %hs", i + 1, components[i]->getFullName().c_str());
	}
	wprintf(L"\n");
}

void ControlSystemDataManager::displaySupportedComponents()
{
	AbstractControlObjectComponentFactory::displaySupportedSyntax();
}

} // namespace ControlObjectAbstractionLayer
