#pragma once
#include "Token.h"

namespace trent::parser
{
	class IdentifierToken : public Token
	{
	public:
		IdentifierToken(const std::string& value = "");
		std::string ToString() override;

		std::string d_value;
	};
}
