#include "SimulinkMechanics_Factory.h"

namespace ControlObjectAbstractionLayer
{

AbstractControlObjectComponent* SimulinkMechanics_Factory::makeComponent(std::string name, int index)
{
	std::vector<SimulinkMechanics_MechanismPart*> parts;

	size_t l = 0, r;
	std::string sub;
	do
	{
		r = name.find(",", l);
		if (r != std::string::npos)
			sub = name.substr(l, r - l);
		else
			sub = name.substr(l);

		if ((sub.compare("BODY") != 0) &&
			(sub.compare("W") != 0) &&
			(sub.compare("P") != 0) &&
			(sub.compare("R") != 0))
			return nullptr;

		l = r + 1;
	} while (r != std::string::npos);

	l = 0;
	do
	{
		r = name.find(",", l);
		if (r != std::string::npos)
			sub = name.substr(l, r - l);
		else
			sub = name.substr(l);

		if (sub.compare("BODY") == 0)
			parts.push_back(new SimulinkMechanics_Body);
		if (sub.compare("W") == 0)
			parts.push_back(new SimulinkMechanics_WeldJoint);
		if (sub.compare("P") == 0)
			parts.push_back(new SimulinkMechanics_PrismaticJoint);
		if (sub.compare("R") == 0)
			parts.push_back(new SimulinkMechanics_RevoluteJoint);

		l = r + 1;
	} while (r != std::string::npos);

	AbstractControlObjectComponent *mechanism = new SimulinkMechanics_Mechanism(name, index, parts);

	return mechanism;
}

void SimulinkMechanics_Factory::displaySupportedSyntax()
{
	wprintf(L"{BODY|W|P|R}[,...]");
}

} // namespace ControlObjectAbstractionLayer
