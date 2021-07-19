#include "TrentBoolean.h"
#include "TrentRuntime.h"

namespace trent
{
	TrentBoolean::TrentBoolean(bool value)
		: d_value(value)
	{
	}

	const char* TrentBoolean::ToString()
	{
		d_string_repr = d_value ? "true" : "false";
		return d_string_repr.c_str();
	}

	const char* TrentBoolean::GetRuntimeName()
	{
		return "Boolean";
	}
	
	TrentObject* TrentBoolean::Copy(bool delegate_ownership_to_runtime)
	{
		return MAKE_TRENT_BOOLEAN_SPEC_OWNERSHIP(delegate_ownership_to_runtime, d_value);
	}

	void TrentBoolean::CopyFrom(TrentObject* other)
	{
		if (strcmp(other->GetRuntimeName(), "Boolean") != 0)
		{
			auto exception = TrentException("TrentBoolean", "Cannot copy object buffer", "CopyObject::Error");
			exception.Raise();
			return;
		}

		this->d_value = reinterpret_cast<TrentBoolean*>(other)->d_value;
	}

	TrentObject* TrentBoolean::__operator_equequ(TrentObject* obj)
	{
		if (strcmp(obj->GetRuntimeName(), "Boolean") != 0)
		{
			std::string ex_message = std::string("Cannot use == operator on types ") + GetRuntimeName() + std::string(" and ") + obj->GetRuntimeName();

			auto exception = TrentException("TrentBoolean", ex_message, "__operator_equequ::Error");
			exception.Raise();

			return TrentObject_Null;
		}

		return MAKE_TRENT_BOOLEAN((this->d_value == reinterpret_cast<TrentBoolean*>(obj)->d_value));
	}
	
	TrentObject* TrentBoolean::__operator_notequ(TrentObject* obj)
	{
		if (strcmp(obj->GetRuntimeName(), "Boolean") != 0)
		{
			std::string ex_message = std::string("Cannot use != operator on types ") + GetRuntimeName() + std::string(" and ") + obj->GetRuntimeName();

			auto exception = TrentException("TrentBoolean", ex_message, "__operator_notequ::Error");
			exception.Raise();

			return TrentObject_Null;
		}

		return MAKE_TRENT_BOOLEAN((this->d_value != reinterpret_cast<TrentBoolean*>(obj)->d_value));
	}
}
