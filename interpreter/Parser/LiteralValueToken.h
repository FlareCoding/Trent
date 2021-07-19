#pragma once
#include "Token.h"
#include "AST/ASTLiteralValueNode.h"

namespace trent::parser
{
	class LiteralValueToken : public Token
	{
	public:
		LiteralValueToken(LiteralType value_type = LiteralType::Null, const std::string& value = "");
		std::string ToString() override;

		LiteralType d_value_type;
		std::string d_value;

	private:
		std::unordered_map<LiteralType, std::string> d_value_string_map = {
			{ LiteralType::Null,	"Null" },
			{ LiteralType::Integer, "Integer" },
			{ LiteralType::String,	"String" },
			{ LiteralType::Float,	"Float" },
		};
	};
}
