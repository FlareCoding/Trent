#include "TrentParser.h"
#include "Trent.h"

namespace trent::parser
{
	static bool IsBinaryOperator(Operator op)
	{
		switch (op)
		{
		case Operator::Add:
		case Operator::Sub:
		case Operator::Mul:
		case Operator::Div:
			return true;
		default:
			return false;
		}
	}

	static bool IsBooleanOperator(Operator op)
	{
		switch (op)
		{
		case Operator::LessThan:
		case Operator::LessThanOrEqual:
		case Operator::GreaterThan:
		case Operator::GreaterThanOrEqual:
		case Operator::Equequ:
		case Operator::Notequ:
		case Operator::Not:
		case Operator::And:
		case Operator::Or:
			return true;
		default:
			return false;
		}
	}

	void TrentParser::Initialize(const std::string& source)
	{
		InitializeTokenPool(source);

		d_current_token = d_token_pool->Next(nullptr);
	}

	void TrentParser::InitializeTokenPool(const std::string& source)
	{
		lexer::TrentLexer lexer;
		d_token_pool = lexer.ConstructTokenPool(source);
	}

	void TrentParser::Expect(Symbol symbol)
	{
		if (d_current_token->d_type != TokenType::Symbol)
		{
			auto exception = TrentException("Syntax Error", "Expected a symbol", "Line " + std::to_string(d_current_token->d_lineno));
			exception.Raise();
			return;
		}

		if (As<SymbolToken>(d_current_token)->d_symbol != symbol)
		{
			auto exception = TrentException("Syntax Error", "Unexpected symbol found", "Line " + std::to_string(d_current_token->d_lineno));
			exception.Raise();
			return;
		}

		d_current_token = d_token_pool->Next(d_current_token);
	}

	void TrentParser::Expect(Keyword keyword)
	{
		if (d_current_token->d_type != TokenType::Keyword)
		{
			auto exception = TrentException("Syntax Error", "Expected a keyword", "Line " + std::to_string(d_current_token->d_lineno));
			exception.Raise();
			return;
		}

		if (As<KeywordToken>(d_current_token)->d_keyword != keyword)
		{
			auto exception = TrentException("Syntax Error", "Unexpected keyword found", "Line " + std::to_string(d_current_token->d_lineno));
			exception.Raise();
			return;
		}

		d_current_token = d_token_pool->Next(d_current_token);
	}

	void TrentParser::Expect(Operator op)
	{
		if (d_current_token->d_type != TokenType::Operator)
		{
			auto exception = TrentException("Syntax Error", "Expected an operator", "Line " + std::to_string(d_current_token->d_lineno));
			exception.Raise();
			return;
		}

		if (As<OperatorToken>(d_current_token)->d_operator != op)
		{
			auto exception = TrentException("Syntax Error", "Unexpected operator found", "Line " + std::to_string(d_current_token->d_lineno));
			exception.Raise();
			return;
		}

		d_current_token = d_token_pool->Next(d_current_token);
	}

	void TrentParser::Expect(TokenType type)
	{
		if (d_current_token->d_type != type)
		{
			auto exception = TrentException("Syntax Error", "Unexpected token type", "Line " + std::to_string(d_current_token->d_lineno));
			exception.Raise();
			return;
		}

		d_current_token = d_token_pool->Next(d_current_token);
	}

	bool TrentParser::IsSymbol(TokenRef<Token> token, Symbol symbol)
	{
		if (d_current_token->d_type != TokenType::Symbol)
			return false;

		if (As<SymbolToken>(d_current_token)->d_symbol != symbol)
			return false;

		return true;
	}

	bool TrentParser::IsKeyword(TokenRef<Token> token, Keyword keyword)
	{
		if (d_current_token->d_type != TokenType::Keyword)
			return false;

		if (As<KeywordToken>(d_current_token)->d_keyword != keyword)
			return false;

		return true;
	}

	bool TrentParser::IsOperator(TokenRef<Token> token, Operator op)
	{
		if (d_current_token->d_type != TokenType::Operator)
			return false;

		if (As<OperatorToken>(d_current_token)->d_operator != op)
			return false;

		return true;
	}

	NodeRef<ASTNode> TrentParser::ParseStatement()
	{
		switch (d_current_token->d_type)
		{
		case TokenType::Keyword: {
			return ParseKeyword();
		}
		case TokenType::Identifier: {
			return ParseIdentifier();
		}
		default: {
			auto exception = TrentException("Syntax Error", "Cannot parse statement", "Line " + std::to_string(d_current_token->d_lineno));
			exception.Raise();
			return nullptr;
		}
		}
	}

	NodeRef<ASTExpressionNode> TrentParser::ParseExpression()
	{
		NodeRef<ASTExpressionNode> nested_expression_node = nullptr;

		auto expression_node = MakeNode<ASTExpressionNode>();
		expression_node->d_lineno = d_current_token->d_lineno;

		// If expression begins with an opening parenthesis,
		// then there is a nested expression that must be evaluated first.
		if (IsSymbol(d_current_token, Symbol::ParenthesisOpen))
		{
			Expect(Symbol::ParenthesisOpen);

			nested_expression_node = ParseExpression();
			nested_expression_node->d_lineno = d_current_token->d_lineno;
			
			Expect(Symbol::ParenthesisClose);

			if (d_current_token->d_type != TokenType::Operator)
				return nested_expression_node;
		}

		switch (d_current_token->d_type)
		{
		case TokenType::Operator: {
			expression_node->d_value = ParseOperator(nested_expression_node);
			break;
		}
		case TokenType::Identifier: {
			// Check if the next oken is an operator
			if (NextToken()->d_type == TokenType::Operator)
			{
				auto lhs = MakeNode<ASTExpressionNode>();
				lhs->d_value = ParseIdentifier();

				expression_node->d_value = ParseOperator(lhs);
				break;
			}

			expression_node->d_value = ParseIdentifier();
			break;
		}
		case TokenType::LiteralValue: {
			if (NextToken()->d_type == TokenType::Operator)
			{
				auto lhs = MakeNode<ASTExpressionNode>();
				lhs->d_value = ParseLiteralValue();

				expression_node->d_value = ParseOperator(lhs);
				break;
			}

			expression_node->d_value = ParseLiteralValue();
			break;
		}
		default: {
			auto exception = TrentException("Syntax Error", "Cannot evaluate expression", "Line " + std::to_string(d_current_token->d_lineno));
			exception.Raise();
			return nullptr;
		}
		}

		return expression_node;
	}

	NodeRef<ASTNode> TrentParser::ParseFunctionDeclaration()
	{
		// Creating a default null node to be placed as a default return statement.
		auto null_node = MakeNode<ASTLiteralValueNode>(LiteralType::Null, "");
		null_node->d_lineno = d_current_token->d_lineno;

		auto null_expr_node = MakeNode<ASTExpressionNode>();
		null_expr_node->d_lineno = d_current_token->d_lineno;
		null_expr_node->d_value = null_node;

		auto function_declaration_node = MakeNode<ASTFunctionDeclarationNode>();
		function_declaration_node->d_lineno = d_current_token->d_lineno;
		//function_declaration_node->d_return_value = null_expr_node;

		Expect(Keyword::Func);
		function_declaration_node->d_function_name = CurrentToken<IdentifierToken>()->d_value;
		Expect(TokenType::Identifier);
		Expect(Symbol::ParenthesisOpen);
		
		// Parsing function parameters
		NodeRef<ASTNode> parameter = nullptr;

		if (!IsSymbol(d_current_token, Symbol::ParenthesisClose))
		{
			parameter = ParseIdentifier();
			parameter->d_lineno = d_current_token->d_lineno;

			function_declaration_node->d_parameters.push_back(As<ASTVariableNode>(parameter));
		}

		while (IsSymbol(d_current_token, Symbol::Comma))
		{
			Expect(Symbol::Comma);

			parameter = ParseIdentifier();
			parameter->d_lineno = d_current_token->d_lineno;

			function_declaration_node->d_parameters.push_back(As<ASTVariableNode>(parameter));
		}

		// Done parsing parameters
		Expect(Symbol::ParenthesisClose);

		// Now it's time to parse the body enclosed
		// inside open and close curly braces.
		Expect(Symbol::BraceOpen);

		// Check if body is empty
		if (IsSymbol(d_current_token, Symbol::BraceClose))
			return function_declaration_node;

		// Check if return statement is immediately present
		if (IsKeyword(d_current_token, Keyword::Return))
		{
			// Setting the return statement
			function_declaration_node->d_return_value = ParseReturnStatement();

			// Looping until the end of the function
			// ultimately skipping all the next tokens since
			// they won't get executed.
			while (!IsSymbol(d_current_token, Symbol::BraceClose))
				d_current_token = d_token_pool->Next(d_current_token);

			return function_declaration_node;
		}

		// If the body of the function is not empty,
		// start parsing the statements.
		auto body_node = ParseStatement();
		function_declaration_node->d_body.push_back(body_node);

		while (IsSymbol(d_current_token, Symbol::Semicolon))
		{
			Expect(Symbol::Semicolon);

			// Check for a return statement
			if (IsKeyword(d_current_token, Keyword::Return))
			{
				// Setting the return statement
				function_declaration_node->d_return_value = ParseReturnStatement();

				// Looping until the end of the function
				// ultimately skipping all the next tokens since
				// they won't get executed.
				while (!IsSymbol(d_current_token, Symbol::BraceClose))
					d_current_token = d_token_pool->Next(d_current_token);

				return function_declaration_node;
			}

			// EOF or end of function body reached.
			if (d_current_token == nullptr || IsSymbol(d_current_token, Symbol::BraceClose))
				return function_declaration_node;

			body_node = ParseStatement();
			function_declaration_node->d_body.push_back(body_node);
		}

		Expect(Symbol::BraceClose);

		return function_declaration_node;
	}

	NodeRef<ASTExpressionNode> TrentParser::ParseReturnStatement()
	{
		Expect(Keyword::Return);

		return ParseExpression();
	}

	NodeRef<ASTNode> TrentParser::ParseFunctionCall()
	{
		auto function_call_node = MakeNode<ASTFunctionCallNode>();
		function_call_node->d_function_name = PreviousToken<IdentifierToken>()->d_value;
		function_call_node->d_lineno = d_current_token->d_lineno;

		Expect(Symbol::ParenthesisOpen);

		NodeRef<ASTExpressionNode> argument = nullptr;
		
		if (!IsSymbol(d_current_token, Symbol::ParenthesisClose))
		{
			argument = ParseExpression();
			argument->d_lineno = d_current_token->d_lineno;

			function_call_node->d_arguments.push_back(argument);
		}

		while (IsSymbol(d_current_token, Symbol::Comma))
		{
			Expect(Symbol::Comma);

			argument = ParseExpression();
			argument->d_lineno = d_current_token->d_lineno;

			function_call_node->d_arguments.push_back(argument);
		}

		Expect(Symbol::ParenthesisClose);
		return function_call_node;
	}

	NodeRef<ASTNode> TrentParser::ParseVariableDeclaration()
	{
		Expect(Keyword::Var);
		auto variable_name = CurrentToken<IdentifierToken>()->d_value;
		Expect(TokenType::Identifier);
		Expect(Operator::Assignment);

		NodeRef<ASTExpressionNode> variable_value = ParseExpression();
		variable_value->d_lineno = d_current_token->d_lineno;

		auto declaration_node = MakeNode<ASTVariableDeclarationNode>();
		declaration_node->d_variable_name = variable_name;
		declaration_node->d_value = variable_value;
		declaration_node->d_lineno = d_current_token->d_lineno;

		return declaration_node;
	}

	NodeRef<ASTNode> TrentParser::ParseVariableAssignment()
	{
		auto variable_node = MakeNode<ASTVariableNode>(PreviousToken<IdentifierToken>()->d_value);
		variable_node->d_lineno = d_current_token->d_lineno;

		Expect(Operator::Assignment);

		NodeRef<ASTExpressionNode> variable_value = ParseExpression();
		variable_value->d_lineno = d_current_token->d_lineno;

		auto assignment_node = MakeNode<ASTAssignmentNode>();
		assignment_node->d_lineno = d_current_token->d_lineno;
		assignment_node->d_left = variable_node;
		assignment_node->d_right = variable_value;

		return assignment_node;
	}

	NodeRef<ASTNode> TrentParser::ParseVariableIncrement()
	{
		auto variable_node = MakeNode<ASTVariableNode>(PreviousToken<IdentifierToken>()->d_value);
		variable_node->d_lineno = d_current_token->d_lineno;

		Expect(Operator::Increment);

		auto addition_lhs_expression = MakeNode<ASTExpressionNode>();
		addition_lhs_expression->d_lineno = d_current_token->d_lineno;
		addition_lhs_expression->d_value = variable_node;

		auto addition_rhs_expression = MakeNode<ASTExpressionNode>();
		addition_rhs_expression->d_lineno = d_current_token->d_lineno;
		addition_rhs_expression->d_value = MakeNode<ASTLiteralValueNode>(LiteralType::Integer, "1");

		auto addition_node = MakeNode<ASTBinaryOperatorNode>();
		addition_node->d_lineno = d_current_token->d_lineno;
		addition_node->d_op_type = Operator::Add;
		addition_node->d_left = addition_lhs_expression;
		addition_node->d_right = addition_rhs_expression;

		auto variable_value = MakeNode<ASTExpressionNode>();
		variable_value->d_lineno = d_current_token->d_lineno;
		variable_value->d_value = addition_node;

		auto assignment_node = MakeNode<ASTAssignmentNode>();
		assignment_node->d_lineno = d_current_token->d_lineno;
		assignment_node->d_left = variable_node;
		assignment_node->d_right = variable_value;

		return assignment_node;
	}

	NodeRef<ASTNode> TrentParser::ParseVariableDecrement()
	{
		auto variable_node = MakeNode<ASTVariableNode>(PreviousToken<IdentifierToken>()->d_value);
		variable_node->d_lineno = d_current_token->d_lineno;

		Expect(Operator::Decrement);

		auto subtraction_lhs_expression = MakeNode<ASTExpressionNode>();
		subtraction_lhs_expression->d_lineno = d_current_token->d_lineno;
		subtraction_lhs_expression->d_value = variable_node;

		auto subtraction_rhs_expression = MakeNode<ASTExpressionNode>();
		subtraction_rhs_expression->d_lineno = d_current_token->d_lineno;
		subtraction_rhs_expression->d_value = MakeNode<ASTLiteralValueNode>(LiteralType::Integer, "1");

		auto subtraction_node = MakeNode<ASTBinaryOperatorNode>();
		subtraction_node->d_lineno = d_current_token->d_lineno;
		subtraction_node->d_op_type = Operator::Sub;
		subtraction_node->d_left = subtraction_lhs_expression;
		subtraction_node->d_right = subtraction_rhs_expression;

		auto variable_value = MakeNode<ASTExpressionNode>();
		variable_value->d_lineno = d_current_token->d_lineno;
		variable_value->d_value = subtraction_node;

		auto assignment_node = MakeNode<ASTAssignmentNode>();
		assignment_node->d_lineno = d_current_token->d_lineno;
		assignment_node->d_left = variable_node;
		assignment_node->d_right = variable_value;

		return assignment_node;
	}

	NodeRef<ASTNode> TrentParser::ParseVariableAdditionAssignment()
	{
		auto variable_node = MakeNode<ASTVariableNode>(PreviousToken<IdentifierToken>()->d_value);
		variable_node->d_lineno = d_current_token->d_lineno;

		Expect(Operator::AdditionAssignment);

		auto binary_op_lhs = MakeNode<ASTExpressionNode>();
		binary_op_lhs->d_lineno = d_current_token->d_lineno;
		binary_op_lhs->d_value = variable_node;

		auto binary_op_rhs = ParseExpression();

		auto binary_op_node = MakeNode<ASTBinaryOperatorNode>();
		binary_op_node->d_lineno = d_current_token->d_lineno;
		binary_op_node->d_op_type = Operator::Add;
		binary_op_node->d_left = binary_op_lhs;
		binary_op_node->d_right = binary_op_rhs;

		auto variable_value = MakeNode<ASTExpressionNode>();
		variable_value->d_lineno = d_current_token->d_lineno;
		variable_value->d_value = binary_op_node;

		auto assignment_node = MakeNode<ASTAssignmentNode>();
		assignment_node->d_lineno = d_current_token->d_lineno;
		assignment_node->d_left = variable_node;
		assignment_node->d_right = variable_value;

		return assignment_node;
	}

	NodeRef<ASTNode> TrentParser::ParseVariableSubtractionAssignment()
	{
		auto variable_node = MakeNode<ASTVariableNode>(PreviousToken<IdentifierToken>()->d_value);
		variable_node->d_lineno = d_current_token->d_lineno;

		Expect(Operator::SubtractionAssignment);

		auto binary_op_lhs = MakeNode<ASTExpressionNode>();
		binary_op_lhs->d_lineno = d_current_token->d_lineno;
		binary_op_lhs->d_value = variable_node;

		auto binary_op_rhs = ParseExpression();

		auto binary_op_node = MakeNode<ASTBinaryOperatorNode>();
		binary_op_node->d_lineno = d_current_token->d_lineno;
		binary_op_node->d_op_type = Operator::Sub;
		binary_op_node->d_left = binary_op_lhs;
		binary_op_node->d_right = binary_op_rhs;

		auto variable_value = MakeNode<ASTExpressionNode>();
		variable_value->d_lineno = d_current_token->d_lineno;
		variable_value->d_value = binary_op_node;

		auto assignment_node = MakeNode<ASTAssignmentNode>();
		assignment_node->d_lineno = d_current_token->d_lineno;
		assignment_node->d_left = variable_node;
		assignment_node->d_right = variable_value;

		return assignment_node;
	}

	NodeRef<ASTNode> TrentParser::ParseVariableMultiplicationAssignment()
	{
		auto variable_node = MakeNode<ASTVariableNode>(PreviousToken<IdentifierToken>()->d_value);
		variable_node->d_lineno = d_current_token->d_lineno;

		Expect(Operator::MultiplicationAssignment);

		auto binary_op_lhs = MakeNode<ASTExpressionNode>();
		binary_op_lhs->d_lineno = d_current_token->d_lineno;
		binary_op_lhs->d_value = variable_node;

		auto binary_op_rhs = ParseExpression();

		auto binary_op_node = MakeNode<ASTBinaryOperatorNode>();
		binary_op_node->d_lineno = d_current_token->d_lineno;
		binary_op_node->d_op_type = Operator::Mul;
		binary_op_node->d_left = binary_op_lhs;
		binary_op_node->d_right = binary_op_rhs;

		auto variable_value = MakeNode<ASTExpressionNode>();
		variable_value->d_lineno = d_current_token->d_lineno;
		variable_value->d_value = binary_op_node;

		auto assignment_node = MakeNode<ASTAssignmentNode>();
		assignment_node->d_lineno = d_current_token->d_lineno;
		assignment_node->d_left = variable_node;
		assignment_node->d_right = variable_value;

		return assignment_node;
	}

	NodeRef<ASTNode> TrentParser::ParseVariableDivisionAssignment()
	{
		auto variable_node = MakeNode<ASTVariableNode>(PreviousToken<IdentifierToken>()->d_value);
		variable_node->d_lineno = d_current_token->d_lineno;

		Expect(Operator::DivisionAssignment);

		auto binary_op_lhs = MakeNode<ASTExpressionNode>();
		binary_op_lhs->d_lineno = d_current_token->d_lineno;
		binary_op_lhs->d_value = variable_node;

		auto binary_op_rhs = ParseExpression();

		auto binary_op_node = MakeNode<ASTBinaryOperatorNode>();
		binary_op_node->d_lineno = d_current_token->d_lineno;
		binary_op_node->d_op_type = Operator::Div;
		binary_op_node->d_left = binary_op_lhs;
		binary_op_node->d_right = binary_op_rhs;

		auto variable_value = MakeNode<ASTExpressionNode>();
		variable_value->d_lineno = d_current_token->d_lineno;
		variable_value->d_value = binary_op_node;

		auto assignment_node = MakeNode<ASTAssignmentNode>();
		assignment_node->d_lineno = d_current_token->d_lineno;
		assignment_node->d_left = variable_node;
		assignment_node->d_right = variable_value;

		return assignment_node;
	}

	NodeRef<ASTNode> TrentParser::ParseLiteralValue()
	{
		Expect(TokenType::LiteralValue);

		auto node = MakeNode<ASTLiteralValueNode>(
			PreviousToken<LiteralValueToken>()->d_value_type,
			PreviousToken<LiteralValueToken>()->d_value
		);
		node->d_lineno = d_current_token->d_lineno;

		return node;
	}

	NodeRef<ASTNode> TrentParser::ParseIdentifier()
	{
		Expect(TokenType::Identifier);

		if (IsSymbol(d_current_token, Symbol::ParenthesisOpen))
			return ParseFunctionCall();

		if (IsOperator(d_current_token, Operator::Assignment))
			return ParseVariableAssignment();

		if (IsOperator(d_current_token, Operator::Increment))
			return ParseVariableIncrement();

		if (IsOperator(d_current_token, Operator::Decrement))
			return ParseVariableDecrement();

		if (IsOperator(d_current_token, Operator::AdditionAssignment))
			return ParseVariableAdditionAssignment();

		if (IsOperator(d_current_token, Operator::SubtractionAssignment))
			return ParseVariableSubtractionAssignment();

		if (IsOperator(d_current_token, Operator::MultiplicationAssignment))
			return ParseVariableMultiplicationAssignment();

		if (IsOperator(d_current_token, Operator::DivisionAssignment))
			return ParseVariableDivisionAssignment();

		auto node = MakeNode<ASTVariableNode>(PreviousToken<IdentifierToken>()->d_value);
		node->d_lineno = d_current_token->d_lineno;

		return node;
	}

	NodeRef<ASTNode> TrentParser::ParseKeyword()
	{
		switch (CurrentToken<KeywordToken>()->d_keyword)
		{
		case Keyword::Var: {
			return ParseVariableDeclaration();
		}
		case Keyword::Func: {
			return ParseFunctionDeclaration();
		}
		case Keyword::Return: {
			return As<ASTNode>(ParseReturnStatement());
		}
		case Keyword::While: {
			return ParseWhileLoop();
		}
		default: {
			return nullptr;
		}
		}
	}

	NodeRef<ASTNode> TrentParser::ParseOperator(TokenRef<ASTExpressionNode> lhs)
	{
		if (IsOperator(NextToken(), Operator::Assignment))
		{
			auto exception = TrentException("Syntax Error", "Cannot use assignment operator inside an expression", "Line " + std::to_string(d_current_token->d_lineno));
			exception.Raise();
			return nullptr;
		}

		// Checking if the operator is a binary operator
		if (IsBinaryOperator(CurrentToken<OperatorToken>()->d_operator))
		{
			auto operator_node = MakeNode<ASTBinaryOperatorNode>();
			operator_node->d_lineno = d_current_token->d_lineno;
			operator_node->d_op_type = CurrentToken<OperatorToken>()->d_operator;
			operator_node->d_left = lhs;

			Expect(TokenType::Operator);

			operator_node->d_right = ParseExpression();

			return operator_node;
		}

		// Checking if the operator is a boolean operator
		if (IsBooleanOperator(CurrentToken<OperatorToken>()->d_operator))
		{
			auto operator_node = MakeNode<ASTBooleanOperatorNode>();
			operator_node->d_lineno = d_current_token->d_lineno;
			operator_node->d_op_type = CurrentToken<OperatorToken>()->d_operator;
			operator_node->d_left = lhs;

			Expect(TokenType::Operator);

			operator_node->d_right = ParseExpression();

			return operator_node;
		}

		auto exception = TrentException("Syntax Error", "Unrecognized operator", "Line " + std::to_string(d_current_token->d_lineno));
		exception.Raise();
		return nullptr;
	}

	NodeRef<ASTNode> TrentParser::ParseWhileLoop()
	{
		auto loop_node = MakeNode<ASTWhileLoopNode>();
		loop_node->d_lineno = d_current_token->d_lineno;

		Expect(Keyword::While);
		Expect(Symbol::ParenthesisOpen);

		auto condition = ParseExpression();
		condition->d_lineno = d_current_token->d_lineno;

		loop_node->d_condition = condition;

		Expect(Symbol::ParenthesisClose);
		Expect(Symbol::BraceOpen);

		// Check if body is empty
		if (IsSymbol(d_current_token, Symbol::BraceClose))
			return condition;

		// If the body of the function is not empty,
		// start parsing the statements.
		auto body_node = ParseStatement();
		loop_node->d_body.push_back(body_node);

		while (IsSymbol(d_current_token, Symbol::Semicolon))
		{
			Expect(Symbol::Semicolon);

			// EOF or end of function body reached.
			if (d_current_token == nullptr || IsSymbol(d_current_token, Symbol::BraceClose))
				return loop_node;

			body_node = ParseStatement();
			loop_node->d_body.push_back(body_node);
		}

		Expect(Symbol::BraceClose);

		return loop_node;
	}

	std::shared_ptr<AST> TrentParser::ConstructAST()
	{
		auto ast = std::make_shared<AST>();
		auto program_root = MakeNode<ASTProgramNode>();

		auto node = ParseStatement();
		if (node->d_type == ASTNodeType::FunctionDeclaration)
			ast->d_functions[As<ASTFunctionDeclarationNode>(node)->d_function_name] = As<ASTFunctionDeclarationNode>(node);
		else
			program_root->d_children.push_back(node);

		while (
			IsSymbol(d_current_token, Symbol::Semicolon) ||
			IsSymbol(d_current_token, Symbol::BraceClose)
		)
		{
			if (CurrentToken<SymbolToken>()->d_symbol == Symbol::Semicolon)
				Expect(Symbol::Semicolon);
			else
				Expect(Symbol::BraceClose);

			// EOF reached
			if (d_current_token == nullptr)
				break;

			node = ParseStatement();

			// Making sure no errors occured
			if (!node)
			{
				auto exception = TrentException("Syntax Error", "Cannot parse statement", "Line " + std::to_string(d_current_token->d_lineno));
				exception.Raise();
				return ast;
			}

			if (node->d_type == ASTNodeType::FunctionDeclaration)
				ast->d_functions[As<ASTFunctionDeclarationNode>(node)->d_function_name] = As<ASTFunctionDeclarationNode>(node);
			else
				program_root->d_children.push_back(node);
		}

		//// x
		//auto xVar = MakeNode<ASTVariableNode>("x");
		//
		//auto xVarExpr = MakeNode<ASTExpressionNode>();
		//xVarExpr->d_value = xVar;

		//// var x = 0;
		//auto xExpr = MakeNode<ASTExpressionNode>();
		//xExpr->d_value = MakeNode<ASTLiteralValueNode>(LiteralType::Integer, "0");

		//auto xDecl = MakeNode<ASTVariableDeclarationNode>();
		//xDecl->d_variable_name = "x";
		//xDecl->d_value = xExpr;

		//// x < 10
		//auto lhsExpr = MakeNode<ASTExpressionNode>();
		//lhsExpr->d_value = xVar;

		//auto rhsExpr = MakeNode<ASTExpressionNode>();
		//rhsExpr->d_value = MakeNode<ASTLiteralValueNode>(LiteralType::Integer, "10");

		//auto boolOp = MakeNode<ASTBooleanOperatorNode>();
		//boolOp->d_op_type = Operator::LessThan;
		//boolOp->d_left = lhsExpr;
		//boolOp->d_right = rhsExpr;

		//auto condition = MakeNode<ASTExpressionNode>();
		//condition->d_value = boolOp;

		//// x = x + 1;
		//auto literal1Expr = MakeNode<ASTExpressionNode>();
		//literal1Expr->d_value = MakeNode<ASTLiteralValueNode>(LiteralType::Integer, "1");

		//auto addNode = MakeNode<ASTBinaryOperatorNode>();
		//addNode->d_op_type = Operator::Add;
		//addNode->d_left = xVarExpr;
		//addNode->d_right = literal1Expr;

		//auto addExpr = MakeNode<ASTExpressionNode>();
		//addExpr->d_value = addNode;

		//auto assignment = MakeNode<ASTAssignmentNode>();
		//assignment->d_left = xVar;
		//assignment->d_right = addExpr;

		//// println(x);
		//auto printParam = MakeNode<ASTExpressionNode>();
		//printParam->d_value = xVar;

		//auto printNode = MakeNode<ASTFunctionCallNode>();
		//printNode->d_function_name = "println";
		//printNode->d_arguments.push_back(printParam);

		//// sleep(1000);
		//auto sleepParam = MakeNode<ASTExpressionNode>();
		//sleepParam->d_value = MakeNode<ASTLiteralValueNode>(LiteralType::Integer, "400");

		//auto sleepNode = MakeNode<ASTFunctionCallNode>();
		//sleepNode->d_function_name = "sleep";
		//sleepNode->d_arguments.push_back(sleepParam);

		//// while (x < 10) {
		////     x = x + 1;
		////     print(x);
		////     sleep(1000);
		//// }
		//auto whileLoopNode = MakeNode<ASTWhileLoopNode>();
		//whileLoopNode->d_condition = condition;
		//whileLoopNode->d_body.push_back(assignment);
		//whileLoopNode->d_body.push_back(printNode);
		//whileLoopNode->d_body.push_back(sleepNode);

		//program_root->d_children.push_back(xDecl);
		//program_root->d_children.push_back(whileLoopNode);

		ast->d_root = program_root;
		return ast;
	}
}
