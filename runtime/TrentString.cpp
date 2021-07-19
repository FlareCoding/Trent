#include "TrentString.h"
#include "TrentInteger.h"
#include "TrentFloat.h"
#include "TrentBoolean.h"
#include "TrentRuntime.h"
#include <algorithm>

namespace trent
{
	TrentString::TrentString(const char* value)
		: d_buffer(value)
	{
		AddMemberFunction("length", TRENT_REGISTER_FUNCTION_LAMBDA(__Length));
		AddMemberFunction("reverse", TRENT_REGISTER_FUNCTION_LAMBDA(__Reverse));
		AddMemberFunction("append", TRENT_REGISTER_FUNCTION_LAMBDA(__Append));
	}

	const char* TrentString::ToString()
	{
		d_string_repr = d_buffer;
		return d_string_repr.c_str();
	}

	const char* TrentString::GetRuntimeName()
	{
		return "String";
	}

	TrentObject* TrentString::Copy(bool delegate_ownership_to_runtime)
	{
		return MAKE_TRENT_STRING_SPEC_OWNERSHIP(delegate_ownership_to_runtime, d_buffer.c_str());
	}

	void TrentString::CopyFrom(TrentObject* other)
	{
		if (strcmp(other->GetRuntimeName(), "String") != 0)
		{
			auto exception = TrentException("TrentString", "Cannot copy object buffer", "CopyObject::Error");
			exception.Raise();
			return;
		}

		this->d_buffer = reinterpret_cast<TrentString*>(other)->d_buffer;
	}

	TrentObject* TrentString::__operator_add(TrentObject* obj)
	{
		if (strcmp(obj->GetRuntimeName(), "String") == 0)
		{
			auto other = reinterpret_cast<TrentString*>(obj);
			std::string new_string = this->d_buffer + other->d_buffer;

			return MAKE_TRENT_STRING(new_string.c_str());
		}

		if (strcmp(obj->GetRuntimeName(), "Int") == 0)
		{
			auto other = reinterpret_cast<TrentInteger*>(obj);
			std::string new_string = this->d_buffer + std::to_string(other->GetValue());

			return MAKE_TRENT_STRING(new_string.c_str());
		}

		if (strcmp(obj->GetRuntimeName(), "Float") == 0)
		{
			auto other = reinterpret_cast<TrentFloat*>(obj);
			std::string new_string = this->d_buffer + std::to_string(other->GetValue());

			return MAKE_TRENT_STRING(new_string.c_str());
		}

		std::string ex_message = std::string("Cannot add objects of type ") + GetRuntimeName() + std::string(" and ") + obj->GetRuntimeName();

		auto exception = TrentException("TrentString", ex_message, "__operator_add::Error");
		exception.Raise();

		return TrentObject_Null;
	}

	TrentObject* TrentString::__operator_equequ(TrentObject* obj)
	{
		if (strcmp(obj->GetRuntimeName(), "String") != 0)
		{
			std::string ex_message = std::string("Cannot use == operator on types ") + GetRuntimeName() + std::string(" and ") + obj->GetRuntimeName();

			auto exception = TrentException("TrentString", ex_message, "__operator_equequ::Error");
			exception.Raise();

			return TrentObject_Null;
		}

		return MAKE_TRENT_BOOLEAN((this->d_buffer == reinterpret_cast<TrentString*>(obj)->d_buffer));
	}

	TrentObject* TrentString::__operator_notequ(TrentObject* obj)
	{
		if (strcmp(obj->GetRuntimeName(), "String") != 0)
		{
			std::string ex_message = std::string("Cannot use != operator on types ") + GetRuntimeName() + std::string(" and ") + obj->GetRuntimeName();

			auto exception = TrentException("TrentString", ex_message, "__operator_notequ::Error");
			exception.Raise();

			return TrentObject_Null;
		}

		return MAKE_TRENT_BOOLEAN((this->d_buffer != reinterpret_cast<TrentString*>(obj)->d_buffer));
	}

	TrentObject* TrentString::__Length(TrentObject* args)
	{
		return MAKE_TRENT_INT((int)this->d_buffer.size());
	}

	TrentObject* TrentString::__Reverse(TrentObject* args)
	{
		std::reverse(d_buffer.begin(), d_buffer.end());
		return TrentObject_Null;
	}

	TrentObject* TrentString::__Append(TrentObject* args)
	{
		char* str = nullptr;
		if (!TrentArg_Parse(args, "s", &str))
		{
			auto exception = TrentException("TrentString", "Failed to parse arguments", "__Append::Error");
			exception.Raise();

			return TrentObject_Null;
		}

		if (str != nullptr)
			d_buffer += str;

		return TrentObject_Null;
	}
}
