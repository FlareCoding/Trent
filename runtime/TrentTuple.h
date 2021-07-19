#pragma once
#include "TrentObject.h"
#include <initializer_list>

namespace trent
{
	class TrentTuple : public TrentObject
	{
	public:
		TRAPI TrentTuple(const std::initializer_list<TrentObject*>& contents);
		TRAPI TrentTuple(const std::vector<TrentObject*>& contents);
		TRAPI const char* ToString() override;
		TRAPI const char* GetRuntimeName() override;

		TRAPI virtual TrentObject* Copy(bool delegate_ownership_to_runtime = true) override;
		TRAPI virtual void CopyFrom(TrentObject* other) override;

		TRAPI TrentObject* __NativeGetItem(size_t idx) const { return d_contents[idx]; }
		TRAPI size_t __NativeGetItemCount() const { return d_contents.size(); }

	private:
		std::vector<TrentObject*> d_contents;
		std::string d_string_repr;

	private:
		TrentObject* __GetItem(TrentObject* args);
		TrentObject* __SetItem(TrentObject* args);
	};
}
