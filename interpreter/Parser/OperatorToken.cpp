#include "OperatorToken.h"

namespace trent::parser
{
	OperatorToken::OperatorToken(Operator op, const std::string& value)
		: d_operator(op), d_value(value)
	{
		d_type = TokenType::Operator;
	}

	std::string OperatorToken::ToString()
	{
		return "[TokenType: Operator, Value: '" + d_value + "']";
	}
}
