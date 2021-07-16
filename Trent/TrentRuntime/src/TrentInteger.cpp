#include "TrentInteger.h"
#include "TrentRuntime.h"

namespace trent
{
	TrentInteger::TrentInteger(int value)
		: d_value(value)
	{
	}

	const char* TrentInteger::ToString()
	{
		d_string_repr = std::to_string(this->d_value);
		return d_string_repr.c_str();
	}

	const char* TrentInteger::GetRuntimeName()
	{
		return "Int";
	}
	
	TrentObject* TrentInteger::__operator_add(TrentObject* obj)
	{
		if (strcmp(obj->GetRuntimeName(), "Int") != 0)
		{
			std::string ex_message = std::string("Cannot use + operator on types ") + GetRuntimeName() + std::string(" and ") + obj->GetRuntimeName();

			auto exception = TrentException("TrentInteger", ex_message, "__operator_add::Error");
			exception.Raise();

			return TrentObject_Null;
		}
		
		return MAKE_TRENT_INT(this->d_value + reinterpret_cast<TrentInteger*>(obj)->d_value);
	}
	
	TrentObject* TrentInteger::__operator_sub(TrentObject* obj)
	{
		if (strcmp(obj->GetRuntimeName(), "Int") != 0)
		{
			std::string ex_message = std::string("Cannot use - operator on types ") + GetRuntimeName() + std::string(" and ") + obj->GetRuntimeName();

			auto exception = TrentException("TrentInteger", ex_message, "__operator_sub::Error");
			exception.Raise();

			return TrentObject_Null;
		}

		return MAKE_TRENT_INT(this->d_value - reinterpret_cast<TrentInteger*>(obj)->d_value);
	}
	
	TrentObject* TrentInteger::__operator_mul(TrentObject* obj)
	{
		if (strcmp(obj->GetRuntimeName(), "Int") != 0)
		{
			std::string ex_message = std::string("Cannot use * operator on types ") + GetRuntimeName() + std::string(" and ") + obj->GetRuntimeName();

			auto exception = TrentException("TrentInteger", ex_message, "__operator_mul::Error");
			exception.Raise();

			return TrentObject_Null;
		}

		return MAKE_TRENT_INT(this->d_value * reinterpret_cast<TrentInteger*>(obj)->d_value);
	}
	
	TrentObject* TrentInteger::__operator_div(TrentObject* obj)
	{
		if (strcmp(obj->GetRuntimeName(), "Int") != 0)
		{
			std::string ex_message = std::string("Cannot use / operator on types ") + GetRuntimeName() + std::string(" and ") + obj->GetRuntimeName();

			auto exception = TrentException("TrentInteger", ex_message, "__operator_div::Error");
			exception.Raise();

			return TrentObject_Null;
		}

		return MAKE_TRENT_INT(this->d_value / reinterpret_cast<TrentInteger*>(obj)->d_value);
	}
}
