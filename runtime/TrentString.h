#pragma once
#include "TrentObject.h"

namespace trent
{
	class TrentString : public TrentObject
	{
	public:
		TRAPI TrentString(const char* value = "");
		TRAPI const char* ToString() override;
		TRAPI const char* GetRuntimeName() override;

		TRAPI virtual TrentObject* Copy(bool delegate_ownership_to_runtime = true) override;
		TRAPI virtual void CopyFrom(TrentObject* other) override;

		TRAPI inline std::string& GetBuffer() { return d_buffer; }

		// Addition binary operator
		TRAPI virtual TrentObject* __operator_add(TrentObject* obj) override;

		// Equal to boolean operator
		TRAPI virtual TrentObject* __operator_equequ(TrentObject* obj) override;

		// Not equal to boolean operator
		TRAPI virtual TrentObject* __operator_notequ(TrentObject* obj) override;

	private:
		std::string d_buffer;
		std::string d_string_repr;

	private:
		TrentObject* __Length(TrentObject* args);
		TrentObject* __Reverse(TrentObject* args);
		TrentObject* __Append(TrentObject* args);
	};
}
