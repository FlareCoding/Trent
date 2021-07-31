#include "TrentArray.h"
#include "TrentInteger.h"
#include "TrentRuntime.h"

namespace trent
{
	TrentArray::TrentArray()
	{
		AddMemberFunctions();
	}

	TrentArray::TrentArray(const std::initializer_list<TrentObject*>& items)
	{
		for (auto& item : items)
			d_items.push_back(item->Copy(false));

		AddMemberFunctions();
	}

	TrentArray::TrentArray(const std::vector<TrentObject*>& items)
	{
		for (auto& item : items)
			d_items.push_back(item->Copy(false));

		AddMemberFunctions();
	}

	TrentArray::~TrentArray()
	{
		for (auto& item : d_items)
			TrentRuntime::FreeObject(item);
	}

	const char* TrentArray::ToString()
	{
		d_string_repr = "[";
		for (size_t i = 0; i < d_items.size(); i++)
		{
			if (strcmp(d_items[i]->GetRuntimeName(), "String") == 0)
				d_string_repr += "'" + std::string(d_items[i]->ToString()) + "'";
			else
				d_string_repr += std::string(d_items[i]->ToString());

			if (i < d_items.size() - 1)
				d_string_repr += ", ";
		}
		d_string_repr += "]";

		return d_string_repr.c_str();
	}

	const char* TrentArray::GetRuntimeName()
	{
		return "Array";
	}

	TrentObject* TrentArray::Copy(bool delegate_ownership_to_runtime)
	{
		return MAKE_TRENT_ARRAY_SPEC_OWNERSHIP(delegate_ownership_to_runtime, d_items);
	}

	void TrentArray::CopyFrom(TrentObject* other)
	{
		if (strcmp(other->GetRuntimeName(), "Array") != 0)
		{
			auto exception = TrentException("TrentArray", "Cannot copy object buffer", "CopyObject::Error");
			exception.Raise();
			return;
		}

		// Deallocate existing items
		for (auto& item : d_items)
			TrentRuntime::FreeObject(item);

		d_items.clear();

		for (auto& item : reinterpret_cast<TrentArray*>(other)->d_items)
			d_items.push_back(item->Copy(false));
	}

	void TrentArray::AddMemberFunctions()
	{
		AddMemberFunction("append", TRENT_REGISTER_FUNCTION_LAMBDA(__Append));
		AddMemberFunction("insert", TRENT_REGISTER_FUNCTION_LAMBDA(__Insert));
		AddMemberFunction("remove", TRENT_REGISTER_FUNCTION_LAMBDA(__Remove));
		AddMemberFunction("length", TRENT_REGISTER_FUNCTION_LAMBDA(__Length));
		AddMemberFunction("get",	TRENT_REGISTER_FUNCTION_LAMBDA(__GetItem));
	}

	TrentObject* TrentArray::__Append(TrentObject* args)
	{
		TrentObject* obj = nullptr;
		if (!TrentArg_Parse(args, "o", &obj))
		{
			auto exception = TrentException("TrentArray", "Failed to parse arguments", "__Append::Error");
			exception.Raise();

			return TrentObject_Null;
		}

		if (!obj)
		{
			auto exception = TrentException("TrentArray", "Argument is nullptr", "__Append::Error");
			exception.Raise();
			return TrentObject_Null;
		}

		d_items.push_back(obj->Copy(false));
		return TrentObject_Null;
	}

	TrentObject* TrentArray::__Insert(TrentObject* args)
	{
		int index = -1;
		TrentObject* obj = nullptr;
		if (!TrentArg_Parse(args, "io", &index, &obj))
		{
			auto exception = TrentException("TrentArray", "Failed to parse arguments", "__Insert::Error");
			exception.Raise();

			return TrentObject_Null;
		}

		// Sanity check the index
		if (index < 0 || index >= d_items.size())
		{
			auto exception = TrentException("TrentArray", "Invalid index (" + std::to_string(index) + ")", "ArrayIndexOutOfBounds");
			exception.Raise();
			return TrentObject_Null;
		}

		// Sanity check the object
		if (!obj)
		{
			auto exception = TrentException("TrentArray", "Argument is nullptr", "__Insert::Error");
			exception.Raise();
			return TrentObject_Null;
		}

		d_items.insert(d_items.begin() + index, obj->Copy(false));
		return TrentObject_Null;
	}

	TrentObject* TrentArray::__Remove(TrentObject* args)
	{
		int index = -1;
		if (!TrentArg_Parse(args, "i", &index))
		{
			auto exception = TrentException("TrentArray", "Failed to parse arguments", "__Remove::Error");
			exception.Raise();

			return TrentObject_Null;
		}

		// Sanity check the index
		if (index < 0 || index >= d_items.size())
		{
			auto exception = TrentException("TrentArray", "Invalid index (" + std::to_string(index) + ")", "ArrayIndexOutOfBounds");
			exception.Raise();
			return TrentObject_Null;
		}

		d_items.erase(d_items.begin() + index);
		return TrentObject_Null;
	}

	TrentObject* TrentArray::__Length(TrentObject* args)
	{
		return MAKE_TRENT_INT((int)this->d_items.size());
	}

	TrentObject* TrentArray::__GetItem(TrentObject* args)
	{
		int index = -1;
		if (!TrentArg_Parse(args, "i", &index))
		{
			auto exception = TrentException("TrentArray", "Failed to parse arguments", "__GetItem::Error");
			exception.Raise();

			return TrentObject_Null;
		}

		// Sanity check the index
		if (index < 0 || index >= d_items.size())
		{
			auto exception = TrentException("TrentArray", "Invalid index (" + std::to_string(index) + ")", "ArrayIndexOutOfBounds");
			exception.Raise();
			return TrentObject_Null;
		}

		return d_items[index];
	}
}
