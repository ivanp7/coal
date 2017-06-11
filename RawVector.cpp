#include "RawVector.h"

namespace ControlObjectAbstractionLayer
{

RawVector::RawVector(std::string _name, int _index)
	:AbstractControlObjectComponent("raw", _name, _index), incoming_vector_size(0), outgoing_vector_size(0)
{
	size_t comma_pos = _name.find(',');
	if (comma_pos != std::string::npos)
	{
		std::string in_param = _name.substr(0, comma_pos),
			out_param = _name.substr(comma_pos + 1);

		if ((in_param.find("in=") == 0) && (out_param.find("out=") == 0))
		{
			incoming_vector_size = atoi(in_param.substr(3).c_str());
			outgoing_vector_size = atoi(out_param.substr(4).c_str());
		}
	}
}

size_t RawVector::getOutgoingVectorSize()
{
	return outgoing_vector_size;
}

size_t RawVector::getIncomingVectorSize()
{
	return incoming_vector_size;
}

bool RawVector::prepareInitialOutcomingMessage(com_ivanp7_cscp::ControlObjectComponent* component)
{
	if (component == nullptr)
		return false;

	component->set_description(""); // TODO: user-provided descriptions
	component->set_prefix_name("raw");
	component->set_name(getName().c_str());

	auto raw_vector = component->mutable_raw_vector();
	
	component->mutable_raw_vector()->clear_elements();
	for (int i = 0; i < incoming_vector_size; i++)
		raw_vector->add_elements()->set_double_value(0.0L);
	
	return true;
}

bool RawVector::convertIncomingVectorToOutcomingMessage(double* incoming_vector, com_ivanp7_cscp::ControlObjectComponent* component)
{
	if (!component->has_raw_vector())
		return false;

	auto raw_vector = component->mutable_raw_vector();

	for (int i = 0; i < incoming_vector_size; i++)
		raw_vector->mutable_elements(i)->set_double_value(incoming_vector[i]);

	return true;
}

bool RawVector::convertIncomingMessageToOutgoingVector(com_ivanp7_cscp::ControlObjectComponent* component, double* outgoing_vector)
{
	if (!component->has_raw_vector())
		return false;

	auto raw_vector = component->mutable_raw_vector();

	for (int i = 0; i < outgoing_vector_size; i++)
		outgoing_vector[i] = raw_vector->mutable_elements(i)->double_value();

	return true;
}

} // namespace ControlObjectAbstractionLayer
