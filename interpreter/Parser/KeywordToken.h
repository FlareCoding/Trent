#pragma once
#include "Token.h"

namespace trent::parser
{
	enum class Keyword
	{
		Unknown = -1,
		Var,
		Func,
		Return,
		While,
		For,
		Break,
		If,
		Elif,
		Else,
		Class,
		This,
		New
	};

	class KeywordToken : public Token
	{
	public:
		KeywordToken(Keyword kwd = Keyword::Unknown, const std::string& value = "");
		std::string ToString() override;

		Keyword		d_keyword;
		std::string d_value;
	};
}
