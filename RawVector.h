#pragma once

#include "AbstractControlObjectComponentFactory.h"

#include <vector>

namespace ControlObjectAbstractionLayer
{

class RawVector : public AbstractControlObjectComponent
{
public:
	RawVector(std::string _name, int _index);
	~RawVector() {}

	size_t getOutgoingVectorSize();
	size_t getIncomingVectorSize();

	bool prepareInitialOutcomingMessage(com_ivanp7_cscp::ControlObjectComponent* component);
	bool convertIncomingVectorToOutcomingMessage(double* incoming_vector, com_ivanp7_cscp::ControlObjectComponent* component);
	bool convertIncomingMessageToOutgoingVector(com_ivanp7_cscp::ControlObjectComponent* component, double* outgoing_vector);

private:
	size_t incoming_vector_size, outgoing_vector_size;
};

} // namespace ControlObjectAbstractionLayer
