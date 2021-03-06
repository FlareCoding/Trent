#include "TrentTuple.h"
#include "TrentRuntime.h"

namespace trent
{
	TrentTuple::TrentTuple(const std::initializer_list<TrentObject*>& contents)
		: d_contents(contents)
	{
		AddMemberFunction("get", TRENT_REGISTER_FUNCTION_LAMBDA(__GetItem));
		AddMemberFunction("set", TRENT_REGISTER_FUNCTION_LAMBDA(__SetItem));
	}

	TrentTuple::TrentTuple(const std::vector<TrentObject*>& contents)
		: d_contents(contents)
	{
		AddMemberFunction("get", TRENT_REGISTER_FUNCTION_LAMBDA(__GetItem));
		AddMemberFunction("set", TRENT_REGISTER_FUNCTION_LAMBDA(__SetItem));
	}

	const char* TrentTuple::ToString()
	{
		d_string_repr = "Tuple(";
		for (size_t i = 0; i < d_contents.size(); i++)
		{
			d_string_repr += std::string(d_contents[i]->ToString());
			if (i < d_contents.size() - 1)
				d_string_repr += ", ";
		}
		d_string_repr += ")";

		return d_string_repr.c_str();
	}

	const char* TrentTuple::GetRuntimeName()
	{
		return "Tuple";
	}

	TrentObject* TrentTuple::Copy(bool delegate_ownership_to_runtime)
	{
		std::vector<TrentObject*> contents;

		for (auto& item : d_contents)
			contents.push_back(item->Copy());

		return MAKE_TRENT_TUPLE_VEC_SPEC_OWNERSHIP(delegate_ownership_to_runtime, contents);
	}

	void TrentTuple::CopyFrom(TrentObject* other)
	{
		if (strcmp(other->GetRuntimeName(), "Tuple") != 0)
		{
			auto exception = TrentException("TrentTuple", "Cannot copy object buffer", "CopyObject::Error");
			exception.Raise();
			return;
		}

		this->d_contents = reinterpret_cast<TrentTuple*>(other)->d_contents;
	}

	TrentObject* TrentTuple::__GetItem(TrentObject* args)
	{
		int idx;
		if (!TrentArg_Parse(args, "i", &idx))
		{
			auto exception = TrentException("TrentTuple", "Failed to parse arguments", "__GetItem::Error");
			exception.Raise();
			return TrentObject_Null;
		}

		if (idx < 0 || idx >= (int)d_contents.size())
		{
			auto exception = TrentException("TrentTuple", "Invalid item index provided", "IndexOutOfBoundsError");
			exception.Raise();
			return TrentObject_Null;
		}

		return d_contents[idx];
	}

	TrentObject* TrentTuple::__SetItem(TrentObject* args)
	{
		int idx;
		TrentObject* item;
		if (!TrentArg_Parse(args, "io", &idx, &item))
		{
			auto exception = TrentException("TrentTuple", "Failed to parse arguments", "__GetItem::Error");
			exception.Raise();
			return TrentObject_Null;
		}

		if (idx < 0 || idx >= (int)d_contents.size())
		{
			auto exception = TrentException("TrentTuple", "Invalid item index provided", "IndexOutOfBoundsError");
			exception.Raise();
			return TrentObject_Null;
		}

		d_contents[idx] = item;
		return TrentObject_Null;
	}
}
