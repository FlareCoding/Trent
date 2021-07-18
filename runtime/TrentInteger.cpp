#include "TrentInteger.h"
#include "TrentRuntime.h"
#include "TrentBoolean.h"

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

	void TrentInteger::CopyFrom(TrentObject* other)
	{
		if (strcmp(other->GetRuntimeName(), "Int") != 0)
		{
			auto exception = TrentException("TrentInteger", "Cannot copy object buffer", "CopyObject::Error");
			exception.Raise();
			return;
		}

		this->d_value = reinterpret_cast<TrentInteger*>(other)->d_value;
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
	
	TrentObject* TrentInteger::__operator_lt(TrentObject* obj)
	{
		if (strcmp(obj->GetRuntimeName(), "Int") != 0)
		{
			std::string ex_message = std::string("Cannot use < operator on types ") + GetRuntimeName() + std::string(" and ") + obj->GetRuntimeName();

			auto exception = TrentException("TrentInteger", ex_message, "__operator_lt::Error");
			exception.Raise();

			return TrentObject_Null;
		}

		return MAKE_TRENT_BOOLEAN((this->d_value < reinterpret_cast<TrentInteger*>(obj)->d_value));
	}
	
	TrentObject* TrentInteger::__operator_gt(TrentObject* obj)
	{
		if (strcmp(obj->GetRuntimeName(), "Int") != 0)
		{
			std::string ex_message = std::string("Cannot use > operator on types ") + GetRuntimeName() + std::string(" and ") + obj->GetRuntimeName();

			auto exception = TrentException("TrentInteger", ex_message, "__operator_gt::Error");
			exception.Raise();

			return TrentObject_Null;
		}

		return MAKE_TRENT_BOOLEAN((this->d_value > reinterpret_cast<TrentInteger*>(obj)->d_value));
	}
	
	TrentObject* TrentInteger::__operator_ltoe(TrentObject* obj)
	{
		if (strcmp(obj->GetRuntimeName(), "Int") != 0)
		{
			std::string ex_message = std::string("Cannot use <= operator on types ") + GetRuntimeName() + std::string(" and ") + obj->GetRuntimeName();

			auto exception = TrentException("TrentInteger", ex_message, "__operator_ltoe::Error");
			exception.Raise();

			return TrentObject_Null;
		}

		return MAKE_TRENT_BOOLEAN((this->d_value <= reinterpret_cast<TrentInteger*>(obj)->d_value));
	}
	
	TrentObject* TrentInteger::__operator_gtoe(TrentObject* obj)
	{
		if (strcmp(obj->GetRuntimeName(), "Int") != 0)
		{
			std::string ex_message = std::string("Cannot use >= operator on types ") + GetRuntimeName() + std::string(" and ") + obj->GetRuntimeName();

			auto exception = TrentException("TrentInteger", ex_message, "__operator_gtoe::Error");
			exception.Raise();

			return TrentObject_Null;
		}

		return MAKE_TRENT_BOOLEAN((this->d_value >= reinterpret_cast<TrentInteger*>(obj)->d_value));
	}
	
	TrentObject* TrentInteger::__operator_equequ(TrentObject* obj)
	{
		if (strcmp(obj->GetRuntimeName(), "Int") != 0)
		{
			std::string ex_message = std::string("Cannot use == operator on types ") + GetRuntimeName() + std::string(" and ") + obj->GetRuntimeName();

			auto exception = TrentException("TrentInteger", ex_message, "__operator_equequ::Error");
			exception.Raise();

			return TrentObject_Null;
		}

		return MAKE_TRENT_BOOLEAN((this->d_value == reinterpret_cast<TrentInteger*>(obj)->d_value));
	}
	
	TrentObject* TrentInteger::__operator_notequ(TrentObject* obj)
	{
		if (strcmp(obj->GetRuntimeName(), "Int") != 0)
		{
			std::string ex_message = std::string("Cannot use != operator on types ") + GetRuntimeName() + std::string(" and ") + obj->GetRuntimeName();

			auto exception = TrentException("TrentInteger", ex_message, "__operator_notequ::Error");
			exception.Raise();

			return TrentObject_Null;
		}

		return MAKE_TRENT_BOOLEAN((this->d_value != reinterpret_cast<TrentInteger*>(obj)->d_value));
	}
}
