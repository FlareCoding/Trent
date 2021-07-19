#include "TrentLexer.h"
#include <sstream>
#include <algorithm>
#include <regex>
#include <iomanip>

namespace trent::parser::lexer
{
	void RemoveMultilineComments(std::string& str)
	{
		auto start_idx = str.find("/*");
		auto end_idx = str.find("*/");

		while (start_idx != std::string::npos &&
			end_idx != std::string::npos)
		{
			str.erase(start_idx, end_idx - start_idx + 2);

			start_idx = str.find("/*");
			end_idx = str.find("*/");
		}
	}

	std::vector<std::string> RemoveNewLines(const std::string& str)
	{
		std::vector<std::string> strings;
		
		std::regex rgx("\n");
		std::sregex_token_iterator iter(str.begin(), str.end(), rgx, -1);
		std::sregex_token_iterator end;
		for (; iter != end; ++iter)
			strings.push_back(*iter);

		return strings;
	}

	std::string& LTrim(std::string& s, const char* t = " \t\n\r\f\v")
	{
		s.erase(0, s.find_first_not_of(t));
		return s;
	}

	std::string& RTrim(std::string& s, const char* t = " \t\n\r\f\v")
	{
		s.erase(s.find_last_not_of(t) + 1);
		return s;
	}

	std::string& Trim(std::string& s, const char* t = " \t\n\r\f\v")
	{
		return LTrim(RTrim(s, t), t);
	}

	std::string Unescape(const std::string& str)
	{
		std::string res;
		std::string::const_iterator it = str.begin();
		while (it != str.end())
		{
			char c = *it++;
			if (c == '\\' && it != str.end())
			{
				switch (*it++) {
				case '\\': c = '\\'; break;
				case 'n': c = '\n'; break;
				case 't': c = '\t'; break;
				case 'r': c = '\r'; break;
				case 'a': c = '\a'; break;
				case 'b': c = '\b'; break;
				case 'v': c = '\v'; break;
				case '"': c = '\f'; break;
					// The \" is being converted to form feed escape sequence
					// to later be added to the string as a valid quote.
				default:
					continue;
				}
			}
			res += c;
		}

		return res;
	}

	bool IsInteger(const std::string& str)
	{
		return std::all_of(str.begin(), str.end(), ::isdigit);
	}

	bool IsFloat(const std::string& str)
	{
		std::istringstream iss(str);
		float f;
		iss >> std::noskipws >> f;
		return iss.eof() && !iss.fail();
	}

	bool IsBoolean(const std::string& str)
	{
		return (str == "true") || (str == "false");
	}

	template <typename T>
	bool IsInMap(const std::string& str, std::unordered_map<std::string, T>& container)
	{
		return (container.find(str) != container.end());
	}

	std::shared_ptr<TokenPool> TrentLexer::ConstructTokenPool(std::string& source)
	{
		d_token_pool = std::make_shared<TokenPool>();
		
		// Pre-filtering and removing multiline comments
		RemoveMultilineComments(source);

		auto lines = RemoveNewLines(source);
		size_t lineno = 1;
		for (auto& line : lines)
		{
			// Parse the line if it's not empty
			if (line.find_first_not_of(' ') != std::string::npos)
				ParseLine(line, lineno);

			lineno++;
		}

		return d_token_pool;
	}

	void TrentLexer::ParseLine(const std::string& line, size_t lineno)
	{
		std::string line_copy = line;
		Trim(line_copy);
		if (line_copy.find("//") == 0) // Line is a comment
			return;

		auto unescaped_line = Unescape(line);

		unsigned counter = 0;
		std::string segment;
		std::stringstream stream_input(unescaped_line);
		while (std::getline(stream_input, segment, '\"'))
		{
			++counter;
			if (counter % 2 == 0)
			{
				if (!segment.empty())
				{
					// Replacing the previously fixed form-feed escape sequences
					// with quotes.
					std::replace(segment.begin(), segment.end(), '\f', '\"');

					auto token = MakeToken<LiteralValueToken>(LiteralType::String, segment);
					token->d_lineno = lineno;
					d_token_pool->Add(token);
				}
			}
			else
			{
				std::stringstream stream_segment(segment);
				while (std::getline(stream_segment, segment, ' '))
				{
					if (!segment.empty())
					{
						// Removing the leading and trailing whitespace
						Trim(segment);
						ParseWord(segment, lineno);
					}
				}
			}
		}
	}

	void TrentLexer::ParseWord(const std::string& line, size_t lineno)
	{
		std::regex rx(R"(>=|<=|/=|\*=|\-=|\-\-|\+=|\+\+|!=|==|&&|\|\||[!();:=,{}\[\]+*/\-])");
		std::sregex_token_iterator srti(line.begin(), line.end(), rx, { -1, 0 });
		std::vector<std::string> tokens;
		std::remove_copy_if(srti, std::sregex_token_iterator(),
			std::back_inserter(tokens),
			[](std::string const& s) { return s.empty(); });

		for (auto& token_str : tokens)
		{
			// Checking if value is null
			if (token_str == "null")
			{
				auto token = MakeToken<LiteralValueToken>(LiteralType::Null, token_str);
				token->d_lineno = lineno;
				d_token_pool->Add(token);
				continue;
			}

			// Checking if token is an integer
			if (IsInteger(token_str))
			{
				auto token = MakeToken<LiteralValueToken>(LiteralType::Integer, token_str);
				token->d_lineno = lineno;
				d_token_pool->Add(token);
				continue;
			}

			// Checking if token is a float
			if (IsFloat(token_str))
			{
				auto token = MakeToken<LiteralValueToken>(LiteralType::Float, token_str);
				token->d_lineno = lineno;
				d_token_pool->Add(token);
				continue;
			}

			// Checking if token is a boolean
			if (IsBoolean(token_str))
			{
				auto token = MakeToken<LiteralValueToken>(LiteralType::Boolean, token_str);
				token->d_lineno = lineno;
				d_token_pool->Add(token);
				continue;
			}
			
			// Checking if token is a double character operator
			if (IsInMap<Operator>(token_str, d_double_char_operators))
			{
				auto token = MakeToken<OperatorToken>(d_double_char_operators[token_str], token_str);
				token->d_lineno = lineno;
				d_token_pool->Add(token);
				continue;
			}

			// Checking if token is a single character operator
			if (IsInMap<Operator>(token_str, d_single_char_operators))
			{
				auto token = MakeToken<OperatorToken>(d_single_char_operators[token_str], token_str);
				token->d_lineno = lineno;
				d_token_pool->Add(token);
				continue;
			}

			// Checking if token is a symbol
			if (IsInMap<Symbol>(token_str, d_symbols))
			{
				auto token = MakeToken<SymbolToken>(d_symbols[token_str], token_str);
				token->d_lineno = lineno;
				d_token_pool->Add(token);
				continue;
			}

			// Checking if token is a keyword
			if (IsInMap<Keyword>(token_str, d_keywords))
			{
				auto token = MakeToken<KeywordToken>(d_keywords[token_str], token_str);
				token->d_lineno = lineno;
				d_token_pool->Add(token);
				continue;
			}

			// If no above condition is met, then
			// the token will be considered an identifier.
			auto token = MakeToken<IdentifierToken>(token_str);
			token->d_lineno = lineno;
			d_token_pool->Add(token);
		}
	}
}
