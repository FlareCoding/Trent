#include "TrentString.h"
#include "TrentInteger.h"
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
		d_string_repr = std::string("'") + d_buffer + "'";
		return d_string_repr.c_str();
	}

	const char* TrentString::GetRuntimeName()
	{
		return "String";
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

		std::string ex_message = std::string("Cannot add objects of type ") + GetRuntimeName() + std::string(" and ") + obj->GetRuntimeName();

		auto exception = TrentException("TrentString", ex_message, "__operator_add::Error");
		exception.Raise();

		return TrentObject_Null;
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
		char* str;
		if (!TrentArg_Parse(args, "s", &str))
		{
			auto exception = TrentException("TrentString", "Failed to parse arguments", "__Append::Error");
			exception.Raise();

			return TrentObject_Null;
		}

#pragma warning(push)
#pragma warning(disable: 6054)
		d_buffer += str;
#pragma warning(pop) 
		return TrentObject_Null;
	}
}
