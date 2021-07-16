#pragma once
#include "Token.h"
#include "AST/ASTExpressionNode.h"

namespace trent::parser
{
	class OperatorToken : public Token
	{
	public:
		OperatorToken(Operator op = Operator::Unknown, const std::string& value = "");
		std::string ToString() override;

		Operator	d_operator;
		std::string d_value;
	};
}
