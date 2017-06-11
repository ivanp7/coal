#pragma once

#include "SimulinkMechanics_Mechanism.h"

namespace ControlObjectAbstractionLayer
{

class SimulinkMechanics_Factory
{
public:
	static AbstractControlObjectComponent* makeComponent(std::string name, int index);
	static void displaySupportedSyntax();
};

} // namespace ControlObjectAbstractionLayer
