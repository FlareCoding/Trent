#pragma once
#include "Token.h"

namespace trent::parser
{
	enum class Operator
	{
		Unknown = -1,
		Add,
		Sub,
		Mul,
		Div,
		Assignment,
		Equequ,
		Notequ,
		Not,
		Or,
		And,
		BitOr,
		BitAnd
	};

	class OperatorToken : public Token
	{
	public:
		OperatorToken(Operator op = Operator::Unknown, const std::string& value = "");
		std::string ToString() override;

		Operator	d_operator;
		std::string d_value;
	};
}
