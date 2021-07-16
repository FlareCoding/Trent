#pragma once
#include "SymbolToken.h"
#include "IdentifierToken.h"
#include "LiteralValueToken.h"
#include "KeywordToken.h"
#include "OperatorToken.h"

namespace trent::parser
{
	class TokenPool
	{
	public:
		TokenPool() = default;
		~TokenPool() = default;

		void Add(TokenRef<Token> token) { d_tokens.push_back(token); }

		TokenRef<Token> Next(TokenRef<Token> token);
		TokenRef<Token> Previous(TokenRef<Token> token);

		/// Returns the next token in the list.
		TokenRef<Token>	Consume(TokenRef<Token> token);

		std::vector<TokenRef<Token>>& __GetAllTokens() { return d_tokens; }

	private:
		std::vector<TokenRef<Token>> d_tokens;

		size_t IndexOf(TokenRef<Token> token);
	};
}
