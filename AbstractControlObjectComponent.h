#pragma once

#include "control_object_component.pb.h"

#include <string>

namespace ControlObjectAbstractionLayer
{

class AbstractControlObjectComponent
{
public:
	AbstractControlObjectComponent(std::string _prefix_name, std::string _name, int _id_number)
		:prefix_name(_prefix_name), name(_name), id_number(_id_number)
	{}
	virtual ~AbstractControlObjectComponent() {}

	virtual size_t getOutgoingVectorSize() = 0;
	virtual size_t getIncomingVectorSize() = 0;

	virtual bool prepareInitialOutcomingMessage(com_ivanp7_cscp::ControlObjectComponent* component) = 0;
	virtual bool convertIncomingVectorToOutcomingMessage(double* incoming_vector, com_ivanp7_cscp::ControlObjectComponent* component) = 0;
	virtual bool convertIncomingMessageToOutgoingVector(com_ivanp7_cscp::ControlObjectComponent* component, double* outgoing_vector) = 0;

	const std::string& getPrefixName() { return prefix_name; }
	const std::string& getName() { return name; }
	std::string getFullName() { return prefix_name + "@" + name; }
	int getIndex() { return id_number; }

private:
	std::string prefix_name, name;
	int id_number;
};

} // namespace ControlObjectAbstractionLayer
