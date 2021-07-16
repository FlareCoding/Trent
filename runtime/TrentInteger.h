#pragma once
#include "TrentObject.h"

namespace trent
{
	class TrentInteger : public TrentObject
	{
	public:
		TRAPI TrentInteger(int value = 0);
		TRAPI const char* ToString() override;
		TRAPI const char* GetRuntimeName() override;

		TRAPI inline int GetValue() const { return d_value; }

		TRAPI virtual TrentObject* __operator_add(TrentObject* obj) override;
		TRAPI virtual TrentObject* __operator_sub(TrentObject* obj) override;
		TRAPI virtual TrentObject* __operator_mul(TrentObject* obj) override;
		TRAPI virtual TrentObject* __operator_div(TrentObject* obj) override;

		// Less than boolean operator
		TRAPI virtual TrentObject* __operator_lt(TrentObject* obj) override;

		// Less than or equal to boolean operator
		TRAPI virtual TrentObject* __operator_gt(TrentObject* obj) override;

		// Greater than boolean operator
		TRAPI virtual TrentObject* __operator_ltoe(TrentObject* obj) override;

		// Greater than or equal to boolean operator
		TRAPI virtual TrentObject* __operator_gtoe(TrentObject* obj) override;

		// Equal to boolean operator
		TRAPI virtual TrentObject* __operator_equequ(TrentObject* obj) override;

		// Not equal to boolean operator
		TRAPI virtual TrentObject* __operator_notequ(TrentObject* obj) override;

	private:
		int d_value;
		std::string d_string_repr;
	};
}
