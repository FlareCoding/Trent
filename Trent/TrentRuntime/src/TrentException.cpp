#include "TrentException.h"
#include "TrentRuntime.h"

namespace trent
{
	TrentException::TrentException(const std::string& parent_class, const std::string& message, const std::string& name)
		: d_parent_class(parent_class), d_message(message), d_name(name)
	{
	}

	void TrentException::Raise()
	{
		TrentRuntime::RaiseException(this);
	}
	
	std::string TrentException::ToString()
	{
		return d_parent_class + "::" + d_name + ": " + d_message;
	}
}
