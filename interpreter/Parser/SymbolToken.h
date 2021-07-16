#pragma once
#include "Token.h"

namespace trent::parser
{
	enum class Symbol
	{
		Unknown = -1,

		BracketOpen,	 BracketClose,
		BraceOpen,		 BraceClose,
		ParenthesisOpen, ParenthesisClose,

		Comma, Period, Colon, Semicolon
	};

	class SymbolToken : public Token
	{
	public:
		SymbolToken(Symbol symbol = Symbol::Unknown, const std::string& value = "");
		std::string ToString() override;

		Symbol		d_symbol;
		std::string d_value;
	};
}
