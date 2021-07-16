#pragma once
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace trent::parser
{
	enum class TokenType
	{
		Empty,
		Identifier,
		LiteralValue,
		Operator,
		Keyword,
		Symbol
	};

	class Token
	{
	public:
		size_t		d_lineno	= 1;
		TokenType	d_type		= TokenType::Empty;

		virtual std::string ToString();
	};

	template <typename T>
	using TokenRef = std::shared_ptr<T>;

	template <typename T, typename... Args>
	constexpr TokenRef<T> MakeToken(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	template <typename T, typename... Args>
	constexpr TokenRef<T> As(Args&&... args)
	{
		return std::reinterpret_pointer_cast<T>(std::forward<Args>(args)...);
	}
}
