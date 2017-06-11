#include "SimulinkMechanics_Mechanism.h"

#include <string>

namespace ControlObjectAbstractionLayer
{

SimulinkMechanics_Mechanism::SimulinkMechanics_Mechanism(std::string _name, int _index, std::vector<SimulinkMechanics_MechanismPart*> _parts)
	:AbstractControlObjectComponent("simm", _name, _index), parts(_parts)
{
}

SimulinkMechanics_Mechanism::~SimulinkMechanics_Mechanism()
{
	for (auto part : parts)
		delete part;
}

size_t SimulinkMechanics_Mechanism::getOutgoingVectorSize()
{
	size_t size = 0;
	for (auto part : parts)
		size += part->getOutgoingVectorSize();

	return size;
}

size_t SimulinkMechanics_Mechanism::getIncomingVectorSize()
{
	size_t size = 0;
	for (auto part : parts)
		size += part->getIncomingVectorSize();

	return size;
}

bool SimulinkMechanics_Mechanism::prepareInitialOutcomingMessage(com_ivanp7_cscp::ControlObjectComponent* component)
{
	if (component == nullptr)
		return false;

	component->set_description(""); // TODO: user-provided descriptions
	component->set_prefix_name("simm");
	component->set_name(getName().c_str());

	auto mechanism = component->mutable_simulink_mechanics_mechanism();

	mechanism->clear_parts();
	for (int i = 0; i < parts.size(); i++) 
		if (!parts[i]->prepareInitialOutcomingMessage(mechanism->add_parts()))
			return false;

	return true;
}

bool SimulinkMechanics_Mechanism::convertIncomingVectorToOutcomingMessage(double* incoming_vector, com_ivanp7_cscp::ControlObjectComponent* component)
{
	if (!component->has_simulink_mechanics_mechanism())
		return false;

	auto mechanism = component->mutable_simulink_mechanics_mechanism();

	size_t displacement = 0;
	for (int i = 0; i < parts.size(); i++)
	{
		if (!parts[i]->convertIncomingVectorToOutcomingMessage(incoming_vector + displacement, mechanism->mutable_parts(i)))
			return false;
		displacement += parts[i]->getIncomingVectorSize();
	}
	
	return true;
}

bool SimulinkMechanics_Mechanism::convertIncomingMessageToOutgoingVector(com_ivanp7_cscp::ControlObjectComponent* component, double* outgoing_vector)
{
	if (!component->has_simulink_mechanics_mechanism())
		return false;
	
	auto mechanism = component->mutable_simulink_mechanics_mechanism();

	size_t displacement = 0;
	for (int i = 0; i < parts.size(); i++)
	{
		if (!parts[i]->convertIncomingMessageToOutgoingVector(mechanism->mutable_parts(i), outgoing_vector + displacement))
			return false;
		displacement += parts[i]->getOutgoingVectorSize();
	}
	
	return true;
}

} // namespace ControlObjectAbstractionLayer
