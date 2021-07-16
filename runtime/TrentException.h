#pragma once
#include "TrentCore.h"
#include <string>

namespace trent
{
	class TrentException
	{
	public:
		TRAPI TrentException(const std::string& parent_class, const std::string& message, const std::string& name = "TrentException");

		TRAPI virtual void Raise();

		TRAPI std::string ToString();

	private:
		std::string d_parent_class;
		std::string d_name;
		std::string d_message;
	};
}
