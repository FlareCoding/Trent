#include "TokenPool.h"

namespace trent::parser
{
	TokenRef<Token> TokenPool::Next(TokenRef<Token> token)
	{
		if (token == nullptr && d_tokens.size())
			return d_tokens[0];

		auto idx = IndexOf(token);
		if (idx == -1 || idx == d_tokens.size() - 1)
			return nullptr;

		return d_tokens[idx + 1];
	}
	
	TokenRef<Token> TokenPool::Previous(TokenRef<Token> token)
	{
		auto idx = IndexOf(token);
		if (idx == -1 || idx == 0)
			return nullptr;

		return d_tokens[idx - 1];
	}
	
	TokenRef<Token> TokenPool::Consume(TokenRef<Token> token)
	{
		auto next_token = Next(token);

		auto idx = IndexOf(token);
		if (idx == -1)
			return nullptr;

		d_tokens.erase(d_tokens.begin() + idx);
		return next_token;
	}
	
	size_t TokenPool::IndexOf(TokenRef<Token> token)
	{
		for (size_t i = 0; i < d_tokens.size(); i++)
		{
			if (d_tokens[i] == token)
				return i;
		}

		return -1;
	}
}
