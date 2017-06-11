#include "SimulinkMechanics_MechanismParts.h"

namespace ControlObjectAbstractionLayer
{

// ///////////////////////// Body /////////////////////////

size_t SimulinkMechanics_Body::getOutgoingVectorSize()
{
	return 6;
}

size_t SimulinkMechanics_Body::getIncomingVectorSize()
{
	return 24;
}

bool SimulinkMechanics_Body::prepareInitialOutcomingMessage(com_ivanp7_cscp::simulink_mechanics::MechanismPart* part)
{
	if (part == nullptr)
		return false;

	part->set_name("BODY");
	part->mutable_body();

	/*
	auto body_state = part->mutable_body()->mutable_state();

	body_state->mutable_position()->set_e1(0);
	body_state->mutable_position()->set_e2(0);
	body_state->mutable_position()->set_e3(0);

	body_state->mutable_velocity()->set_e1(0);
	body_state->mutable_velocity()->set_e2(0);
	body_state->mutable_velocity()->set_e3(0);

	body_state->mutable_angular_velocity()->set_e1(0);
	body_state->mutable_angular_velocity()->set_e2(0);
	body_state->mutable_angular_velocity()->set_e3(0);

	body_state->mutable_rotation_matrix()->mutable_row1()->set_e1(0);
	body_state->mutable_rotation_matrix()->mutable_row1()->set_e2(0);
	body_state->mutable_rotation_matrix()->mutable_row1()->set_e3(0);
	body_state->mutable_rotation_matrix()->mutable_row2()->set_e1(0);
	body_state->mutable_rotation_matrix()->mutable_row2()->set_e2(0);
	body_state->mutable_rotation_matrix()->mutable_row2()->set_e3(0);
	body_state->mutable_rotation_matrix()->mutable_row3()->set_e1(0);
	body_state->mutable_rotation_matrix()->mutable_row3()->set_e2(0);
	body_state->mutable_rotation_matrix()->mutable_row3()->set_e3(0);

	body_state->mutable_acceleration()->set_e1(0);
	body_state->mutable_acceleration()->set_e2(0);
	body_state->mutable_acceleration()->set_e3(0);

	body_state->mutable_angular_acceleration()->set_e1(0);
	body_state->mutable_angular_acceleration()->set_e2(0);
	body_state->mutable_angular_acceleration()->set_e3(0);
	*/
	return true;
}

bool SimulinkMechanics_Body::convertIncomingVectorToOutcomingMessage(double* incoming_vector, com_ivanp7_cscp::simulink_mechanics::MechanismPart* part)
{
	if (part == nullptr)
		return false;

	if (!part->has_body())
		return false;

	auto body_state = part->mutable_body()->mutable_state();

	body_state->mutable_position()->set_e1(incoming_vector[0]);
	body_state->mutable_position()->set_e2(incoming_vector[1]);
	body_state->mutable_position()->set_e3(incoming_vector[2]);

	body_state->mutable_velocity()->set_e1(incoming_vector[3]);
	body_state->mutable_velocity()->set_e2(incoming_vector[4]);
	body_state->mutable_velocity()->set_e3(incoming_vector[5]);

	body_state->mutable_angular_velocity()->set_e1(incoming_vector[6]);
	body_state->mutable_angular_velocity()->set_e2(incoming_vector[7]);
	body_state->mutable_angular_velocity()->set_e3(incoming_vector[8]);

	body_state->mutable_rotation_matrix()->mutable_row1()->set_e1(incoming_vector[9]);
	body_state->mutable_rotation_matrix()->mutable_row1()->set_e2(incoming_vector[10]);
	body_state->mutable_rotation_matrix()->mutable_row1()->set_e3(incoming_vector[11]);
	body_state->mutable_rotation_matrix()->mutable_row2()->set_e1(incoming_vector[12]);
	body_state->mutable_rotation_matrix()->mutable_row2()->set_e2(incoming_vector[13]);
	body_state->mutable_rotation_matrix()->mutable_row2()->set_e3(incoming_vector[14]);
	body_state->mutable_rotation_matrix()->mutable_row3()->set_e1(incoming_vector[15]);
	body_state->mutable_rotation_matrix()->mutable_row3()->set_e2(incoming_vector[16]);
	body_state->mutable_rotation_matrix()->mutable_row3()->set_e3(incoming_vector[17]);

	body_state->mutable_acceleration()->set_e1(incoming_vector[18]);
	body_state->mutable_acceleration()->set_e2(incoming_vector[19]);
	body_state->mutable_acceleration()->set_e3(incoming_vector[20]);

	body_state->mutable_angular_acceleration()->set_e1(incoming_vector[21]);
	body_state->mutable_angular_acceleration()->set_e2(incoming_vector[22]);
	body_state->mutable_angular_acceleration()->set_e3(incoming_vector[23]);

	return true;
}

bool SimulinkMechanics_Body::convertIncomingMessageToOutgoingVector(com_ivanp7_cscp::simulink_mechanics::MechanismPart* part, double* outgoing_vector)
{
	if (part == nullptr)
		return false;

	if (!part->has_body() || !part->mutable_body()->has_reaction())
		return false;

	auto body_reaction = part->mutable_body()->mutable_reaction();

	outgoing_vector[0] = body_reaction->mutable_torque()->e1();
	outgoing_vector[1] = body_reaction->mutable_torque()->e2();
	outgoing_vector[2] = body_reaction->mutable_torque()->e3();

	outgoing_vector[3] = body_reaction->mutable_force()->e1();
	outgoing_vector[4] = body_reaction->mutable_force()->e2();
	outgoing_vector[5] = body_reaction->mutable_force()->e3();

	return true;
}

// ///////////////////////// Weld Joint /////////////////////////

size_t SimulinkMechanics_WeldJoint::getOutgoingVectorSize()
{
	return 0;
}

size_t SimulinkMechanics_WeldJoint::getIncomingVectorSize()
{
	return 6;
}

bool SimulinkMechanics_WeldJoint::prepareInitialOutcomingMessage(com_ivanp7_cscp::simulink_mechanics::MechanismPart* part)
{
	if (part == nullptr)
		return false;

	part->set_name("W");
	part->mutable_weld_joint();

	/*
	auto weld_joint_state = part->mutable_weld_joint()->mutable_state();

	weld_joint_state->mutable_reaction_torque()->set_e1(0);
	weld_joint_state->mutable_reaction_torque()->set_e2(0);
	weld_joint_state->mutable_reaction_torque()->set_e3(0);

	weld_joint_state->mutable_reaction_force()->set_e1(0);
	weld_joint_state->mutable_reaction_force()->set_e2(0);
	weld_joint_state->mutable_reaction_force()->set_e3(0);
	*/
	return true;
}

bool SimulinkMechanics_WeldJoint::convertIncomingVectorToOutcomingMessage(double* incoming_vector, com_ivanp7_cscp::simulink_mechanics::MechanismPart* part)
{
	if (!part->has_weld_joint())
		return false;

	auto weld_joint_state = part->mutable_weld_joint()->mutable_state();

	weld_joint_state->mutable_reaction_torque()->set_e1(incoming_vector[0]);
	weld_joint_state->mutable_reaction_torque()->set_e2(incoming_vector[1]);
	weld_joint_state->mutable_reaction_torque()->set_e3(incoming_vector[2]);

	weld_joint_state->mutable_reaction_force()->set_e1(incoming_vector[3]);
	weld_joint_state->mutable_reaction_force()->set_e2(incoming_vector[4]);
	weld_joint_state->mutable_reaction_force()->set_e3(incoming_vector[5]);

	return true;
}

bool SimulinkMechanics_WeldJoint::convertIncomingMessageToOutgoingVector(com_ivanp7_cscp::simulink_mechanics::MechanismPart* part, double* outgoing_vector)
{
	if (part == nullptr)
		return false;

	if (!part->has_weld_joint() || part->mutable_weld_joint()->has_reaction())
		return false;

	return true;
}

// ///////////////////////// Prismatic Joint /////////////////////////

size_t SimulinkMechanics_PrismaticJoint::getOutgoingVectorSize()
{
	return 1;
}

size_t SimulinkMechanics_PrismaticJoint::getIncomingVectorSize()
{
	return 10;
}

bool SimulinkMechanics_PrismaticJoint::prepareInitialOutcomingMessage(com_ivanp7_cscp::simulink_mechanics::MechanismPart* part)
{
	if (part == nullptr)
		return false;

	part->set_name("P");
	part->mutable_prismatic_joint();

	/*
	auto prismatic_joint_state = part->mutable_prismatic_joint()->mutable_state();

	prismatic_joint_state->set_position(0);
	prismatic_joint_state->set_velocity(0);
	prismatic_joint_state->set_acceleration(0);

	prismatic_joint_state->set_computed_force(0);

	prismatic_joint_state->mutable_reaction_torque()->set_e1(0);
	prismatic_joint_state->mutable_reaction_torque()->set_e2(0);
	prismatic_joint_state->mutable_reaction_torque()->set_e3(0);

	prismatic_joint_state->mutable_reaction_force()->set_e1(0);
	prismatic_joint_state->mutable_reaction_force()->set_e2(0);
	prismatic_joint_state->mutable_reaction_force()->set_e3(0);
	*/
	return true;
}

bool SimulinkMechanics_PrismaticJoint::convertIncomingVectorToOutcomingMessage(double* incoming_vector, com_ivanp7_cscp::simulink_mechanics::MechanismPart* part)
{
	if (part == nullptr)
		return false;

	if (!part->has_prismatic_joint())
		return false;

	auto prismatic_joint_state = part->mutable_prismatic_joint()->mutable_state();

	prismatic_joint_state->set_position(incoming_vector[0]);
	prismatic_joint_state->set_velocity(incoming_vector[1]);
	prismatic_joint_state->set_acceleration(incoming_vector[2]);

	prismatic_joint_state->set_computed_force(incoming_vector[3]);

	prismatic_joint_state->mutable_reaction_torque()->set_e1(incoming_vector[4]);
	prismatic_joint_state->mutable_reaction_torque()->set_e2(incoming_vector[5]);
	prismatic_joint_state->mutable_reaction_torque()->set_e3(incoming_vector[6]);

	prismatic_joint_state->mutable_reaction_force()->set_e1(incoming_vector[7]);
	prismatic_joint_state->mutable_reaction_force()->set_e2(incoming_vector[8]);
	prismatic_joint_state->mutable_reaction_force()->set_e3(incoming_vector[9]);

	return true;
}

bool SimulinkMechanics_PrismaticJoint::convertIncomingMessageToOutgoingVector(com_ivanp7_cscp::simulink_mechanics::MechanismPart* part, double* outgoing_vector)
{
	if (part == nullptr)
		return false;

	if (!part->has_prismatic_joint() || !part->mutable_prismatic_joint()->has_reaction())
		return false;

	outgoing_vector[0] = part->mutable_prismatic_joint()->mutable_reaction()->output();
	
	return true;
}

// ///////////////////////// Revolute Joint /////////////////////////

size_t SimulinkMechanics_RevoluteJoint::getOutgoingVectorSize()
{
	return 1;
}

size_t SimulinkMechanics_RevoluteJoint::getIncomingVectorSize()
{
	return 10;
}

bool SimulinkMechanics_RevoluteJoint::prepareInitialOutcomingMessage(com_ivanp7_cscp::simulink_mechanics::MechanismPart* part)
{
	if (part == nullptr)
		return false;

	part->set_name("R");
	part->mutable_revolute_joint();

	/*
	auto revolute_joint_state = part->mutable_revolute_joint()->mutable_state();

	revolute_joint_state->set_angle(0);
	revolute_joint_state->set_angular_velocity(0);
	revolute_joint_state->set_angular_acceleration(0);

	revolute_joint_state->set_computed_torque(0);

	revolute_joint_state->mutable_reaction_torque()->set_e1(0);
	revolute_joint_state->mutable_reaction_torque()->set_e2(0);
	revolute_joint_state->mutable_reaction_torque()->set_e3(0);

	revolute_joint_state->mutable_reaction_force()->set_e1(0);
	revolute_joint_state->mutable_reaction_force()->set_e2(0);
	revolute_joint_state->mutable_reaction_force()->set_e3(0);
	*/
	return true;
}

bool SimulinkMechanics_RevoluteJoint::convertIncomingVectorToOutcomingMessage(double* incoming_vector, com_ivanp7_cscp::simulink_mechanics::MechanismPart* part)
{
	if (part == nullptr)
		return false;

	if (!part->has_revolute_joint())
		return false;

	auto revolute_joint_state = part->mutable_revolute_joint()->mutable_state();

	revolute_joint_state->set_angle(incoming_vector[0]);
	revolute_joint_state->set_angular_velocity(incoming_vector[1]);
	revolute_joint_state->set_angular_acceleration(incoming_vector[2]);

	revolute_joint_state->set_computed_torque(incoming_vector[3]);

	revolute_joint_state->mutable_reaction_torque()->set_e1(incoming_vector[4]);
	revolute_joint_state->mutable_reaction_torque()->set_e2(incoming_vector[5]);
	revolute_joint_state->mutable_reaction_torque()->set_e3(incoming_vector[6]);

	revolute_joint_state->mutable_reaction_force()->set_e1(incoming_vector[7]);
	revolute_joint_state->mutable_reaction_force()->set_e2(incoming_vector[8]);
	revolute_joint_state->mutable_reaction_force()->set_e3(incoming_vector[9]);

	return true;
}

bool SimulinkMechanics_RevoluteJoint::convertIncomingMessageToOutgoingVector(com_ivanp7_cscp::simulink_mechanics::MechanismPart* part, double* outgoing_vector)
{
	if (part == nullptr)
		return false;

	if (!part->has_revolute_joint() || !part->mutable_revolute_joint()->has_reaction())
		return false;

	outgoing_vector[0] = part->mutable_revolute_joint()->mutable_reaction()->output();
	
	return true;
}

} // namespace ControlObjectAbstractionLayer
