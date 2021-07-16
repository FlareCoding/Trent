#include "TrentObject.h"
#include "TrentTuple.h"
#include "TrentInteger.h"
#include "TrentString.h"
#include <sstream> 
#include <string>

namespace trent
{
	TrentObject* TrentObject_Null = nullptr;

	void TrentObject::__Init()
	{
		const void* this_address = static_cast<const void*>(this);
		std::stringstream ss;
		ss << std::hex << this_address;

		this->d_description = std::string("<") + this->d_type.name + std::string(" object at 0x") + ss.str() + ">";
		this->d_instance_description = this->d_description;
	}

	const char* TrentObject::ToString()
	{
		return d_description.c_str();
	}

	void TrentObject::AddGetter(const char* property, getter_fn_t fn)
	{
		d_property_getters[property] = fn;
	}

	const char* TrentObject::GetRuntimeName()
	{
		return "Object";
	}

	const char* TrentObject::GetInstanceDescription()
	{
		return this->d_instance_description.c_str();
	}

	void TrentObject::AddSetter(const char* property, setter_fn_t fn)
	{
		d_property_setters[property] = fn;
	}

	TrentObject* TrentObject::Call(const char* fn_name, TrentObject* args)
	{
		if (d_member_functions.find(fn_name) == d_member_functions.end())
		{
			printf("TrentObject::Call::Error: Member function '%s' doesn't exist for object of type '%s'\n", fn_name, this->d_type.name);
			return TrentObject_Null;
		}

		member_fn_t func = d_member_functions[fn_name];
		if (!func)
		{
			printf("TrentObject::Call::Error: Native function is nullptr\n");
			return TrentObject_Null;
		}

		return func(args);
	}

	void TrentObject::AddMemberFunction(const char* fn_name, member_fn_t fn)
	{
		d_member_functions[fn_name] = fn;
	}

	TrentObject* TrentObject::__operator_add(TrentObject* obj)
	{
		std::string ex_message = std::string("Cannot use + operator on types ") + GetRuntimeName() + std::string(" and ") + obj->GetRuntimeName();
		
		auto exception = TrentException("TrentObject", ex_message, "__operator_add::Error");
		exception.Raise();

		return TrentObject_Null;
	}

	TrentObject* TrentObject::__operator_sub(TrentObject* obj)
	{
		std::string ex_message = std::string("Cannot use - operator on types ") + GetRuntimeName() + std::string(" and ") + obj->GetRuntimeName();

		auto exception = TrentException("TrentObject", ex_message, "__operator_sub::Error");
		exception.Raise();

		return TrentObject_Null;
	}

	TrentObject* TrentObject::__operator_mul(TrentObject* obj)
	{
		std::string ex_message = std::string("Cannot use * operator on types ") + GetRuntimeName() + std::string(" and ") + obj->GetRuntimeName();

		auto exception = TrentException("TrentObject", ex_message, "__operator_mul::Error");
		exception.Raise();

		return TrentObject_Null;
	}

	TrentObject* TrentObject::__operator_div(TrentObject* obj)
	{
		std::string ex_message = std::string("Cannot use / operator on types ") + GetRuntimeName() + std::string(" and ") + obj->GetRuntimeName();

		auto exception = TrentException("TrentObject", ex_message, "__operator_div::Error");
		exception.Raise();

		return TrentObject_Null;
	}

	std::ostream& operator<<(std::ostream& os, TrentObject& obj)
	{
		os << obj.ToString();
		return os;
	}

	bool TrentArg_Parse(TrentObject* obj, const char* fmt, ...)
	{
		if (strcmp(obj->GetRuntimeName(), "Tuple") != 0)
		{
			auto exception = TrentException("TrentArg_Parse", "Argument not a tuple", "ArgumentTypeError");
			exception.Raise();
			return false;
		}

		TrentTuple* args_tuple = dynamic_cast<TrentTuple*>(obj);

		size_t fmt_arg_count = std::min(strlen(fmt), args_tuple->__NativeGetItemCount());

		va_list args;
		va_start(args, fmt);

		for (size_t i = 0; i < fmt_arg_count; i++)
		{
			char arg_type = fmt[i];
			switch (arg_type)
			{
			case 'i': {
				int* p_arg = va_arg(args, int*);
				TrentInteger* t_arg = reinterpret_cast<TrentInteger*>(args_tuple->__NativeGetItem(i));
				if (strcmp(t_arg->GetRuntimeName(), "Int") != 0)
				{
					std::string ex_message = std::string("Argument ") + std::to_string(i + 1) + " is not a TrentInteger";
					auto exception = TrentException("TrentArg_Parse", ex_message, "ArgumentTypeError");
					exception.Raise();
					return false;
				}

				*p_arg = t_arg->GetValue();
				break;
			}
			case 's': {
				char** p_arg = va_arg(args, char**);
				TrentString* t_arg = reinterpret_cast<TrentString*>(args_tuple->__NativeGetItem(i));
				if (strcmp(t_arg->GetRuntimeName(), "String") != 0)
				{
					std::string ex_message = std::string("Argument ") + std::to_string(i + 1) + " is not a TrentString";
					auto exception = TrentException("TrentArg_Parse", ex_message, "ArgumentTypeError");
					exception.Raise();
					return false;
				}
				
				*p_arg = (char*)t_arg->GetBuffer().c_str();
				break;
			}
			case 'o': {
				TrentObject** p_arg = va_arg(args, TrentObject**);
				TrentObject* t_arg = args_tuple->__NativeGetItem(i);
				*p_arg = t_arg;
				break;
			}
			default: {
				int _ = va_arg(args, int);
				printf("TrentArg_Parse::Warn: Unknown format argument '%c'\n", fmt[i]);
				break;
			}
			}
		}

		va_end(args);
		return true;
	}
}
