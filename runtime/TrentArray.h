#pragma once
#include "TrentObject.h"
#include <initializer_list>

namespace trent
{
	class TrentArray : public TrentObject
	{
	public:
		TRAPI TrentArray();
		TRAPI ~TrentArray();
		TRAPI TrentArray(const std::initializer_list<TrentObject*>& items);
		TRAPI TrentArray(const std::vector<TrentObject*>& items);
		TRAPI const char* ToString() override;
		TRAPI const char* GetRuntimeName() override;

		TRAPI virtual TrentObject* Copy(bool delegate_ownership_to_runtime = true) override;
		TRAPI virtual void CopyFrom(TrentObject* other) override;

	private:
		std::vector<TrentObject*> d_items;
		std::string d_string_repr;

		void AddMemberFunctions();

	private:
		TrentObject* __Append(TrentObject* args);
		TrentObject* __Insert(TrentObject* args);
		TrentObject* __Remove(TrentObject* args);
		TrentObject* __Length(TrentObject* args);
		TrentObject* __GetItem(TrentObject* args);
	};
}
