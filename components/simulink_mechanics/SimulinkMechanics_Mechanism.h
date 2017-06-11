#pragma once

#include "..\..\AbstractControlObjectComponentFactory.h"
#include "SimulinkMechanics_MechanismParts.h"

#include <vector>

namespace ControlObjectAbstractionLayer
{

class SimulinkMechanics_Mechanism : public AbstractControlObjectComponent
{
public:
	SimulinkMechanics_Mechanism(std::string _name, int _index, std::vector<SimulinkMechanics_MechanismPart*> _parts);
	~SimulinkMechanics_Mechanism();

	size_t getOutgoingVectorSize();
	size_t getIncomingVectorSize();

	bool prepareInitialOutcomingMessage(com_ivanp7_cscp::ControlObjectComponent* component);
	bool convertIncomingVectorToOutcomingMessage(double* incoming_vector, com_ivanp7_cscp::ControlObjectComponent* component);
	bool convertIncomingMessageToOutgoingVector(com_ivanp7_cscp::ControlObjectComponent* component, double* outgoing_vector);

private:
	std::vector<SimulinkMechanics_MechanismPart*> parts;
};

} // namespace ControlObjectAbstractionLayer
