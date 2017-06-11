#include "AbstractControlObjectComponentFactory.h"
#include "RawVector.h"
#include "components\simulink_mechanics\SimulinkMechanics_Factory.h"

#include <string>

namespace ControlObjectAbstractionLayer
{

AbstractControlObjectComponent* AbstractControlObjectComponentFactory::makeComponent(std::string fullname, int index)
{
	size_t delim_pos = fullname.find('@');
	if (delim_pos == std::string::npos)
		return nullptr;

	std::string prefix_name = fullname.substr(0, delim_pos);
	std::string name = fullname.substr(delim_pos + 1);
	
	if (prefix_name.compare("raw") == 0)
		return new RawVector(name, index);
	else if (prefix_name.compare("simm") == 0)
		return SimulinkMechanics_Factory::makeComponent(name, index);
	
	return nullptr;
}

void AbstractControlObjectComponentFactory::displaySupportedSyntax()
{
	wprintf(L"raw@in=X,out=Y\n");
	
	wprintf(L"simm@(");
	SimulinkMechanics_Factory::displaySupportedSyntax();
	wprintf(L")\n");
}

} // namespace ControlObjectAbstractionLayer
