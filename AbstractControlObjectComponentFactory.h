#pragma once

#include "AbstractControlObjectComponent.h"

namespace ControlObjectAbstractionLayer
{

class AbstractControlObjectComponentFactory
{
public:
	static AbstractControlObjectComponent* makeComponent(std::string type, int index);
	static void displaySupportedSyntax();
};

} // namespace ControlObjectAbstractionLayer
