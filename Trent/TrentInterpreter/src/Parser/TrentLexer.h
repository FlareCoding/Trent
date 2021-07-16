#pragma once
#include "TokenPool.h"

namespace trent::parser::lexer
{
	class TrentLexer
	{
	public:
		TrentLexer() = default;
		~TrentLexer() = default;

		std::shared_ptr<TokenPool> ConstructTokenPool(const std::string& source);

	private:
		void ParseLine(const std::string& line, size_t lineno);
		void ParseWord(const std::string& line, size_t lineno);

	private:
		std::shared_ptr<TokenPool> d_token_pool;

		std::unordered_map<std::string, Keyword> d_keywords = {
			{ "var",		Keyword::Var },
			{ "func",		Keyword::Func },
			{ "return",		Keyword::Return },
		};

		std::unordered_map<std::string, Operator> d_single_char_operators = {
			{ "=",		Operator::Assignment },
			{ "!",		Operator::Not },
			{ "+",		Operator::Add },
			{ "-",		Operator::Sub },
			{ "*",		Operator::Mul },
			{ "/",		Operator::Div },
		};

		std::unordered_map<std::string, Operator> d_double_char_operators = {
			{ "==",		Operator::Equequ },
			{ "!=",		Operator::Notequ },
			{ "&&",		Operator::And },
			{ "||",		Operator::Or },
		};

		std::unordered_map<std::string, Symbol> d_symbols = {
			{ "{",		Symbol::BraceOpen },
			{ "}",		Symbol::BraceClose },
			{ "[",		Symbol::BracketOpen },
			{ "]",		Symbol::BracketClose },
			{ "(",		Symbol::ParenthesisOpen },
			{ ")",		Symbol::ParenthesisClose },
			{ ",",		Symbol::Comma },
			{ ".",		Symbol::Period },
			{ ":",		Symbol::Colon },
			{ ";",		Symbol::Semicolon },
		};
	};
}
