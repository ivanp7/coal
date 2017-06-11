#pragma once

#include "simulink_mechanics.pb.h"

namespace ControlObjectAbstractionLayer
{

class SimulinkMechanics_MechanismPart
{
public:
	virtual size_t getOutgoingVectorSize() = 0;
	virtual size_t getIncomingVectorSize() = 0;

	virtual bool prepareInitialOutcomingMessage(com_ivanp7_cscp::simulink_mechanics::MechanismPart* part) = 0;
	virtual bool convertIncomingVectorToOutcomingMessage(double* incoming_vector, com_ivanp7_cscp::simulink_mechanics::MechanismPart* part) = 0;
	virtual bool convertIncomingMessageToOutgoingVector(com_ivanp7_cscp::simulink_mechanics::MechanismPart* part, double* outgoing_vector) = 0;
};

class SimulinkMechanics_Body : public SimulinkMechanics_MechanismPart
{
public:
	size_t getOutgoingVectorSize();
	size_t getIncomingVectorSize();

	bool prepareInitialOutcomingMessage(com_ivanp7_cscp::simulink_mechanics::MechanismPart* part);
	bool convertIncomingVectorToOutcomingMessage(double* incoming_vector, com_ivanp7_cscp::simulink_mechanics::MechanismPart* part);
	bool convertIncomingMessageToOutgoingVector(com_ivanp7_cscp::simulink_mechanics::MechanismPart* part, double* outgoing_vector);
};

class SimulinkMechanics_WeldJoint : public SimulinkMechanics_MechanismPart
{
public:
	size_t getOutgoingVectorSize();
	size_t getIncomingVectorSize();

	bool prepareInitialOutcomingMessage(com_ivanp7_cscp::simulink_mechanics::MechanismPart* part);
	bool convertIncomingVectorToOutcomingMessage(double* incoming_vector, com_ivanp7_cscp::simulink_mechanics::MechanismPart* part);
	bool convertIncomingMessageToOutgoingVector(com_ivanp7_cscp::simulink_mechanics::MechanismPart* part, double* outgoing_vector);
};

class SimulinkMechanics_PrismaticJoint : public SimulinkMechanics_MechanismPart
{
public:
	size_t getOutgoingVectorSize();
	size_t getIncomingVectorSize();

	bool prepareInitialOutcomingMessage(com_ivanp7_cscp::simulink_mechanics::MechanismPart* part);
	bool convertIncomingVectorToOutcomingMessage(double* incoming_vector, com_ivanp7_cscp::simulink_mechanics::MechanismPart* part);
	bool convertIncomingMessageToOutgoingVector(com_ivanp7_cscp::simulink_mechanics::MechanismPart* part, double* outgoing_vector);
};

class SimulinkMechanics_RevoluteJoint : public SimulinkMechanics_MechanismPart
{
public:
	size_t getOutgoingVectorSize();
	size_t getIncomingVectorSize();

	bool prepareInitialOutcomingMessage(com_ivanp7_cscp::simulink_mechanics::MechanismPart* part);
	bool convertIncomingVectorToOutcomingMessage(double* incoming_vector, com_ivanp7_cscp::simulink_mechanics::MechanismPart* part);
	bool convertIncomingMessageToOutgoingVector(com_ivanp7_cscp::simulink_mechanics::MechanismPart* part, double* outgoing_vector);
};

} // namespace ControlObjectAbstractionLayer
