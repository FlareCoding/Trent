#pragma once
#include "TrentObject.h"

namespace trent
{
	class TrentBoolean : public TrentObject
	{
	public:
		TRAPI TrentBoolean(bool value = false);
		TRAPI const char* ToString() override;
		TRAPI const char* GetRuntimeName() override;

		TRAPI inline bool GetValue() const { return d_value; }

		// Equal to boolean operator
		TRAPI virtual TrentObject* __operator_equequ(TrentObject* obj) override;

		// Not equal to boolean operator
		TRAPI virtual TrentObject* __operator_notequ(TrentObject* obj) override;

	private:
		bool d_value;
		std::string d_string_repr;
	};
}