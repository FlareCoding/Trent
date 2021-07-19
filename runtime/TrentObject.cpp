#include "TrentObject.h"
#include "TrentTuple.h"
#include "TrentInteger.h"
#include "TrentString.h"
#include "TrentBoolean.h"
#include "TrentRuntime.h"
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

	TrentObject* TrentObject::Copy(bool delegate_ownership_to_runtime)
	{
		auto obj = TrentRuntime::AllocateObject<TrentObject>(delegate_ownership_to_runtime);
		if (d_description == "null")
			obj->d_description = "null";

		return obj;
	}

	void TrentObject::CopyFrom(TrentObject* other) {}

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

	TrentObject::getter_fn_t TrentObject::GetPropertyGetter(const char* fn_name)
	{
		if (d_property_getters.find(fn_name) != d_property_getters.end())
			return d_property_getters[fn_name];
		else
			return nullptr;
	}

	TrentObject::setter_fn_t TrentObject::GetPropertySetter(const char* fn_name)
	{
		if (d_property_setters.find(fn_name) != d_property_setters.end())
			return d_property_setters[fn_name];
		else
			return nullptr;
	}

	TrentObject::member_fn_t TrentObject::GetMemberFunction(const char* fn_name)
	{
		if (d_member_functions.find(fn_name) != d_member_functions.end())
			return d_member_functions[fn_name];
		else
			return nullptr;
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

	TrentObject* TrentObject::__operator_lt(TrentObject* obj)
	{
		std::string ex_message = std::string("Cannot use < operator on types ") + GetRuntimeName() + std::string(" and ") + obj->GetRuntimeName();

		auto exception = TrentException("TrentObject", ex_message, "__operator_lt::Error");
		exception.Raise();

		return TrentObject_Null;
	}

	TrentObject* TrentObject::__operator_gt(TrentObject* obj)
	{
		std::string ex_message = std::string("Cannot use > operator on types ") + GetRuntimeName() + std::string(" and ") + obj->GetRuntimeName();

		auto exception = TrentException("TrentObject", ex_message, "__operator_gt::Error");
		exception.Raise();

		return TrentObject_Null;
	}

	TrentObject* TrentObject::__operator_ltoe(TrentObject* obj)
	{
		std::string ex_message = std::string("Cannot use <= operator on types ") + GetRuntimeName() + std::string(" and ") + obj->GetRuntimeName();

		auto exception = TrentException("TrentObject", ex_message, "__operator_ltoe::Error");
		exception.Raise();

		return TrentObject_Null;
	}

	TrentObject* TrentObject::__operator_gtoe(TrentObject* obj)
	{
		std::string ex_message = std::string("Cannot use >= operator on types ") + GetRuntimeName() + std::string(" and ") + obj->GetRuntimeName();

		auto exception = TrentException("TrentObject", ex_message, "__operator_gtoe::Error");
		exception.Raise();

		return TrentObject_Null;
	}

	TrentObject* TrentObject::__operator_equequ(TrentObject* obj)
	{
		std::string ex_message = std::string("Cannot use == operator on types ") + GetRuntimeName() + std::string(" and ") + obj->GetRuntimeName();

		auto exception = TrentException("TrentObject", ex_message, "__operator_equequ::Error");
		exception.Raise();

		return TrentObject_Null;
	}

	TrentObject* TrentObject::__operator_notequ(TrentObject* obj)
	{
		std::string ex_message = std::string("Cannot use != operator on types ") + GetRuntimeName() + std::string(" and ") + obj->GetRuntimeName();

		auto exception = TrentException("TrentObject", ex_message, "__operator_notequ::Error");
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
			case 'b': {
				bool* p_arg = va_arg(args, bool*);
				TrentBoolean* t_arg = reinterpret_cast<TrentBoolean*>(args_tuple->__NativeGetItem(i));
				if (strcmp(t_arg->GetRuntimeName(), "Boolean") != 0)
				{
					std::string ex_message = std::string("Argument ") + std::to_string(i + 1) + " is not a TrentBoolean";
					auto exception = TrentException("TrentArg_Parse", ex_message, "ArgumentTypeError");
					exception.Raise();
					return false;
				}

				*p_arg = t_arg->GetValue();
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
