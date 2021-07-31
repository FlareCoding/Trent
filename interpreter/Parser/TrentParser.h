#pragma once
#include "../AST/AST.h"
#include "TrentLexer.h"
#include <functional>

namespace trent::parser
{
	class TrentParser
	{
	public:
		// Epic default constructor that does nothing :D
		TrentParser() = default;

		// Initializes the patterns and other parser resources.
		void Initialize(std::string& source);

		// Constructs an Abstract Syntax Tree of nodes required by the interpreter.
		std::shared_ptr<AST> ConstructAST();

	private:
		// Populated the token pool with tokens.
		void InitializeTokenPool(std::string& source);

		// Advances the current token to the next one if the current token is a
		// SymbolToken and has the given symbol.
		// Raises an exception otherwise.
		void Expect(Symbol symbol);

		// Advances the current token to the next one if the current token is a
		// KeywordToken and has the given keyword.
		// Raises an exception otherwise.
		void Expect(Keyword keyword);

		// Advances the current token to the next one if the current token is a
		// OperatorToken and has the given operator.
		// Raises an exception otherwise.
		void Expect(Operator op);

		// Advances the current token to the next one if the current token
		// type matches the specified type.
		void Expect(TokenType type);

		// Checks if the token is a symbol token and
		// whether or not it contains the specified symbol.
		bool IsSymbol(TokenRef<Token> token, Symbol symbol);

		// Checks if the token is a keyword token and
		// whether or not it contains the specified keyword.
		bool IsKeyword(TokenRef<Token> token, Keyword keyword);

		// Checks if the token is an operator token and
		// whether or not it contains the specified operator.
		bool IsOperator(TokenRef<Token> token, Operator op);

		NodeRef<ASTNode>			ParseStatement();
		NodeRef<ASTExpressionNode>	ParseExpression(bool this_membership_flag = false);
		NodeRef<ASTNode>			ParseReturnStatement();
		NodeRef<ASTNode>			ParseBreakStatement();

		NodeRef<ASTNode>			ParseFunctionDeclaration();
		NodeRef<ASTNode>			ParseFunctionCall();

		NodeRef<ASTNode>			ParseVariableDeclaration();
		NodeRef<ASTNode>			ParseArrayDeclaration();
		NodeRef<ASTNode>			ParseVariableAssignment();
		NodeRef<ASTNode>			ParseVariableIncrement();
		NodeRef<ASTNode>			ParseVariableDecrement();
		NodeRef<ASTNode>			ParseVariableAdditionAssignment();
		NodeRef<ASTNode>			ParseVariableSubtractionAssignment();
		NodeRef<ASTNode>			ParseVariableMultiplicationAssignment();
		NodeRef<ASTNode>			ParseVariableDivisionAssignment();

		NodeRef<ASTNode>			ParseLiteralValue();
		NodeRef<ASTNode>			ParseIdentifier();
		NodeRef<ASTNode>			ParseKeyword();
		NodeRef<ASTNode>			ParseOperator(TokenRef<ASTExpressionNode> lhs);

		NodeRef<ASTNode>			ParseWhileLoop();
		NodeRef<ASTNode>			ParseForLoop();
		NodeRef<ASTNode>			ParseIfElseStatement();

		NodeRef<ASTNode>			ParseClassDeclaration();

	private:
		// Contains all the tokens
		std::shared_ptr<TokenPool>	d_token_pool;

		// Specifies the current token that is being processed
		TokenRef<Token>				d_current_token;

		// Casts the current token to the specified token sub-type
		template <typename T = Token>
		TokenRef<T> CurrentToken()
		{
			return As<T>(d_current_token);
		}

		// Casts the previous token to the specified token sub-type
		template <typename T = Token>
		TokenRef<T> PreviousToken()
		{
			return As<T>(d_token_pool->Previous(d_current_token));
		}

		// Casts the next token to the specified token sub-type
		template <typename T = Token>
		TokenRef<T> NextToken()
		{
			return As<T>(d_token_pool->Next(d_current_token));
		}
	};
}
