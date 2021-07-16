#include "KeywordToken.h"

namespace trent::parser
{
	KeywordToken::KeywordToken(Keyword kwd, const std::string& value)
		: d_keyword(kwd), d_value(value)
	{
		d_type = TokenType::Keyword;
	}

	std::string KeywordToken::ToString()
	{
		return "[TokenType: Keyword, Value: '" + d_value + "']";
	}
}
